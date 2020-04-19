//
// Created by n.orlov on 02/01/2020.
//

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <simgrid/s4u.hpp>
#include "simulation_result.h"
#include "simulator_pipeline.h"

namespace s4u = simgrid::s4u;

namespace aphrodite
{
class Simulator
{
private:
    aphrodite::SimulatorPipeline pipeline;
    s4u::Engine & engine;

public:
    explicit Simulator();

    explicit Simulator(s4u::Engine & engine_, const SimulatorPipeline & pipeline_)
        : pipeline(pipeline_), engine(engine_)
    {
    }

    explicit Simulator(s4u::Engine & engine_, const std::vector<PipelineStepPtr> & steps_)
        : pipeline(steps_), engine(engine_)
    {
    }

    aphrodite::SimulationResult run();
};

} // namespace aphrodite
#endif // SIMULATOR_H
