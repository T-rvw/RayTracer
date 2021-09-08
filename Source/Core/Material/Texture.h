#pragma once

#include "XYZ.h"

class Texture
{
public:
    Texture() = default;
    virtual ~Texture() = default;

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    virtual Color value(double u, double v, const XYZ& p) const = 0;
};