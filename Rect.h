#pragma once

#include "GeometryBase.h"

class Rect : public GeometryBase
{
public:
    Rect() = delete;
    Rect(XYZ p0, XYZ p1, char kIndex, double depth);
    virtual ~Rect() = default;

    Rect(const Rect& rhs) = delete;
    Rect(Rect&& rhs) = default;
    Rect& operator=(const Rect&) = delete;
    Rect& operator=(Rect&&) = default;

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    XYZ						    m_p0, m_p1;
    double						m_depth;
    int                         m_verticalAxisIndex;
};