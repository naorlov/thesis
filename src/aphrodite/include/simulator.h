//
// Created by n.orlov on 02/01/2020.
//

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "simulation_result.h"
#include "simulator_pipeline.h"

namespace aphrodite
{
class Simulator
{
private:
    aphrodite::SimulatorPipeline pipeline;

public:
    explicit Simulator();

    explicit Simulator(const SimulatorPipeline & pipeline_) : pipeline(pipeline_)
    {
        std::cout << "Construct with pipeline of length " << pipeline.size() << "\n";
    }

    aphrodite::SimulationResult run();
};

} // namespace aphrodite
#endif // SIMULATOR_H
