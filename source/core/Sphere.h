#pragma once

#include "Hitable.h"

class Sphere : public Hitable
{
public:
    Sphere() = delete;
    Sphere(XYZ center, double radius) : m_center(center), m_radius(radius) {};
    virtual ~Sphere() = default;

    Sphere(const Sphere& rhs) { m_center = rhs.center(); m_radius = rhs.radius(); };
    Sphere& operator=(const Sphere&) = default;
    Sphere& operator=(Sphere&&) = default;

    XYZ center() const { return m_center; }
    double radius() const { return m_radius; }

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT = -10000000.0, double maxT = +10000000.0) const;

private:
    XYZ     m_center;
    double  m_radius;
};