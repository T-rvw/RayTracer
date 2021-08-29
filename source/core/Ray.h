#pragma once

#include "XYZ.h"

class Ray final
{
public:
	Ray() = default;
	Ray(const XYZ& origin, const XYZ& direction) : m_origin(origin), m_direction(direction) {}
	Ray(const Ray&) = default;
	Ray(Ray&&) = default;
	Ray& operator=(const Ray&) = default;
	Ray& operator=(Ray&&) = default;
	~Ray() = default;
	
	XYZ origin() const { return m_origin; }
	XYZ direction() const { return m_direction; }
	
	XYZ at(double t) const { return m_origin + t * m_direction; }
	
private:
	XYZ m_origin;
	XYZ m_direction;
};