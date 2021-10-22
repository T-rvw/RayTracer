#pragma once

#include "Ray.h"

class Camera final
{
public:
	explicit Camera(double verticalFov = 90.0, double aspectRatio = 16.0 / 9.0);
	explicit Camera(XYZ lookFrom, XYZ lookAt, XYZ viewportUp, double verticalFov = 90.0, double aspectRatio = 16.0 / 9.0);
	explicit Camera(XYZ lookFrom, XYZ lookAt, XYZ viewportUp, double aperture, double focusDist, double verticalFov, double aspectRatio);
	~Camera() = default;

	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) = delete;
	
	double aspectRatio() const { return m_aspectRatio; }
	void setShutterTime(double minTime, double maxTime) { m_shutterMinTime = minTime; m_shutterMaxTime = maxTime; }

	Ray getRay(double u, double v) const;

private:
	XYZ		m_origin;
	XYZ		m_leftDownCorner;
	XYZ		m_horizontal;
	XYZ		m_vertical;
	double	m_aspectRatio;
	XYZ		m_u, m_v, m_w;
	double	m_lensRadius = 0.0;
	double	m_shutterMinTime = 0.0;
	double	m_shutterMaxTime = 0.0;
};