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

    virtual Color emitted(double /*u*/, double /*v*/, const XYZ& /*p*/) const
	{
		return Color(0.0, 0.0, 0.0);
	}

    virtual bool scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const = 0;
};