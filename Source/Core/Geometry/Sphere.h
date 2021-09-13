#pragma once

#include "Hittable.h"

#include <memory>

class Sphere : public Hittable
{
public:
    Sphere() = delete;
    Sphere(XYZ center, double radius, std::shared_ptr<Material> pMaterial = nullptr);
    virtual ~Sphere() = default;

    Sphere(const Sphere& rhs) { m_center = rhs.center(); m_radius = rhs.radius(); }
    Sphere(Sphere&& rhs) = default;
    Sphere& operator=(const Sphere&) = default;
    Sphere& operator=(Sphere&&) = default;

    bool isStatic() const { return m_moveBeginTime == m_moveEndTime || m_center == m_moveEndPos; }
    void setMoveInfo(XYZ endPos, double beginTime, double endTime)
    {
        m_moveEndPos = std::move(endPos);
        m_moveBeginTime = beginTime;
        m_moveEndTime = endTime;
    };

    XYZ center(double curTime = 0.0) const;
    double radius() const { return m_radius; }
    virtual std::shared_ptr<Material> material() const { return m_pMaterial; }

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT = -10000000.0, double maxT = +10000000.0) const override;

    virtual UV uv(const XYZ& point) const;

private:
    XYZ                         m_center;
    double                      m_radius;
    XYZ                         m_moveEndPos;
    double                      m_moveBeginTime = 0.0;
    double                      m_moveEndTime = 0.0;
    std::shared_ptr<Material>   m_pMaterial;
};