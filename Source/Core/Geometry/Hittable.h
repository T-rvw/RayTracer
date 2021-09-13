#pragma once

#include "HitRecord.h"
#include "Ray.h"
#include "XY.h"

#include <memory>
#include <optional>

class Material;

class Hittable
{
public:
    Hittable() = default;
    virtual ~Hittable() = default;

    Hittable(const Hittable&) = delete;
    Hittable(Hittable&&) = delete;
    Hittable& operator=(const Hittable&) = delete;
    Hittable& operator=(Hittable&&) = delete;

    virtual std::shared_ptr<Material> material() const { return nullptr; }
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const = 0;
    virtual UV uv(const XYZ& /*point*/) const { return UV(0.0, 0.0); }
};