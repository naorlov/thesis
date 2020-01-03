//
// Created by n.orlov on 02/01/2020.
//

#ifndef SIMULATION_RESULT_H
#define SIMULATION_RESULT_H

#include <string>
#include <vector>
namespace aphrodite
{
//class SimulationResult
//{
//    std::vector<size_t> times;
//
//public:
//    explicit SimulationResult() = default;
//
//    SimulationResult(SimulationResult && other) = default;
//    void append_entry(const std::string & entry_name);
//    void append_entry(const aphrodite::SimulationResult & result);
//};


class SimulationResult
{
private:
    std::vector<size_t> times;

public:
    SimulationResult() = default;

    //    SimulationResult(SimulationResult && other) = default;
};

} // namespace aphrodite

#endif // SIMULATION_RESULT_H
