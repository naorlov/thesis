//
// Created by n.orlov on 02/01/2020.
//
#include <iostream>

#include "simgrid/s4u/Engine.hpp"

#include <simulator.h>
#include <simulator_pipeline.h>
#include "pipeline.h"
#include "simulation_result.h"


aphrodite::SimulationResult aphrodite::Simulator::run(s4u::Engine & engine)
{
    aphrodite::SimulationResult result;

    engine.load_platform(this->platform_path.c_str());

    this->build_actors();

    return result;
}


void aphrodite::Simulator::build_actors()
{
    
}