#pragma once

#include "Perlin.h"
#include "Texture.h"

class TurbulenceTexture final : public Texture
{
public:
    TurbulenceTexture() = default;
    explicit TurbulenceTexture(double scale) : m_scale(scale) {}
    virtual ~TurbulenceTexture() = default;

    TurbulenceTexture(const TurbulenceTexture&) = delete;
    TurbulenceTexture(TurbulenceTexture&&) = delete;
    TurbulenceTexture& operator=(const TurbulenceTexture&) = delete;
    TurbulenceTexture& operator=(TurbulenceTexture&&) = delete;

    virtual Color value(double /*u*/, double /*v*/, const XYZ& p) const
    {
        return Color(1.0, 1.0, 1.0) * m_perlinNoise.trubulence(p * m_scale);
    }

private:
    double m_scale = 1.0;
    Perlin m_perlinNoise;
};