#include "AABB.h"

AABB::AABB(XYZ p0, XYZ p1)
    : m_min(std::move(p0)),
      m_max(std::move(p1))
{
}

AABB::AABB(const AABB& rhs)
{
    m_min = rhs.m_min;
    m_max = rhs.m_max;
}

bool AABB::hit(const Ray& ray, double minT, double maxT) const
{
    const XYZ& origin = ray.origin();
    const XYZ& direction = ray.direction();

    for (int ii = 0; ii < 3; ++ii)
    {
        double inverseDirection = 1 / direction[ii];
        double minTi = (m_min[ii] - origin[ii]) * inverseDirection;
        double maxTi = (m_max[ii] - origin[ii]) * inverseDirection;

        if (inverseDirection < 0.0f)
        {
            std::swap(minTi, maxTi);
        }

        double newMinT = std::fmax(minT, minTi);
        double newMaxT = std::fmin(maxT, maxTi);
        if (newMaxT <= newMinT)
        {
            return false;
        }
    }

    return true;
}

AABB AABB::merge(const AABB& lhs, const AABB& rhs)
{
    XYZ small(std::fmin(lhs.m_min.x(), rhs.m_min.x()),
              std::fmin(lhs.m_min.y(), rhs.m_min.y()),
              std::fmin(lhs.m_min.z(), rhs.m_min.z()));

    XYZ big(std::fmax(lhs.m_max.x(), rhs.m_max.x()),
            std::fmax(lhs.m_max.y(), rhs.m_max.y()),
            std::fmax(lhs.m_max.z(), rhs.m_max.z()));

    return AABB(small, big);
}