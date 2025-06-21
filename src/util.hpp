//
// Created by Jens Kromdijk on 07/04/25.
//

#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>
#include <fstream>
#include <string>

namespace Util
{
    inline float random()
    {
        return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    }

    inline bool fileExists(const std::string& name)
    {
        std::ifstream file {name.c_str()};
        return file.good();
    }
}

#endif // UTIL_H
