#include "NoiseTexture.h"

Color NoiseTexture::value(double /*u*/, double /*v*/, const XYZ& p) const
{
    constexpr bool bUseTrubulence = false;
    if constexpr (bUseTrubulence)
    {
        return Color(1.0, 1.0, 1.0) * m_perlinNoise.trubulence(p * m_scale);
    }
    else
    {
        return Color(0.5, 0.5, 0.5) * (1.0 + m_perlinNoise.noise(p * m_scale));
    }
}