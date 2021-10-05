#pragma once

#include "Ray.h"
#include "XY.h"

class GeometryBase;

class HitRecord
{
public:
    HitRecord() = delete;
    HitRecord(XYZ hitPoint, XYZ normal, double rayT, bool isFront, const GeometryBase* pHitObject);
    ~HitRecord() = default;

    HitRecord(const HitRecord&) = default;
    HitRecord(HitRecord&&) = default;
    HitRecord& operator=(const HitRecord&) = default;
    HitRecord& operator=(HitRecord&&) = default;

    XYZ& hitPointForWrite() { return m_hitPoint; }
    const XYZ& hitPoint() const { return m_hitPoint; }
    const UV& hitPointUV() const { return m_hitPointUV; }
    XYZ& normalForWrite() { return m_normal; }
    const XYZ& normal() const { return m_normal; }
    double rayT() const { return m_rayT; }
    double& rayTForWrite() { return m_rayT; }
    bool isFront() const { return m_isFront; }
    const GeometryBase* hitObject() const { return m_pHitObject; }

private:
    XYZ                     m_hitPoint;
    UV                      m_hitPointUV;
    XYZ                     m_normal;
    double                  m_rayT;
    bool                    m_isFront;
    const GeometryBase*     m_pHitObject;
};