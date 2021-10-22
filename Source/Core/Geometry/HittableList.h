#pragma once

#include "GeometryBase.h"

#include <vector>

class HittableList final
{
public:
    HittableList() = default;
    explicit HittableList(std::shared_ptr<GeometryBase> pHittableObject) { add(pHittableObject); }
    virtual ~HittableList() = default;

    HittableList(const HittableList&) = delete;
    HittableList(HittableList&&) = delete;
    HittableList& operator=(const HittableList&) = delete;
    HittableList& operator=(HittableList&&) = delete;

    size_t size() const { return m_vecHittableObjects.size(); }
    std::vector<std::shared_ptr<GeometryBase>>& objects() { return m_vecHittableObjects; }
    std::shared_ptr<GeometryBase> operator[](int index) const { return m_vecHittableObjects[index]; }

    void clear() { m_vecHittableObjects.clear(); }
    void reserve(size_t expectedSize) { m_vecHittableObjects.reserve(expectedSize); }
    GeometryBase& add(std::shared_ptr<GeometryBase> pHittableObject) { m_vecHittableObjects.push_back(pHittableObject); return *(pHittableObject.get()); }
    
    std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const;
    std::optional<AABB> boundingBox(double t0, double t1) const;

private:
    std::vector<std::shared_ptr<GeometryBase>> m_vecHittableObjects;
};