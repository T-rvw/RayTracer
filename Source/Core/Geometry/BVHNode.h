#pragma once

#include "HittableList.h"

class BVHNode : public GeometryBase
{
public:
    BVHNode() = default;
    virtual ~BVHNode() = default;

    BVHNode(const HittableList& hitList, double t0, double t1);

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    AABB                    m_box;
    GeometryBase* const     m_pLeft;
    GeometryBase* const     m_pRight;
};