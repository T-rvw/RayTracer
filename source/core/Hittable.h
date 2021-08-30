#pragma once

#include "Ray.h"

#include <optional>

class HitRecord
{
public:
    HitRecord() = delete;
    HitRecord(XYZ hitPoint, XYZ normal, double rayT, bool isFront) :
        m_hitPoint(hitPoint),
        m_normal(normal),
        m_rayT(rayT),
        m_isFront(isFront)
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

private:
    XYZ     m_hitPoint;
    XYZ     m_normal;
    double  m_rayT;
    bool    m_isFront;
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

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const = 0;
};