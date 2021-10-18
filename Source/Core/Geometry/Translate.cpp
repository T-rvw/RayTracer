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
        bool isFront = XYZ::dot(movedRay.direction(), outwardNormal) < 0;
        hitRecord.normalForWrite() = isFront ? std::move(outwardNormal) : std::move(outwardNormal.inverse());
    }

    return optHitRecord;
}

UV Translate::uv(const XYZ& point) const
{
    return m_pObject->uv(point);
}

std::optional<AABB> Translate::boundingBox(double t0, double t1) const
{
    std::optional optBox = m_pObject->boundingBox(t0, t1);
    if (!optBox.has_value())
    {
        return optBox;
    }

    AABB& box = optBox.value();
    box.minForWrite() += m_offset;
    box.maxForWrite() += m_offset;

    return optBox;
}