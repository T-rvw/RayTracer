#pragma once

#include "AABB.h"
#include "HitRecord.h"
#include "Material.h"

#include <memory>
#include <optional>

class GeometryBase
{
public:
    GeometryBase() = default;
    virtual ~GeometryBase() = default;

    GeometryBase(const GeometryBase&) = delete;
    GeometryBase(GeometryBase&&) = delete;
    GeometryBase& operator=(const GeometryBase&) = delete;
    GeometryBase& operator=(GeometryBase&&) = delete;

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const = 0;
    virtual std::optional<AABB> boundingBox(double t0, double t1) const = 0;

    virtual UV uv(const XYZ& /*point*/) const = 0;

    virtual void setMaterial(std::shared_ptr<Material> pMaterial) { m_pMaterial = pMaterial; }
    std::shared_ptr<Material> material() const { return m_pMaterial; }

    void setMoveInfo(XYZ endPos, double beginTime, double endTime)
    {
        m_moveEndPos = std::move(endPos);
        m_moveBeginTime = beginTime;
        m_moveEndTime = endTime;
    }

protected:
    // move parameters
    XYZ                         m_moveEndPos;
    double                      m_moveBeginTime = 0.0;
    double                      m_moveEndTime = 0.0;

    // style parameters
    std::shared_ptr<Material>   m_pMaterial;
};