#include "GeometryBase.h"
#include "Metal.h"

bool Metal::scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
{
	XYZ normal = hitRecord.normal();
	XYZ direction = ray.direction();
	XYZ reflected = reflect(direction.normalize(), normal);
	scattered = Ray(hitRecord.hitPoint(), reflected + m_fuzz * randomInUnitSphere().normalize(), ray.delayTime());
	attenuation = m_albedo;
	return true;
}
