#include "NoiseTexture.h"

Color NoiseTexture::value(double /*u*/, double /*v*/, const XYZ& p) const
{
    return Color(1.0, 1.0, 1.0) * m_perlinNoise.noise(p);
}