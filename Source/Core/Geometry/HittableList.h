#pragma once

#include "GeometryBase.h"

#include <vector>

class HittableList
{
public:
    HittableList() = default;
    HittableList(GeometryBase* pHittableObject) { add(pHittableObject); }
    virtual ~HittableList() = default;

    HittableList(const HittableList&) = delete;
    HittableList(HittableList&&) = delete;
    HittableList& operator=(const HittableList&) = delete;
    HittableList& operator=(HittableList&&) = delete;

    size_t size() const { return m_vecHittableObjects.size(); }
    std::vector<GeometryBase*>& objects() { return m_vecHittableObjects; }
    GeometryBase* operator[](int index) const { return m_vecHittableObjects[index]; }

    void clear() { m_vecHittableObjects.clear(); }
    void reserve(size_t expectedSize) { m_vecHittableObjects.reserve(expectedSize); }
    GeometryBase& add(GeometryBase* pHittableObject) { m_vecHittableObjects.push_back(pHittableObject); return *pHittableObject; }
    
    std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const;
    std::optional<AABB> boundingBox(double t0, double t1) const;

private:
    std::vector<GeometryBase*> m_vecHittableObjects;
};