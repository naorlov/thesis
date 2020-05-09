#pragma once

#include <string>
#include "function.h"

namespace aphrodite
{
class StepContext
{
public:
    std::string type;
    double input_size;
    aphrodite::Function flop_function;
    aphrodite::Function output_size_function;

public:
    StepContext() = default;

    StepContext(const std::string & type_, const double input_size_, const YAML::Node functions)
    {
        type = type_;
        input_size = input_size_;
        output_size_function = aphrodite::Function(functions["space"]);
        flop_function = aphrodite::Function(functions["time"]);
    }
};

}
