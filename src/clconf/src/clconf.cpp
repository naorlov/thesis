//
// Created by n.orlov on 07/01/2020.
//

#include <clconf.h>


std::istream &
aphrodite::clconf::operator>>(std::istream & stream, aphrodite::clconf::ClConf & config)
{
    return stream;
}
