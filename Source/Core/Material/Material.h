#pragma once

#include "Ray.h"

class HitRecord;

// Interface
class Material
{
public:
    Material() = default;
    virtual ~Material() = default;

    Material(const Material&) = delete;
    Material(Material&&) = delete;
    Material& operator=(const Material&) = delete;
    Material& operator=(Material&&) = delete;

    virtual bool scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const = 0;
};