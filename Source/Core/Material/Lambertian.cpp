#pragma once

#include "Hittable.h"
#include "Lambertian.h"

bool Lambertian::scatter([[maybe_unused]] const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
{
	XYZ normal = hitRecord.normal();

	XYZ scatterDirection = normal + unit(randomInUnitSphere());
	if (scatterDirection.isZero())
	{
		scatterDirection = hitRecord.normal();
	}
	
	scattered = Ray(hitRecord.hitPoint(), scatterDirection, ray.delayTime());
	UV hitPointUV = hitRecord.hitPointUV();
	attenuation = m_pAlbedo->value(hitPointUV.x(), hitPointUV.y(), hitRecord.hitPoint());
	return true;
}