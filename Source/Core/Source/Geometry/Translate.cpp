#include "Translate.h"

std::optional<HitRecord> Translate::hit(const Ray& ray, double minT, double maxT) const
{
    Ray movedRay(ray.origin() - m_offset, ray.direction(), ray.delayTime());
    
    std::optional<HitRecord> optHitRecord = m_pObject->hit(movedRay, minT, maxT);
    if (optHitRecord.has_value())
    {
        HitRecord& hitRecord = optHitRecord.value();
        hitRecord.hitPointForWrite() += m_offset;

        XYZ outwardNormal = hitRecord.normal();
        bool isFront = dot(movedRay.direction(), outwardNormal) < 0;
        hitRecord.normalForWrite() = isFront ? std::move(outwardNormal) : std::move(outwardNormal.inverse());
    }

    return optHitRecord;
}

UV Translate::uv(const XYZ& point) const
{
    return m_pObject->uv(point);
}