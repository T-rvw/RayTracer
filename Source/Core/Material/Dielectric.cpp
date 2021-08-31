#pragma once

#include "Hittable.h"
#include "Dielectric.h"

bool Dielectric::scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
{
	attenuation = Color(1.0, 1.0, 1.0);
	double refractionRatio = hitRecord.isFront() ? 1.0 / m_refractionIndex : m_refractionIndex;

	XYZ unitDirection = unit(ray.direction());
	XYZ refracted = refract(unitDirection, hitRecord.normal(), refractionRatio);
	scattered = Ray(hitRecord.hitPoint(), refracted);

	return true;
}