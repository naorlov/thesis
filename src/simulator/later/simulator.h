//
// Created by n.orlov on 02/01/2020.
//

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <simgrid/s4u.hpp>
#include "simgrid/s4u/Engine.hpp"
#include "simulation_result.h"
#include "simulator_pipeline.h"

namespace s4u = simgrid::s4u;

namespace aphrodite
{
class Simulator
{
private:
    aphrodite::SimulatorPipeline pipeline;
    std::string platform_path;
    std::string hosts_available;

public:
    explicit Simulator() = default;

    Simulator(Simulator & other) = delete;
    Simulator(Simulator && other) = delete;
    void operator=(Simulator other) = delete;

    aphrodite::SimulationResult run(s4u::Engine & engine);

private:
    void build_actors();
};

} // namespace aphrodite
#endif // SIMULATOR_H
