#pragma once

#include "GeometryBase.h"
#include "HittableList.h"

class Box final : public GeometryBase
{
public:
    Box() = delete;
    explicit Box(XYZ p0, XYZ p1);
    virtual ~Box() = default;

    Box(const Box& rhs) = delete;
    Box(Box&& rhs) = default;
    Box& operator=(const Box&) = delete;
    Box& operator=(Box&&) = default;

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

    virtual void setMaterial(std::shared_ptr<Material> pMaterial) override;

private:
    XYZ						    m_p0, m_p1;
    HittableList                m_hittableList;
};