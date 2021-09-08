#pragma once

#include "Perlin.h"
#include "Texture.h"

class NoiseTexture : public Texture
{
public:
    NoiseTexture() = default;
    virtual ~NoiseTexture() = default;

    NoiseTexture(const NoiseTexture&) = delete;
    NoiseTexture(NoiseTexture&&) = delete;
    NoiseTexture& operator=(const NoiseTexture&) = delete;
    NoiseTexture& operator=(NoiseTexture&&) = delete;

    virtual Color value(double /*u*/, double /*v*/, const XYZ& p) const;

private:
    Perlin m_perlinNoise;
};