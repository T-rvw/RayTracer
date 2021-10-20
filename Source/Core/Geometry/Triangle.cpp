#include "HitRecord.h"
#include "Triangle.h"

Triangle::Triangle(XYZ p0, XYZ p1, XYZ p2)
	: m_p0(std::move(p0)),
	m_p1(std::move(p1)),
	m_p2(std::move(p2))
{
	m_normal = XYZ::cross(m_p2 - m_p0, m_p1 - m_p0);
	m_normal.normalize();
	m_distance = -XYZ::dot(m_normal, m_p0);
}

std::optional<HitRecord> Triangle::hit(const Ray& ray, double minT, double maxT) const
{
	const XYZ& origin = ray.origin();
	const XYZ& direction = ray.direction();

	double t = -(XYZ::dot(m_normal, origin) + m_distance) / XYZ::dot(m_normal, direction);
	if (t < minT || t > maxT)
	{
		return std::nullopt;
	}

	// Check if the point is inside the triangle
	XYZ pointInPlane = origin + direction * t;

	XYZ pp0 = pointInPlane - m_p0;
	XYZ p1p0 = m_p1 - m_p0;
	XYZ p2p0 = m_p2 - m_p0;

	double dotP1Sqr = XYZ::dot(p1p0, p1p0);
	double dotP2Sqr = XYZ::dot(p2p0, p2p0);
	double dotP1P2 = XYZ::dot(p1p0, p2p0);
	double factor = 1 / (dotP1Sqr * dotP2Sqr - dotP1P2 * dotP1P2);

	double dotP0P1 = XYZ::dot(pp0, p1p0);
	double dotP0P2 = XYZ::dot(pp0, p2p0);

	// P = w0P0 + w1P1 + w2P2
	double w1 = factor * (dotP2Sqr * dotP0P1 - dotP1P2 * dotP0P2);
	double w2 = factor * (-dotP1P2 * dotP0P1 + dotP1Sqr * dotP0P2);
	double w0 = 1 - w1 - w2;

	// If w0, w1, w2 are all nonnegative, the point is inside the triangle.
	if (w0 > DOUBLE_EPS &&
		w1 > DOUBLE_EPS &&
		w2 > DOUBLE_EPS)
	{
		bool isFront = XYZ::dot(direction, m_normal) < DOUBLE_EPS;
		return HitRecord(std::move(pointInPlane), m_normal, t, isFront, this);
	}

	return std::nullopt;
 }

UV Triangle::uv(const XYZ&) const
{
	return UV(0.0, 0.0);
}

std::optional<AABB> Triangle::boundingBox(double /*t0*/, double /*t1*/) const
{
	XYZ minPoint(std::min({ m_p0.x(), m_p1.x(), m_p2.x() }),
				 std::min({ m_p0.y(), m_p1.y(), m_p2.y() }),
				 std::min({ m_p0.z(), m_p1.z(), m_p2.z() }));

	XYZ maxPoint(std::max({ m_p0.x(), m_p1.x(), m_p2.x() }),
				 std::max({ m_p0.y(), m_p1.y(), m_p2.y() }),
				 std::max({ m_p0.z(), m_p1.z(), m_p2.z() }));

	return AABB(std::move(minPoint), std::move(maxPoint));
}