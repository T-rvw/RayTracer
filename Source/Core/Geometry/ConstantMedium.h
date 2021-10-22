#pragma once

#include "GeometryBase.h"
#include "Material.h"
#include "Texture.h"

class ConstantMedium final : public GeometryBase
{
public:
    explicit ConstantMedium(std::shared_ptr<GeometryBase> pBoundary, double density);
    virtual ~ConstantMedium() = default;

    ConstantMedium(const ConstantMedium& rhs) = delete;
    ConstantMedium(ConstantMedium&& rhs) = default;
    ConstantMedium& operator=(const ConstantMedium&) = delete;
    ConstantMedium& operator=(ConstantMedium&&) = default;

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    double                          m_negativeInverseDensity;
    std::shared_ptr<GeometryBase>   m_pBoundary;
};