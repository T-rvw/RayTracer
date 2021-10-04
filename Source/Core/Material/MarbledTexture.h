#pragma once

#include "Perlin.h"
#include "Texture.h"

class MarbledTexture : public Texture
{
public:
    MarbledTexture() = default;
    MarbledTexture(double scale) : m_scale(scale) {}
    virtual ~MarbledTexture() = default;

    MarbledTexture(const MarbledTexture&) = delete;
    MarbledTexture(MarbledTexture&&) = delete;
    MarbledTexture& operator=(const MarbledTexture&) = delete;
    MarbledTexture& operator=(MarbledTexture&&) = delete;

    virtual Color value(double /*u*/, double /*v*/, const XYZ& p) const
    {
        return Color(0.5, 0.5, 0.5) * (1.0 + std::sin(m_scale * p.z() + 10.0 * m_perlinNoise.trubulence(p)));
    }

private:
    double m_scale = 1.0;
    Perlin m_perlinNoise;
};