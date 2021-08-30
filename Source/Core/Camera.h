#pragma once

#include "Ray.h"

class Camera final
{
public:
	Camera();
	~Camera() = default;

	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) = delete;
	
	constexpr double aspectRatio() const { return 16.0 / 9.0; }

	Ray getRay(double u, double v) const
	{ 
		return Ray(m_origin, m_leftDownCorner + u * m_horizontal + v * m_vertical - m_origin);
	}

private:
	XYZ m_origin;
	XYZ m_leftDownCorner;
	XYZ m_horizontal;
	XYZ m_vertical;
};