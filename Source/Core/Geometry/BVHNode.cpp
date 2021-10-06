#include "BVHNode.h"

namespace
{
    bool compareBox(const GeometryBase* lhs, const GeometryBase* rhs, int axis)
    {
        std::optional<AABB> optLhsAABB = lhs->boundingBox(0.0, 0.0);
        std::optional<AABB> optRhsAABB = rhs->boundingBox(0.0, 0.0);
        if (!optLhsAABB.has_value() || !optRhsAABB.has_value())
        {
            return false;
        }

        return optLhsAABB.value().min()[axis] < optRhsAABB.value().min()[axis];
    }
}

BVHNode::BVHNode(HittableList& hitList, double t0, double t1)
    : BVHNode(hitList.objects(), 0, hitList.size(), t0, t1)
{
}

BVHNode::BVHNode(std::vector<GeometryBase*>& vecHitObjects, size_t start, size_t end, double t0, double t1)
{
    int axis = MathUtils::randomInt(0, 2);
    auto comparator = std::bind(compareBox, std::placeholders::_1, std::placeholders::_2, axis);
    size_t objectSpan = end - start;

    if (1 == objectSpan)
    {
        m_pLeft = vecHitObjects[start];
        m_pRight = vecHitObjects[start];
    }
    else if (2 == objectSpan)
    {
        if (comparator(vecHitObjects[start], vecHitObjects[start + 1]))
        {
            m_pLeft = vecHitObjects[start];
            m_pRight = vecHitObjects[start + 1];
        }
        else
        {
            m_pLeft = vecHitObjects[start + 1];
            m_pRight = vecHitObjects[start];
        }
    }
    else
    {
        std::sort(vecHitObjects.begin(), vecHitObjects.end(), comparator);

        size_t mid = start + static_cast<size_t>(objectSpan * 0.5);
        m_pLeft = new BVHNode(vecHitObjects, start, mid, t0, t1);
        m_pRight = new BVHNode(vecHitObjects, mid, end, t0, t1);
    }

    std::optional<AABB> optLeftBox = m_pLeft->boundingBox(t0, t1);
    std::optional<AABB> optRightBox = m_pRight->boundingBox(t0, t1);
    if (optLeftBox.has_value() && optRightBox.has_value())
    {
        m_box = AABB::merge(optLeftBox.value(), optRightBox.value());
    }
}

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
    return optLeftHitRecord.has_value() ? optLeftHitRecord : m_pRight->hit(ray, minT, maxT);
}

UV BVHNode::uv(const XYZ& point) const
{
    // TODO
    return m_pLeft->uv(point);
}