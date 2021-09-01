#include "Camera.h"

Camera::Camera(double verticalFov, double aspectRatio) :
	m_aspectRatio(aspectRatio)
{
	double theta = degrees2Radians(verticalFov);
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
	double theta = degrees2Radians(verticalFov);
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