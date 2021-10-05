#pragma once

#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <random>

constexpr double DOUBLE_EPS = 0.000000001;
constexpr double DOUBLE_INFINITY = std::numeric_limits<double>::infinity();
constexpr double PI = 3.1415926535897932385;

namespace MathUtils
{
    inline double degrees2Radians(double degrees)
    {
        constexpr double temp = PI / 180.0;
        return degrees * temp;
    }

    inline double radians2Degrees(double radians)
    {
        constexpr double temp = 180.0 / PI;
        return radians * temp;
    }

    inline int randomInt(int min, int max)
    {
        // [min, max]
        return min + rand() % (max - min + 1);
    }

    inline double randomDouble()
    {
        // [0, 1)
        return rand() / (RAND_MAX + 1.0);
    }

    inline double randomDouble(double min, double max)
    {
        // [min, max)
        return min + (max - min) * randomDouble();
    }

    // Use Schlick's approximation for MathUtils::reflectance.
    inline double reflectance(double cosine, double reflectionIndex)
    {
        double r0 = (1.0 - reflectionIndex) / (1.0 + reflectionIndex);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
}