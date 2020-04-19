//
// Created by n.orlov on 02/01/2020.
//

#include <pipeline.h>
#include <simulator_pipeline.h>

aphrodite::SimulationResult aphrodite::SimulatorPipeline::run()
{
    aphrodite::SimulationResult result;
    for (auto & entry : steps)
    {
        entry->run();
    }
    return result;
}