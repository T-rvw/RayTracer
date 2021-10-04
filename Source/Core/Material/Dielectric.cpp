#include "GeometryBase.h"
#include "Dielectric.h"

bool Dielectric::scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
{
	attenuation = Color(1.0, 1.0, 1.0);
	double refractionRatio = hitRecord.isFront() ? 1.0 / m_refractionIndex : m_refractionIndex;

	XYZ unitDirection = ray.direction();
	unitDirection.normalize();

	XYZ normal = hitRecord.normal();
	double cosTheta = fmin(XYZ::dot(unitDirection, normal), 1.0);
	double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

	constexpr bool bUseReflectance = false;
	XYZ direction;
	if (refractionRatio * sinTheta > 1.0 ||
		(bUseReflectance && MathUtils::reflectance(cosTheta, refractionRatio) > MathUtils::randomDouble()))
	{
		direction = reflect(unitDirection, normal);
	}
	else
	{
		direction = refract(unitDirection, normal, refractionRatio);
	}

	scattered = Ray(hitRecord.hitPoint(), direction, ray.delayTime());

	return true;
}
