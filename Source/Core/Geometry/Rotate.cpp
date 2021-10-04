#include "Rotate.h"

Rotate::Rotate(std::shared_ptr<GeometryBase> pGeometry, double angle)
    : m_pObject(pGeometry)
{
    double radians = degrees2Radians(angle);
    m_sinTheta = std::sin(radians);
    m_cosTheta = std::cos(radians);
}

std::optional<HitRecord> Rotate::hit(const Ray& ray, double minT, double maxT) const
{
    const XYZ& oldOrigin = ray.origin();
    const XYZ& oldDirection = ray.direction();

    XYZ origin = ray.origin();
    XYZ direction = ray.direction();

    origin[0] = m_cosTheta * oldOrigin[0] - m_sinTheta * oldOrigin[2];
    origin[2] = m_sinTheta * oldOrigin[0] + m_cosTheta * oldOrigin[2];

    direction[0] = m_cosTheta * oldDirection[0] - m_sinTheta * oldDirection[2];
    direction[2] = m_sinTheta * oldDirection[0] + m_cosTheta * oldDirection[2];

    Ray rotatedRay(origin, direction, ray.delayTime());
    std::optional<HitRecord> optHitRecord = m_pObject->hit(rotatedRay, minT, maxT);
    if (optHitRecord.has_value())
    {
        HitRecord& hitRecord = optHitRecord.value();

        const XYZ& oldHitPoint = hitRecord.hitPoint();
        const XYZ& oldNormal = hitRecord.normal();

        XYZ hitPoint = hitRecord.hitPoint();
        XYZ normal = hitRecord.normal();

        hitPoint[0] = m_cosTheta * oldHitPoint[0] + m_sinTheta * oldHitPoint[2];
        hitPoint[2] = -m_sinTheta * oldHitPoint[0] + m_cosTheta * oldHitPoint[2];

        normal[0] = m_cosTheta * oldNormal[0] + m_sinTheta * oldNormal[2];
        normal[2] = -m_sinTheta * oldNormal[0] + m_cosTheta * oldNormal[2];

        hitRecord.hitPointForWrite() = std::move(hitPoint);

        bool isFront = dot(rotatedRay.direction(), normal) < 0;
        hitRecord.normalForWrite() = isFront ? std::move(normal) : std::move(normal.inverse());
    }

    return optHitRecord;
}

UV Rotate::uv(const XYZ& point) const
{
    return m_pObject->uv(point);
}

std::optional<AABB> Rotate::boundingBox(double t0, double t1) const
{
    return m_pObject->boundingBox(t0, t1);
}