#include "ConstantMedium.h"

ConstantMedium::ConstantMedium(std::shared_ptr<GeometryBase> pBoundary, double density)
    : m_negativeInverseDensity(-1 / density),
    m_pBoundary(pBoundary)
{
}

std::optional<AABB> ConstantMedium::boundingBox(double t0, double t1) const
{
    return m_pBoundary->boundingBox(t0, t1);
}

std::optional<HitRecord> ConstantMedium::hit(const Ray& ray, double minT, double maxT) const
{
    std::optional<HitRecord> optHitResult1;

    optHitResult1 = m_pBoundary->hit(ray, -DOUBLE_INFINITY, DOUBLE_INFINITY);
    if (!optHitResult1.has_value())
    {
        return std::nullopt;
    }

    std::optional<HitRecord> optHitResult2;
    HitRecord& hitRecord1 = optHitResult1.value();
    
    optHitResult2 = m_pBoundary->hit(ray, hitRecord1.rayT() + 0.0001, DOUBLE_INFINITY);
    if (!optHitResult2.has_value())
    {
        return std::nullopt;
    }

    if (hitRecord1.rayT() < minT)
    {
        hitRecord1.rayTForWrite() = minT;
    }

    HitRecord& hitRecord2 = optHitResult2.value();
    if (hitRecord2.rayT() > maxT)
    {
        hitRecord2.rayTForWrite() = maxT;
    }

    if (hitRecord1.rayT() >= hitRecord2.rayT())
    {
        return std::nullopt;
    }

    if (hitRecord1.rayT() < 0.0)
    {
        hitRecord1.rayTForWrite() = 0.0;
    }

    double rayLength = ray.direction().length();
    double deltaT = hitRecord2.rayT() - hitRecord1.rayT();
    double distanceInsideBoundary = deltaT * rayLength;
    double hitDistance = m_negativeInverseDensity * std::log(MathUtils::randomDouble());

    if (hitDistance > distanceInsideBoundary)
    {
        return std::nullopt;
    }

    double rayT = hitRecord1.rayT() + hitDistance / rayLength;
    return HitRecord(ray.at(rayT), XYZ(1.0, 0.0, 0.0), rayT, true, this);
}

UV ConstantMedium::uv(const XYZ& point) const
{
    return m_pBoundary->uv(point);
}
