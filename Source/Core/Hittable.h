#pragma once

#include "Ray.h"

#include <memory>
#include <optional>

class Hittable;
class Material;

class HitRecord
{
public:
    HitRecord() = delete;
    HitRecord(XYZ hitPoint, XYZ normal, double rayT, bool isFront, const Hittable* pHitObject) :
        m_hitPoint(hitPoint),
        m_normal(normal),
        m_rayT(rayT),
        m_isFront(isFront),
        m_pHitObject(pHitObject)
    {
    };
    ~HitRecord() = default;

    HitRecord(const HitRecord&) = default;
    HitRecord(HitRecord&&) = default;
    HitRecord& operator=(const HitRecord&) = default;
    HitRecord& operator=(HitRecord&&) = default;

    const XYZ& hitPoint() const { return m_hitPoint; }
    const XYZ& normal() const { return m_normal; }
    double rayT() const { return m_rayT; }
    bool isFront() const { return m_isFront; }
    const Hittable* hitObject() const { return m_pHitObject; }

private:
    XYZ               m_hitPoint;
    XYZ               m_normal;
    double            m_rayT;
    bool              m_isFront;
    const Hittable*   m_pHitObject;
};

// Interface
class Hittable
{
public:
    Hittable() = default;
    virtual ~Hittable() = default;

    Hittable(const Hittable&) = delete;
    Hittable(Hittable&&) = delete;
    Hittable& operator=(const Hittable&) = delete;
    Hittable& operator=(Hittable&&) = delete;

    virtual std::shared_ptr<Material> material() const = 0;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const = 0;
};