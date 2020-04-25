#ifndef FUNCITON_H
#define FUNCITON_H

#include <vector>

#include <function.pb.h>

namespace aphrodite
{
class Function
{
private:
    aphrodite::protobufs::function _function;

public:
    Function(protobufs::function function) : _function(function) {}

    double operator()(double x);
};

}


#endif