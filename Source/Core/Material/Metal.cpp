#pragma once

#include "Hittable.h"
#include "Metal.h"

bool Metal::scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
{
	XYZ normal = hitRecord.normal();
	XYZ reflected = reflect(unit(ray.direction()), normal);
	scattered = Ray(hitRecord.hitPoint(), reflected);
	attenuation = m_albedo;
	return true;
}