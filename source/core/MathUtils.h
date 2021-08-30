#pragma once

#include <cmath>
#include <limits>

constexpr double DOUBLE_INFINITY = std::numeric_limits<double>::infinity();
constexpr double PI = 3.1415926535897932385;

inline double degrees2Radians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double radians2Degrees(double radians)
{
    return radians * 180.0 / PI;
}