#pragma once

#include "GeometryBase.h"

class AARect : public GeometryBase
{
public:
    AARect() = delete;
    AARect(XYZ p0, XYZ p1, char kIndex, double depth);
    virtual ~AARect() = default;

    AARect(const AARect& rhs) = delete;
    AARect(AARect&& rhs) = default;
    AARect& operator=(const AARect&) = delete;
    AARect& operator=(AARect&&) = default;

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    XYZ						    m_p0, m_p1;
    double						m_depth;
    int                         m_verticalAxisIndex;
};