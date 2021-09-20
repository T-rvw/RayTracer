#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>

constexpr double DOUBLE_EPS = 0.000000001;
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

inline int randomInt(int min, int max)
{
    return min + rand() % (max - min);
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

// Use Schlick's approximation for reflectance.
inline double reflectance(double cosine, double reflectionIndex)
{
    double r0 = (1.0 - reflectionIndex) / (1.0 + reflectionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}