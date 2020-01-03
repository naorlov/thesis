//
// Created by n.orlov on 02/01/2020.
//
#include <simulator.h>

#include <simulator_pipeline.h>

#include <iostream>

aphrodite::Simulator::Simulator()
{
    this->pipeline = aphrodite::SimulatorPipeline();

    std::cout << "Simulator build();" << std::endl;
}

aphrodite::SimulationResult aphrodite::Simulator::run()
{
    return this->pipeline.run();
}