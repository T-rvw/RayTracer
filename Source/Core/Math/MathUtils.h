#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>

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

inline double randomDouble()
{
    // [0, 1)
    return rand() / (RAND_MAX + 1.0);
}

inline double stdRandomDouble()
{
    // Another new version generator
    // [0, 1)
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max)
{
    // [min, max)
    return min + (max - min) * randomDouble();
}

inline double clamp(double x, double min, double max)
{
    return x < min ? min : (x > max ? max : x);
}