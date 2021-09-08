#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

// Interface
class HittableList : public Hittable
{
public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> pHittableObject) { appendOne(pHittableObject); }
    virtual ~HittableList() = default;

    HittableList(const HittableList&) = delete;
    HittableList(HittableList&&) = delete;
    HittableList& operator=(const HittableList&) = delete;
    HittableList& operator=(HittableList&&) = delete;

    void clear() { m_vecHittableObjects.clear(); }
    void reserve(size_t expectedSize) { m_vecHittableObjects.reserve(expectedSize); }
    void appendOne(std::shared_ptr<Hittable> pHittableObject) { m_vecHittableObjects.push_back(pHittableObject); }

    // TODO : remove these two virtual methods
    virtual std::shared_ptr<Material> material() const override { return nullptr; }
    virtual UV uv(const XYZ& /*point*/) const override { return UV(0.0, 0.0); }

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;

private:
    std::vector<std::shared_ptr<Hittable>> m_vecHittableObjects;
};