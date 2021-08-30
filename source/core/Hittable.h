#pragma once

#include "Ray.h"

#include <optional>

class HitRecord
{
public:
    HitRecord() = delete;
    HitRecord(XYZ hitPoint, XYZ normal, double rayT) : m_hitPoint(hitPoint), m_normal(normal), m_rayT(rayT) {};
    ~HitRecord() = default;

    HitRecord(const HitRecord&) = default;
    HitRecord(HitRecord&&) = default;
    HitRecord& operator=(const HitRecord&) = default;
    HitRecord& operator=(HitRecord&&) = default;

    const XYZ& hitPoint() const { return m_hitPoint; }
    const XYZ& normal() const { return m_normal; }
    double rayT() const { return m_rayT; }

private:
    XYZ     m_hitPoint;
    XYZ     m_normal;
    double  m_rayT;
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