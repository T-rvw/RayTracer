#pragma once

#include "GeometryBase.h"

class Rotate final : public GeometryBase
{
public:
    explicit Rotate(std::shared_ptr<GeometryBase> pGeometry, double angle);

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    double m_sinTheta;
    double m_cosTheta;
    std::shared_ptr<GeometryBase> m_pObject;
};