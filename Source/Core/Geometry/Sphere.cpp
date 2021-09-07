#include "Sphere.h"

#include <cmath>

Sphere::Sphere(XYZ center, double radius, std::shared_ptr<Material> pMaterial) :
    m_center(center),
    m_radius(radius),
    m_pMaterial(pMaterial)
{
}

XYZ Sphere::center(double curTime) const
{
    if (isStatic())
    {
        return m_center;
    }

    return m_center + ((curTime - m_moveBeginTime) / (m_moveEndTime - m_moveBeginTime)) * (m_moveEndPos - m_center);
}

std::optional<HitRecord> Sphere::hit(const Ray& ray, double minT, double maxT) const
{
    XYZ ray2Sphere = ray.origin() - center(ray.delayTime());
    double a = ray.direction().lengthSquare();
    double half_b = dot(ray2Sphere, ray.direction());
    double c = ray2Sphere.lengthSquare() - m_radius * m_radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < DOUBLE_EPS)
    {
        return std::nullopt;
    }

    // Find the nearest root that lies in the acceptable range.
    double discriminantSqrt = std::sqrt(discriminant);
    double root = (-half_b - discriminantSqrt) / a;
    if (root < minT || root > maxT)
    {
        // Only one root.
        if (discriminant < DOUBLE_EPS)
        {
            return std::nullopt;
        }

        // Check another root.
        root = (-half_b + discriminantSqrt) / a;
        if (root < minT || root > maxT)
        {
            return std::nullopt;
        }
    }

    // Return final hit result.
    XYZ hitPoint = ray.at(root);
    XYZ outwardNormal = (hitPoint - center(ray.delayTime())) / m_radius;
    bool isFront = dot(ray.direction(), outwardNormal) < DOUBLE_EPS;
    return HitRecord(std::move(hitPoint), isFront ? std::move(outwardNormal) : std::move(outwardNormal.inverse()), root, isFront, this);
}