#include <fstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <simgrid/s4u.hpp>
#include "function.pb.h"
#include "simgrid/forward.h"
#include "simgrid/s4u/Actor.hpp"
#include "simgrid/s4u/Comm.hpp"
#include "simgrid/s4u/Mailbox.hpp"

namespace aphrodite
{
class Function
{
private:
    aphrodite::protobufs::function _function;

public:
    Function(protobufs::function function) : _function(function) {}

    double operator()(double x)
    {
        double result = 0;

        for (size_t power = 0; power < this->_function.linear_part().size(); ++power)
        {
            result += this->_function.linear_part(power) * std::pow(x, power);
        }

        for (size_t power = 0; power < this->_function.log_part().size(); ++power)
        {
            result += this->_function.log_part(power) * std::pow(x, power) * std::log(x);
        }

        return result;
    }
};

}


namespace po = boost::program_options;
XBT_LOG_NEW_DEFAULT_CATEGORY(s4u_actor_create, "The logging channel used in this example");

namespace s4u = simgrid::s4u;

static void mapper()
{
    XBT_INFO("Starting mapper");
    simgrid::s4u::Disk * disk = s4u::Host::current()->get_disks().front();
    XBT_INFO("Reading data form disk");
    disk->read(400000000);

    XBT_INFO("Executing something");
    s4u::this_actor::execute(700e6);

    XBT_INFO("Writing data to disk");
    disk->write(300000000);

    XBT_INFO("Task done");
}

static void pinger(simgrid::s4u::Mailbox * mailbox_in, simgrid::s4u::Mailbox * mailbox_out)
{
    auto host = s4u::Actor::self()->get_host();
    XBT_INFO(
        "Actor count on host %s: %d, available speed: %f",
        host->get_name().c_str(),
        host->get_actor_count(),
        host->get_available_speed());

    XBT_INFO(
        "Ping from mailbox %s to mailbox %s",
        mailbox_in->get_name().c_str(),
        mailbox_out->get_name().c_str());

    /* - Do the ping with a 1-Byte payload (latency bound) ... */
    double * payload = new double();
    *payload = simgrid::s4u::Engine::get_clock();

    mailbox_out->put(payload, 1);
    /* - ... then wait for the (large) pong */
    const double * sender_time = static_cast<double *>(mailbox_in->get());

    double communication_time = simgrid::s4u::Engine::get_clock() - *sender_time;
    XBT_INFO("Payload received : large communication (bandwidth bound)");
    XBT_INFO("Pong time (bandwidth bound): %.3f", communication_time);
    delete sender_time;
}

static void ponger(simgrid::s4u::Mailbox * mailbox_in, simgrid::s4u::Mailbox * mailbox_out)
{
    XBT_INFO(
        "Pong from mailbox %s to mailbox %s",
        mailbox_in->get_name().c_str(),
        mailbox_out->get_name().c_str());

    /* - Receive the (small) ping first ....*/
    const double * sender_time = static_cast<double *>(mailbox_in->get());
    double communication_time = simgrid::s4u::Engine::get_clock() - *sender_time;
    XBT_INFO("Payload received : small communication (latency bound)");
    XBT_INFO("Ping time (latency bound) %f", communication_time);
    delete sender_time;

    /*  - ... Then send a 1GB pong back (bandwidth bound) */
    double * payload = new double();
    *payload = simgrid::s4u::Engine::get_clock();
    XBT_INFO("payload = %.3f", *payload);

    mailbox_out->put(payload, 1e9);
}


po::variables_map parse_args(int argc, char ** argv)
{
    try
    {
        po::options_description description("Simple options");

        // clang-format off
        description.add_options()
            ("help",     ("Produce help message"))
            ("platform", po::value<std::string>()->required(), ("Platform file"))
            ("host", po::value<std::string>(), ("Hostname in config file"));
        // clang-format on
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, description), vm);

        if (vm.count("help"))
        {
            std::cout << description << std::endl;
            exit(0);
        }

        po::notify(vm);
        return vm;
    }
    catch (po::required_option & exception)
    {
        std::cerr << exception.what() << std::endl;
        exit(1);
    }
}

class AppContext
{
public:
    aphrodite::Function mapper_function() const;
    aphrodite::Function reducer_function() const;
};


using AppContextPtr = std::shared_ptr<AppContext>;
using ConstAppContextPtr = std::shared_ptr<const AppContext>;

class MapperContext
{
public:
    double input_size() const;
    aphrodite::Function time_function() const;
    aphrodite::Function space_function() const;

    s4u::Mailbox * coordinator_mailbox;
};

using ConstMapperContextPtr = std::shared_ptr<const MapperContext>;

class ReducerContext
{
public:
    double input_size() const;
    aphrodite::Function time_function() const;
    aphrodite::Function space_function() const;
};

using ConstReducerContextPtr = std::shared_ptr<const ReducerContext>;


static void mapper(ConstMapperContextPtr mapper_context)
{
    auto time_complexity_function = mapper_context->time_function();
    auto space_complexity_function = mapper_context->space_function();
    auto input_size = mapper_context->input_size();

    auto work_size = time_complexity_function(input_size);
    auto output_data_size = space_complexity_function(input_size);

    simgrid::s4u::Disk * disk = s4u::Host::current()->get_disks().front();

    disk->read(input_size);
    s4u::this_actor::execute(work_size);
    disk->write(output_data_size);

    std::string msg = s4u::this_actor::get_name();
    std::string * msgptr = new std::string(msg);
    mapper_context->coordinator_mailbox->put_async(msgptr, 16)->wait();
}

static void reducer(ConstReducerContextPtr reducer_context)
{
}

static void spill_and_combine(ConstMapperContextPtr)
{
}

int simple(int argc, char ** argv)
{
    simgrid::s4u::Engine e(&argc, argv);
    auto varmap = parse_args(argc, argv);

    e.load_platform(varmap["platform"].as<std::string>());

    ConstAppContextPtr app_context = std::make_shared<const AppContext>();

    simgrid::s4u::Mailbox * mb1 = simgrid::s4u::Mailbox::by_name("coordinator_input");

    auto hosts = e.get_all_hosts();
    for (auto host : hosts)
    {
        std::stringstream actor_name_stream;
        actor_name_stream << host->get_cname() << "_actor";

        MapperContext ctx;

        simgrid::s4u::Actor::create(actor_name_stream.str(), host, mapper, app_context);
    }

    return 0;
}

int main(int argc, char ** argv)
{
    return simple(argc, argv);
}
