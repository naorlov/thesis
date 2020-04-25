//
// Created by n.orlov on 02/01/2020.
//

#ifndef SIMULATOR_PIPELINE_H
#define SIMULATOR_PIPELINE_H

#include <memory>
#include <vector>

#include "pipeline.h"
#include "simulation_result.h"

namespace aphrodite
{
class SimulatorPipeline
{
private:
    std::vector<PipelineStepPtr> steps;

public:
    explicit SimulatorPipeline() = default;

    SimulatorPipeline(std::initializer_list<PipelineStep *> steps_)
    {
        for (PipelineStep * step : steps_)
        {
            steps.push_back(std::shared_ptr<PipelineStep>(step));
        }
    };

    SimulatorPipeline(std::initializer_list<std::shared_ptr<PipelineStep>> steps_) : steps(steps_)
    {
    }

    SimulatorPipeline(const std::vector<PipelineStepPtr> & steps_) : steps(steps_) {}


    SimulatorPipeline & operator=(const SimulatorPipeline & other) = default;

    aphrodite::SimulationResult run();
    size_t size() { return steps.size(); }
};

} // namespace aphrodite

#endif // HW02_SIMULATOR_PIPELINE_H
