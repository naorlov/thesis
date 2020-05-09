#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>
#include "function.h"
#include "simgrid/s4u.hpp"
#include "simgrid/s4u/Actor.hpp"
#include "simgrid/s4u/Comm.hpp"
#include "simgrid/s4u/Host.hpp"
#include "simgrid/s4u/Mailbox.hpp"
#include "step_context.h"
#include "utils.h"

namespace fs = std::filesystem;
namespace s4u = simgrid::s4u;

static void executor(const std::vector<aphrodite::StepContext> & steps)
{
    std::cout << "Running executor " << s4u::this_actor::get_cname() << std::endl;
    for (auto step : steps)
    {
        auto input_size = step.input_size;
        auto flopcount = step.flop_function(input_size);
        auto output_size = step.output_size_function(input_size);

        //TODO : more smart disk selection
        auto current_host = s4u::Host::current();
        // auto disks = current_host->get_disks();
        // auto disk = disks.front();
        // std::cout << s4u::Engine::get_clock() << " " << s4u::this_actor::get_cname()
        //           << " starting read of " << input_size << " lines with disk speed "
        //           << disk->get_read_bandwidth() << "." << std::endl;

        // disk->read(input_size);
        // std::cout << s4u::Engine::get_clock() << " " << s4u::this_actor::get_cname()
        //           << " read done." << std::endl;

        std::cout << s4u::Engine::get_clock() << " " << s4u::this_actor::get_cname()
                  << " starting execution of " << flopcount << " operations with speed "
                  << s4u::this_actor::get_host()->get_speed() << "." << std::endl;

        s4u::this_actor::execute(flopcount);

        // std::cout << s4u::Engine::get_clock() << " " << s4u::this_actor::get_cname()
        //           << " execution done." << std::endl;

        // std::cout << s4u::Engine::get_clock() << " " << s4u::this_actor::get_cname()
        //           << " starting write of " << output_size << " lines with disk speed "
        //           << disk->get_write_bandwidth() << "." << std::endl;
        // disk->write(output_size);
        // std::cout << s4u::Engine::get_clock() << " " << s4u::this_actor::get_cname() << "done."
        //           << std::endl;
    }
}

std::vector<aphrodite::StepContext>
build_pipeline(const YAML::Node & result, const YAML::Node & spec, const )
{
    for (auto node : result)
    {
        auto stage = node.first.as<std::string>();
        auto type = spec["stages"][stage]["type"].as<std::string>();
        auto functions = node.second;

        double step_input_size = 0;
        if (type == "mapper")
        {
            step_input_size = spec["simulation"][stage]["input_size"].as<double>() / hosts.size();
        }
        else if (type == "reducer")
        {
            auto dependency_name = spec["stages"][stage]["after"].as<std::string>();
            auto prev_step_size_function = aphrodite::Function(result[dependency_name]["space"]);

            auto simulation_size = spec["simulation"][dependency_name]["input_size"].as<double>();
            step_input_size = prev_step_size_function(simulation_size);
            std::cout << simulation_size << " " << step_input_size << '\n';
        }

        aphrodite::StepContext current_step(type, step_input_size, functions);

        pipeline.push_back(current_step);
    }
}

int simple(int argc, char ** argv)
{
    auto varmap = parse_args(argc, argv);

    s4u::Engine e(&argc, argv);

    fs::path specfile(varmap["specfile"]);
    if (!fs::exists(specfile))
    {
        std::cerr << "Specfile is not found!" << std::endl;
        return 1;
    }

    auto resultfile = specfile.parent_path() / "result.yaml";
    if (!fs::exists(resultfile))
    {
        std::cerr << "Resultfile is not found!" << std::endl;
        return 1;
    }

    auto spec = YAML::LoadFile(specfile);
    auto result = YAML::LoadFile(resultfile);

    e.load_platform(varmap["platform"]);
    auto hosts = e.get_all_hosts();

    std::vector<aphrodite::StepContext> pipeline;


    for (auto host : hosts)
    {
        std::cout << "Creating actor " << std::string(host->get_name()) + std::string("Actor")
                  << std::endl;
        simgrid::s4u::Actor::create(
            /* name = */ std::string(host->get_name()) + std::string("Actor"),
            /* host = */ host,
            /* code = */ executor,
            /* pipeline = */ pipeline);
    }

    e.run();
    std::cout << e.get_clock() << std::endl;
    return 0;
}

int main(int argc, char ** argv)
{
    return simple(argc, argv);
}
