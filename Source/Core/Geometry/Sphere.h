#pragma once

#include "Hittable.h"

#include <memory>

class Sphere : public Hittable
{
public:
    Sphere() = delete;
    Sphere(XYZ center, double radius, std::shared_ptr<Material> pMaterial = nullptr) :
        m_center(center),
        m_radius(radius),
        m_pMaterial(pMaterial)
    {

    };
    virtual ~Sphere() = default;

    Sphere(const Sphere& rhs) { m_center = rhs.center(); m_radius = rhs.radius(); }
    Sphere(Sphere&& rhs) = default;
    Sphere& operator=(const Sphere&) = default;
    Sphere& operator=(Sphere&&) = default;

    XYZ center() const { return m_center; }
    double radius() const { return m_radius; }
    virtual std::shared_ptr<Material> material() const override { return m_pMaterial; }

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT = -10000000.0, double maxT = +10000000.0) const override;

private:
    XYZ                         m_center;
    double                      m_radius;
    std::shared_ptr<Material>   m_pMaterial;
};