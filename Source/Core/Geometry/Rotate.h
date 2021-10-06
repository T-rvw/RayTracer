#pragma once

#include "GeometryBase.h"

class Rotate : public GeometryBase
{
public:
    Rotate(GeometryBase* pGeometry, double angle);

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    double m_sinTheta;
    double m_cosTheta;
    GeometryBase* m_pObject;
};