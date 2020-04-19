//
// Created by n.orlov on 02/01/2020.
//
#include <simulator.h>

#include <simulator_pipeline.h>

#include <iostream>

aphrodite::SimulationResult aphrodite::Simulator::run()
{
    
    return this->pipeline.run();
}