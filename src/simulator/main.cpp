//
// Created by n.orlov on 02/01/2020.
//
#include <pipeline.h>
#include <simulator.h>
#include <simulator_pipeline.h>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
    po::options_description description("Allowed options");

    // clang-format off
    description.add_options()
        ("help", "Show help message");
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);


    if (vm.count("help"))
    {
        std::cout << description << std::endl;
        return 1;
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
