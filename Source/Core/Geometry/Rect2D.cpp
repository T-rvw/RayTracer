#include "HitRecord.h"
#include "Rect2D.h"

#include <cmath>

Rect2D::Rect2D(double x0, double x1, double y0, double y1, double z, std::shared_ptr<Material> pMaterial) :
    m_x0(x0),
	m_x1(x1),
    m_y0(y0),
    m_y1(y1),
    m_z(z),
    m_pMaterial(pMaterial)
{
}

std::optional<HitRecord> Rect2D::hit(const Ray& ray, double minT, double maxT) const
{
	XYZ origin = ray.origin();
	XYZ direction = ray.direction();

	// ray = origin + direction * t
    // ray_z = origin_z + direction_z * t
	// x0 < ray_x < x1, y0 < ray_y < y1
	// t = (ray_z - origin_z) / direction_z
	// Intersect : m_z == ray_z
	double t = (m_z - origin.z()) / direction.z();
	if(t < minT || t > maxT)
	{
		return std::nullopt;
	}

	// ray_x = origin_x + direction_x * t
	// ray_y = origin_y + direction_y * t
	double x = origin.x() + direction.x() * t;
	double y = origin.y() + direction.y() * t;
	if(x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1)
	{
		return std::nullopt;
	}

    // Return final hit result.
    // XYZ hitPoint = ray.at(t);
    XYZ hitPoint(x, y, m_z);
    XYZ outwardNormal(0.0, 0.0, 1.0);
    bool isFront = dot(ray.direction(), outwardNormal) < DOUBLE_EPS;
    return HitRecord(std::move(hitPoint), isFront ? std::move(outwardNormal) : std::move(outwardNormal.inverse()), t, isFront, this);
}

UV Rect2D::uv(const XYZ& point) const
{
	// u / (point.x() - m_x0) = 1.0 / (m_x1 - m_x0)
	// v / (point.y() - m_y0) = 1.0 / (m_y1 - m_y0)
	double u = (point.x() - m_x0) / (m_x1 - m_x0);
	double v = (point.y() - m_y0) / (m_y1 - m_y0);
    return UV(u, v);
}