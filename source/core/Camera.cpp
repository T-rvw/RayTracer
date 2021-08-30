#include "Camera.h"

Camera::Camera()
{
	constexpr double viewPortHeight = 2.0;
	constexpr double focalLength = 1.0;
	double viewPortWidth = aspectRatio() * viewPortHeight;
	
	m_origin = XYZ(0.0, 0.0, 0.0);
	m_horizontal = XYZ(viewPortWidth, 0.0, 0.0);
	m_vertical = XYZ(0.0, viewPortHeight, 0.0);
	m_leftDownCorner = m_origin - m_horizontal * 0.5 - m_vertical * 0.5 - XYZ(0.0, 0.0, focalLength);
}