//
// Created by n.orlov on 07/01/2020.
//

#ifndef CLCONF_H
#define CLCONF_H

#include <istream>
namespace aphrodite
{
namespace clconf
{
    class ClConf
    {
    private:
    public:
        ClConf() = default;


        friend std::istream & operator>>(std::istream & stream, ClConf & config);
    };

}

}
#endif //CLCONF_H
