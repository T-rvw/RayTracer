#pragma once

#include "Perlin.h"
#include "Texture.h"

class NoiseTexture final : public Texture
{
public:
    NoiseTexture() = default;
    explicit NoiseTexture(double scale) : m_scale(scale) {}
    virtual ~NoiseTexture() = default;

    NoiseTexture(const NoiseTexture&) = delete;
    NoiseTexture(NoiseTexture&&) = delete;
    NoiseTexture& operator=(const NoiseTexture&) = delete;
    NoiseTexture& operator=(NoiseTexture&&) = delete;

    virtual Color value(double /*u*/, double /*v*/, const XYZ& p) const
    {
        return Color(0.5, 0.5, 0.5) * (1.0 + m_perlinNoise.noise(p * m_scale));
    }

private:
    double m_scale = 1.0;
    Perlin m_perlinNoise;
};