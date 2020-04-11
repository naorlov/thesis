//
// Created by n.orlov on 02/01/2020.
//
#include <vector>
#include <pipeline.h>
#include <simulator.h>
#include <simulator_pipeline.h>
#include <boost/program_options.hpp>
#include <simgrid/s4u.hpp>

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

int simple(int argc, char ** argv)
{
    simgrid::s4u::Engine e(&argc, argv);

    po::options_description description("Simple options");

    // clang-format off
    description.add_options()
        ("platform", po::value<std::string>(), ("Platform file"))
        ("host", po::value<std::string>(), ("Hostname in config file"));
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    e.load_platform(vm["platform"].as<std::string>());
    simgrid::s4u::Actor::create(
        "mapper", simgrid::s4u::Host::by_name(vm["host"].as<std::string>()), &mapper);

    e.run();
    std::cout << e.get_clock() << std::endl;
    return 0;
}

int main(int argc, char ** argv)
{
    if (argc == 1)
    {
        std::cout << "Use `" << argv[0] << " simple` to run simple simulation" << std::endl;
        return 1;
    }
    if (strcmp(argv[1], "simple") == 0)
    {
        return simple(argc - 1, argv + 1);
    }

    // clang-format off
    aphrodite::SimulatorPipeline pipeline {
        new aphrodite::mapper::Init(),
        new aphrodite::mapper::Read(),
        new aphrodite::mapper::Execute(),
        new aphrodite::mapper::Save(),
    };
    // clang-format on

    aphrodite::Simulator s(pipeline);
    aphrodite::SimulationResult result = s.run();
}
