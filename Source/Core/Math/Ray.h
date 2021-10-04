#pragma once

#include "XYZ.h"

class Ray final
{
public:
	Ray() = default;
	Ray(const XYZ& origin, const XYZ& direction) : m_origin(origin), m_direction(direction) {}
	Ray(const XYZ& origin, const XYZ& direction, double delayTime) : m_origin(origin), m_direction(direction), m_delayTime(delayTime) {}
	~Ray() = default;

	Ray(const Ray&) = default;
	Ray(Ray&&) = default;
	Ray& operator=(const Ray&) = default;
	Ray& operator=(Ray&&) = default;

	const XYZ& origin() const { return m_origin; }
	const XYZ& direction() const { return m_direction; }
	double delayTime() const { return m_delayTime; }

	XYZ at(double t) const { return m_origin + t * m_direction; }

private:
	XYZ		m_origin;
	XYZ		m_direction;
	double	m_delayTime = 0.0;
};