// Portable PixelMap file format exporter
#pragma once

#include "XYZ.h"

#include <cstdint>
#include <filesystem>

enum class PPMResult
{
	Failure,
	PartialSuccess,
	TotalSuccess,
};

enum class PPMEncode
{
	ASCII, // p3
};

class PPMExporter final
{
public:	
	PPMExporter() = delete;
	PPMExporter(uint16_t width, uint16_t height, uint8_t maxColor = 255, PPMEncode encode = PPMEncode::ASCII);
	~PPMExporter() = default;
	
	PPMExporter(const PPMExporter&) = delete;
	PPMExporter(PPMExporter&&) = delete;
	PPMExporter& operator=(const PPMExporter&) = delete;
	PPMExporter& operator=(PPMExporter&&) = delete;
	
	void fillColor(size_t index, uint8_t r, uint8_t g, uint8_t b);
	void fillColor(size_t index, const XYZ& color);
	PPMResult generate(std::filesystem::path filePath, bool bOverwrite = false);
	
private:
	PPMEncode				m_encodeType;
	uint8_t					m_maxColor;
	uint16_t				m_pixelMapWidth;
	uint16_t				m_pixelMapHeight;
	std::vector<uint8_t>	m_vecPixelMap;

	uint32_t				m_errorPixelCount;
};