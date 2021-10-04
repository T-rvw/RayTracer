#pragma once

#include "HitRecord.h"

class AABB
{
public:
    AABB() = default;
    AABB(XYZ p0, XYZ p1);
    virtual ~AABB() = default;

    AABB(const AABB& rhs) = delete;
    AABB(AABB&& rhs) = default;
    AABB& operator=(const AABB&) = default;
    AABB& operator=(AABB&&) = default;

    bool hit(const Ray& ray, double minT, double maxT) const;
    static AABB merge(const AABB& lhs, const AABB& rhs);

    XYZ& minForWrite() { return m_min; }
    XYZ& maxForWrite() { return m_max; }

    const XYZ& min() const { return m_min; }
    const XYZ& max() const { return m_max; }

private:
    XYZ m_min;
    XYZ m_max;
};