#include "CheckerTexture.h"

Color CheckerTexture::value(double u, double v, const XYZ& p) const
{
    double sines = std::sin(10.0 * p.x()) * std::sin(10.0 * p.y()) * std::sin(10.0 * p.z());
    if (sines > DOUBLE_EPS)
    {
        return m_pEvenTexture->value(u, v, p);
    }

    return m_pOddTexture->value(u, v, p);
}