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

Camera::Camera(XYZ lookFrom, XYZ lookAt, XYZ viewportUp, double verticalFov, double aspectRatio)
	: m_origin(std::move(lookFrom)),
	m_aspectRatio(aspectRatio)
{
	double theta = MathUtils::degrees2Radians(verticalFov);
	double viewPortHeight = 2.0 * tan(theta * 0.5);
	double viewPortWidth = aspectRatio * viewPortHeight;

	m_w = std::move((m_origin - lookAt).normalize());
	m_u = std::move(XYZ::cross(viewportUp, m_w).normalize());
	m_v = std::move(XYZ::cross(m_w, m_u));

	m_horizontal = viewPortWidth * m_u;
	m_vertical = viewPortHeight * m_v;
	m_leftDownCorner = m_origin - (m_vertical + m_horizontal) * 0.5 - m_w;
}

Camera::Camera(XYZ lookFrom, XYZ lookAt, XYZ viewportUp, double aperture, double focusDist, double verticalFov, double aspectRatio)
	: m_origin(lookFrom),
	m_aspectRatio(aspectRatio),
	m_lensRadius(aperture * 0.5)
{
	double theta = MathUtils::degrees2Radians(verticalFov);
	double viewPortHeight = 2.0 * tan(theta * 0.5);
	double viewPortWidth = aspectRatio * viewPortHeight;

	m_w = std::move((m_origin - lookAt).normalize());
	m_u = std::move(XYZ::cross(viewportUp, m_w).normalize());
	m_v = std::move(XYZ::cross(m_w, m_u));

	m_horizontal = focusDist * viewPortWidth * m_u;
	m_vertical = focusDist * viewPortHeight * m_v;
	m_leftDownCorner = m_origin - (m_vertical + m_horizontal) * 0.5 - focusDist * m_w;
}

Ray Camera::getRay(double u, double v) const
{
	XYZ rd = m_lensRadius * randomInUnitDisk();
	XYZ offset = m_u * rd.x() + m_v * rd.y();
	XYZ rayDirection = m_leftDownCorner + u * m_horizontal + v * m_vertical - m_origin - offset;
	double rayDelayTime = MathUtils::randomDouble(m_shutterMinTime, m_shutterMaxTime);
	return Ray(m_origin + offset, rayDirection, rayDelayTime);
}