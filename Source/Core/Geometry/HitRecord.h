#pragma once

#include "Ray.h"
#include "XY.h"

#include <memory>
#include <optional>

class Hittable;

class HitRecord
{
public:
    HitRecord() = delete;
    HitRecord(XYZ hitPoint, XYZ normal, double rayT, bool isFront, const Hittable* pHitObject);
    ~HitRecord() = default;

    HitRecord(const HitRecord&) = default;
    HitRecord(HitRecord&&) = default;
    HitRecord& operator=(const HitRecord&) = default;
    HitRecord& operator=(HitRecord&&) = default;

    const XYZ& hitPoint() const { return m_hitPoint; }
    UV hitPointUV() const { return m_hitPointUV; }
    const XYZ& normal() const { return m_normal; }
    double rayT() const { return m_rayT; }
    bool isFront() const { return m_isFront; }
    const Hittable* hitObject() const { return m_pHitObject; }

private:
    XYZ                 m_hitPoint;
    UV                  m_hitPointUV;
    XYZ                 m_normal;
    double              m_rayT;
    bool                m_isFront;
    const Hittable*     m_pHitObject;
};