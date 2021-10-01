#include "HitRecord.h"
#include "AARect.h"

constexpr char axisIndexArr[] = { 'x', 'y', 'z' };
constexpr int axisIndexArrSize = sizeof(axisIndexArr);

AARect::AARect(XYZ p0, XYZ p1, char kIndex, double k) :
    m_p0(p0),
	m_p1(p1),
    m_k(k)
{
	for (int ii = 0; ii < axisIndexArrSize; ++ii)
	{
		if (kIndex == axisIndexArr[ii])
		{
			m_verticalAxisIndex = ii;
			break;
		}
	}
}

std::optional<HitRecord> AARect::hit(const Ray& ray, double minT, double maxT) const
{
	XYZ origin = ray.origin();
	XYZ direction = ray.direction();

	// ray = origin + direction * t
    // ray_z = origin_z + direction_z * t
	// x0 < ray_x < x1, y0 < ray_y < y1
	// t = (ray_z - origin_z) / direction_z
	// Intersect : m_z == ray_z
	double t = (m_k - origin[m_verticalAxisIndex]) / direction[m_verticalAxisIndex];
	if(t < minT || t > maxT)
	{
		return std::nullopt;
	}

	// ray_x = origin_x + direction_x * t
	// ray_y = origin_y + direction_y * t
	// Check other two axises
	XYZ hitPoint;
	hitPoint[m_verticalAxisIndex] = m_k;

	for (int ii = 0; ii < 3; ++ii)
	{
		if (ii != m_verticalAxisIndex)
		{
			double axisValue = origin[ii] + direction[ii] * t;
			if (axisValue < m_p0[ii] || axisValue > m_p1[ii])
			{
				return std::nullopt;
			}

			hitPoint[ii] = axisValue;
		}
	}

    // Return final hit result.
	// The hitPoint should be equal to ray.at(t)
    XYZ outwardNormal;
	outwardNormal[m_verticalAxisIndex] = 1.0;
    bool isFront = dot(ray.direction(), outwardNormal) < DOUBLE_EPS;
    return HitRecord(std::move(hitPoint), isFront ? std::move(outwardNormal) : std::move(outwardNormal.inverse()), t, isFront, this);
}

UV AARect::uv(const XYZ& point) const
{
	// u / (point.x() - m_x0) = 1.0 / (m_x1 - m_x0)
	// v / (point.y() - m_y0) = 1.0 / (m_y1 - m_y0)
	UV uv;
	int count = 0;
	for (int ii = 0; ii < 3; ++ii)
	{
		if (ii != m_verticalAxisIndex)
		{
			double uvCoordinate = (point[ii] - m_p0[ii]) / (m_p1[ii] - m_p0[ii]);
			uv[count++] = uvCoordinate;
		}
	}

    return uv;
}