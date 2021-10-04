#include "BVHNode.h"

std::optional<AABB> BVHNode::boundingBox(double /*t0*/, double /*t1*/) const
{
    return std::make_optional<AABB>(m_box);
}

std::optional<HitRecord> BVHNode::hit(const Ray& ray, double minT, double maxT) const
{
    if (!m_box.hit(ray, minT, maxT))
    {
        return std::nullopt;
    }

    std::optional<HitRecord> optLeftHitRecord = m_pLeft->hit(ray, minT, maxT);
    double newMaxT = optLeftHitRecord.has_value() ? optLeftHitRecord.value().rayT() : maxT;
    std::optional<HitRecord> optRightHitRecord = m_pRight->hit(ray, minT, newMaxT);
    return optRightHitRecord;
}

UV BVHNode::uv(const XYZ& point) const
{
    // TODO
    return m_pLeft->uv(point);
}