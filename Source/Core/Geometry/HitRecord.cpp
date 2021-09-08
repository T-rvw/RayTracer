#include <HitRecord.h>
#include <Hittable.h>

HitRecord::HitRecord(XYZ hitPoint, XYZ normal, double rayT, bool isFront, const Hittable* pHitObject) :
    m_hitPoint(hitPoint),
    m_normal(normal),
    m_rayT(rayT),
    m_isFront(isFront),
    m_pHitObject(pHitObject)
{
    if (m_pHitObject)
    {
        m_hitPointUV = m_pHitObject->uv(hitPoint);
    }
}