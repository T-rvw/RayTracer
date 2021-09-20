#pragma once

#include "GeometryBase.h"

class AARect : public GeometryBase
{
public:
    AARect() = delete;
    AARect(XYZ p0, XYZ p1, char kIndex, double k);
    virtual ~AARect() = default;

    AARect(const AARect& rhs) = delete;
    AARect(AARect&& rhs) = default;
    AARect& operator=(const AARect&) = default;
    AARect& operator=(AARect&&) = default;

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT = -10000000.0, double maxT = +10000000.0) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    XYZ						    m_p0, m_p1;
    double						m_k;
    int                         m_verticalAxisIndex;
};