#pragma once

#include "GeometryBase.h"

class Sphere : public GeometryBase
{
public:
    Sphere() = delete;
    Sphere(XYZ center, double radius);
    virtual ~Sphere() = default;

    Sphere(const Sphere& rhs) { m_center = rhs.center(); m_radius = rhs.radius(); }
    Sphere(Sphere&& rhs) = default;
    Sphere& operator=(const Sphere& rhs) { m_center = rhs.center(); m_radius = rhs.radius(); }
    Sphere& operator=(Sphere&&) = default;

    XYZ center(double curTime = 0.0) const;
    double radius() const { return m_radius; }

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    XYZ                         m_center;
    double                      m_radius;
};