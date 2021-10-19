#pragma once

#include "GeometryBase.h"

class Triangle : public GeometryBase
{
public:
    Triangle() = delete;
    Triangle(XYZ p0, XYZ p1, XYZ p2);
    virtual ~Triangle() = default;

    Triangle(const Triangle& rhs) = delete;
    Triangle(Triangle&& rhs) = default;
    Triangle& operator=(const Triangle&) = default;
    Triangle& operator=(Triangle&&) = default;

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    XYZ     m_p0, m_p1, m_p2;
    XYZ     m_normal;
    double  m_distance;
};