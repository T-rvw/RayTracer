#include "GeometryBase.h"

HitRecord::HitRecord(XYZ hitPoint, XYZ normal, double rayT, bool isFront, const GeometryBase* pHitObject) :
    m_hitPoint(std::move(hitPoint)),
    m_normal(std::move(normal)),
    m_rayT(rayT),
    m_isFront(isFront),
    m_pHitObject(pHitObject)
{
    if (m_pHitObject)
    {
        m_hitPointUV = m_pHitObject->uv(normal);
    }
}