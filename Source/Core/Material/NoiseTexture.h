#pragma once

#include "Perlin.h"
#include "Texture.h"

class NoiseTexture : public Texture
{
public:
    NoiseTexture() = default;
    NoiseTexture(double scale) : m_scale(scale) {}
    virtual ~NoiseTexture() = default;

    NoiseTexture(const NoiseTexture&) = delete;
    NoiseTexture(NoiseTexture&&) = delete;
    NoiseTexture& operator=(const NoiseTexture&) = delete;
    NoiseTexture& operator=(NoiseTexture&&) = delete;

    virtual Color value(double /*u*/, double /*v*/, const XYZ& p) const;

private:
    double m_scale = 1.0;
    Perlin m_perlinNoise;
};