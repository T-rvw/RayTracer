#include "HitRecord.h"
#include "Sphere.h"

Sphere::Sphere(XYZ center, double radius) :
    m_center(center),
    m_radius(radius)
{
}

XYZ Sphere::center(double curTime) const
{
    if (std::fabs(m_moveBeginTime - m_moveEndTime) < DOUBLE_EPS)
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

UV Sphere::uv(const XYZ& point) const
{
    // u : [0, 1] start from -x -> +z -> +x -> -z -> -x
    // v : [0, 1] start from -y -> -z -> +y -> +z -> -y

    // y = -cos(vAngle)
    // cos(vAngle) = -y
    double vAngle = std::acos(-point.y());

    // Project to x-z plane
    // length = sin(vAngle)
    // z = length * cos(uAngle - PI * 0.5)
    // z = length * cos(PI * 0.5 - uAngle)
    // z = length * sin(uAngle)
    // z = sin(vAngle) * sin(uAngle)
    // x = length * sin(uAngle - PI * 0.5)
    // x = length * -sin(0.5 * PI - uAngle)
    // x = length * -cos(uAngle)
    // x = -sin(vAngle) * cos(uAngle)
    // x / z = -cos(uAngle)
    // uAngle = atan(-x / z)
    double uAngle = std::atan2(-point.z(), point.x()) + PI;

    // u = uAngle / 2 * PI
    // v = vAngle / PI
    double u = (uAngle * 0.5) / PI;
    double v = vAngle / PI;

    return UV(u, v);
}

std::optional<AABB> Sphere::boundingBox(double t0, double t1) const
{
    XYZ radiusScale(m_radius, m_radius, m_radius);
    AABB box0(center(t0) - radiusScale,
              center(t0) + radiusScale);

    AABB box1(center(t1) - radiusScale,
              center(t1) + radiusScale);

    return AABB::merge(box0, box1);
}