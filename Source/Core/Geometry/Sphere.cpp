#include "Sphere.h"

std::optional<HitRecord> Sphere::hit(const Ray& ray, double minT, double maxT) const
{
    XYZ ray2Sphere = ray.origin() - m_center;
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
    XYZ normal = (hitPoint - m_center) / m_radius;
    bool isFront = dot(ray.direction(), normal) < 0;
    return HitRecord(std::move(hitPoint), isFront ? std::move(normal) : -normal, root, isFront, this);
}