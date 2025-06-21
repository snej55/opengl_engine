//
// Created by Jens Kromdijk on 07/04/25.
//

#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>

namespace Util
{
    inline float random()
    {
        return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    }
}

#endif // UTIL_H
