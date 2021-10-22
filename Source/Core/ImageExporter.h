// Portable PixelMap file format exporter
#pragma once

#include "XYZ.h"

#include <cstdint>
#include <filesystem>

class ImageExporter final
{
public:	
	ImageExporter() = delete;
	explicit ImageExporter(int width, int height, int component = 3);
	~ImageExporter();
	
	ImageExporter(const ImageExporter&) = delete;
	ImageExporter(ImageExporter&&) = delete;
	ImageExporter& operator=(const ImageExporter&) = delete;
	ImageExporter& operator=(ImageExporter&&) = delete;
	
	void fillColor(size_t index, const XYZ& color);

	bool generate(std::filesystem::path filePath, bool bOverwrite = false);
	
private:
	int				m_imageWidth;
	int				m_imageHeight;
	int				m_component;
	unsigned char*	m_imageData;
};