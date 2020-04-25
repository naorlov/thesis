#include <cmath>
#include <function.h>

double aphrodite::Function::operator()(double x)
{
    double result = 0;

    for (size_t power = 0; power < this->_function.linear_part().size(); ++power)
    {
        result += this->_function.linear_part(power) * std::pow(x, power);
    }

    for (size_t power = 0; power < this->_function.log_part().size(); ++power)
    {
        result += this->_function.log_part(power) * std::pow(x, power) * std::log(x);
    }

    return result;
}