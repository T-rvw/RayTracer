#include "FrameBuffer.h"

constexpr int component = 3;

FrameBuffer::FrameBuffer(int width, int height)
	: m_width(width),
	m_height(height)
{
	m_pData = new uint8_t[width * height * component];
}

FrameBuffer::~FrameBuffer()
{
	delete[] m_pData;
}

void FrameBuffer::fill(int index, const XYZ& value)
{
	uint8_t r = static_cast<uint8_t>(255.999 * value.x());
	uint8_t g = static_cast<uint8_t>(255.999 * value.y());
	uint8_t b = static_cast<uint8_t>(255.999 * value.z());

	int rgbIndex = index * component;

	m_pData[rgbIndex] = r;
	m_pData[rgbIndex + 1] = g;
	m_pData[rgbIndex + 2] = b;
}

int FrameBuffer::fill(int x, int y, const XYZ& value)
{
	uint8_t r = static_cast<uint8_t>(255.999 * value.x());
	uint8_t g = static_cast<uint8_t>(255.999 * value.y());
	uint8_t b = static_cast<uint8_t>(255.999 * value.z());

	int rgbIndex = (y * m_width + x) * component;

	m_pData[rgbIndex] = r;
	m_pData[rgbIndex + 1] = g;
	m_pData[rgbIndex + 2] = b;

	return rgbIndex;
}