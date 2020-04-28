#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>
#include "simgrid/s4u.hpp"
#include "simgrid/s4u/Actor.hpp"
#include "simgrid/s4u/Comm.hpp"
#include "simgrid/s4u/Host.hpp"
#include "simgrid/s4u/Mailbox.hpp"

namespace fs = std::filesystem;

namespace aphrodite
{
class Function
{
private:
    uint64_t degree;
    std::vector<double> polynomial_part;
    std::vector<double> logarithmic_part;

public:
    Function() = default;
    Function(YAML::Node function)
    {
        degree = function["degree"].as<uint64_t>();

        for (auto coef : function["logarithmic_part"])
        {
            logarithmic_part.push_back(coef.as<double>());
        }
        for (auto coef : function["polynomial_part"])
        {
            polynomial_part.push_back(coef.as<double>());
        }
    }
    double operator()(double & x) const
    {
        double result = 0;

        for (size_t power = 0; power < polynomial_part.size(); ++power)
        {
            result += polynomial_part[power] * std::pow(x, power);
        }

        for (size_t power = 0; power < logarithmic_part.size(); ++power)
        {
            result += logarithmic_part[power] * std::log(x) * std::pow(x, power);
        }

        return result;
    }
};

}

namespace s4u = simgrid::s4u;

std::map<std::string, std::string> parse_args(int argc, char ** argv)
{
    std::map<std::string, std::string> options;

    if (argc < 2)
    {
        std::cerr << "Not enough arguments" << std::endl;
        exit(1);
    }
    options["specfile"] = argv[1];
    options["platform"] = argv[2];
    return options;
}

class StepContext
{
public:
    std::string type;
    double input_size;
    aphrodite::Function flop_function;
    aphrodite::Function output_size_function;

public:
    StepContext() = default;

    StepContext(std::string & type_, double input_size_, YAML::Node functions)
    {
        type = type_;
        input_size = input_size_;
        output_size_function = aphrodite::Function(functions["space"]);
        flop_function = aphrodite::Function(functions["time"]);
    }
};

static void executor(const std::vector<StepContext> & steps)
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

    std::vector<StepContext> pipeline;

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
            std::string dependency_name = spec["stages"][stage]["after"].as<std::string>();
            auto prev_step_size_function = aphrodite::Function(result[dependency_name]["space"]);

            auto simulation_size = spec["simulation"][dependency_name]["input_size"].as<double>();
            step_input_size = spec["simulation"][stage]["input_size"].as<double>();
            std::cout << step_input_size << '\n';
        }

        StepContext current_step(type, step_input_size, functions);

        pipeline.push_back(current_step);
    }


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
