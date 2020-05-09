#pragma once

#include <vector>
#include <yaml-cpp/yaml.h>

namespace aphrodite
{
class Function
{
private:
    uint64_t degree;
    std::vector<double> polynomial_part;
    std::vector<double> logarithmic_part;

public:
    Function() = default;
    
    Function(YAML::Node function)
    {
        degree = function["degree"].as<uint64_t>();

        for (auto coef : function["logarithmic_part"])
        {
            logarithmic_part.push_back(coef.as<double>());
        }
        for (auto coef : function["polynomial_part"])
        {
            polynomial_part.push_back(coef.as<double>());
        }
    }

    double operator()(double & x) const
    {
        double result = 0;

        for (size_t power = 0; power < polynomial_part.size(); ++power)
        {
            result += polynomial_part[power] * std::pow(x, power);
        }

        for (size_t power = 0; power < logarithmic_part.size(); ++power)
        {
            result += logarithmic_part[power] * std::log(x) * std::pow(x, power);
        }

        return result;
    }
};

}
