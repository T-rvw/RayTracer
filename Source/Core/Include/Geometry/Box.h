#pragma once

#include "GeometryBase.h"
#include "HittableList.h"

class Box : public GeometryBase
{
public:
    Box() = delete;
    Box(XYZ p0, XYZ p1);
    virtual ~Box() = default;

    Box(const Box& rhs) = delete;
    Box(Box&& rhs) = default;
    Box& operator=(const Box&) = default;
    Box& operator=(Box&&) = default;

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT = -10000000.0, double maxT = +10000000.0) const override;
    
    virtual void setMaterial(std::shared_ptr<Material> pMaterial) override;
    virtual UV uv(const XYZ& point) const override;
    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;

private:
    XYZ						    m_p0, m_p1;
    HittableList                m_hittableList;
};