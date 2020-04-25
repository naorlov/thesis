//
// Created by n.orlov on 02/01/2020.
//
#include <fstream>
#include <vector>
#include <fcntl.h>
#include <pipeline.h>
#include <simulator.h>
#include <simulator_pipeline.h>
#include <boost/program_options.hpp>
#include <simgrid/s4u.hpp>
#include "function.pb.h"
#include "simgrid/s4u/Engine.hpp"


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

namespace po = boost::program_options;

po::variables_map parse_args(int argc, char ** argv)
{
    try
    {
        po::options_description description("Simple options");

        // clang-format off
        description.add_options()
            ("help",     ("Produce help message"))
            ("platform", po::value<std::string>()->required(), ("Platform file"))
            ("host",     po::value<std::string>(), ("Hostname in config file"));
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

void setup_libraries()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
}

int main(int argc, char ** argv)
{
    setup_libraries();
    s4u::Engine engine(&argc, argv);
    aphrodite::Simulator simulator();
    
    auto result = simulator.run(engine);
}
