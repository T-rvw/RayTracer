#pragma once

#include "Hittable.h"
#include "Dielectric.h"

bool Dielectric::scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
{
	attenuation = Color(1.0, 1.0, 1.0);
	double refractionRatio = hitRecord.isFront() ? 1.0 / m_refractionIndex : m_refractionIndex;

	XYZ unitDirection = unit(ray.direction());
	XYZ normal = hitRecord.normal();
	double cosTheta = fmin(dot(unitDirection, normal), 1.0);
	double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	XYZ direction;
	if (refractionRatio * sinTheta > 1.0 ||
		reflectance(cosTheta, refractionRatio) > randomDouble())
	{
		direction = reflect(unitDirection, normal);
	}
	else
	{
		direction = refract(unitDirection, normal, refractionRatio);
	}

	scattered = Ray(hitRecord.hitPoint(), direction);

	return true;
}