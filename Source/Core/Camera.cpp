#include "Camera.h"

Camera::Camera(double verticalFov, double aspectRatio) :
	m_aspectRatio(aspectRatio)
{
	double theta = MathUtils::degrees2Radians(verticalFov);
	double h = tan(theta * 0.5);
	double viewPortHeight = 2.0 * h;
	constexpr double focalLength = 1.0;
	double viewPortWidth = aspectRatio * viewPortHeight;
	
	m_origin = XYZ(0.0, 0.0, 0.0);
	m_horizontal = XYZ(viewPortWidth, 0.0, 0.0);
	m_vertical = XYZ(0.0, viewPortHeight, 0.0);
	m_leftDownCorner = m_origin - m_horizontal * 0.5 - m_vertical * 0.5 - XYZ(0.0, 0.0, focalLength);
}

Camera::Camera(XYZ lookFrom, XYZ lookAt, XYZ viewportUp, double verticalFov, double aspectRatio) :
	m_aspectRatio(aspectRatio)
{
	double theta = MathUtils::degrees2Radians(verticalFov);
	double viewPortHeight = 2.0 * tan(theta * 0.5);
	double viewPortWidth = aspectRatio * viewPortHeight;

	XYZ w = unit(lookFrom - lookAt);
	XYZ u = unit(cross(viewportUp, w));
	XYZ v = cross(w, u);

	m_origin = lookFrom;
	m_horizontal = viewPortWidth * u;
	m_vertical = viewPortHeight * v;
	m_leftDownCorner = m_origin - (m_vertical + m_horizontal) * 0.5 - w;
}

Camera::Camera(XYZ lookFrom, XYZ lookAt, XYZ viewportUp, double aperture, double focusDist, double verticalFov, double aspectRatio) :
	m_aspectRatio(aspectRatio)
{
	double theta = MathUtils::degrees2Radians(verticalFov);
	double viewPortHeight = 2.0 * tan(theta * 0.5);
	double viewPortWidth = aspectRatio * viewPortHeight;

	m_w = unit(lookFrom - lookAt);
	m_u = unit(cross(viewportUp, m_w));
	m_v = cross(m_w, m_u);

	m_origin = lookFrom;
	m_horizontal = focusDist * viewPortWidth * m_u;
	m_vertical = focusDist * viewPortHeight * m_v;
	m_leftDownCorner = m_origin - (m_vertical + m_horizontal) * 0.5 - focusDist * m_w;
	m_lensRadius = aperture * 0.5;
}

Ray Camera::getRay(double u, double v) const
{
	XYZ rd = m_lensRadius * randomInUnitDisk();
	XYZ offset = m_u * rd.x() + m_v * rd.y();
	XYZ rayDirection = m_leftDownCorner + u * m_horizontal + v * m_vertical - m_origin - offset;
	double rayDelayTime = MathUtils::randomDouble(m_shutterMinTime, m_shutterMaxTime);
	return Ray(m_origin + offset, rayDirection, rayDelayTime);
}