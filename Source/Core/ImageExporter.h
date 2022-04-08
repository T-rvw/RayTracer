#pragma once

#include "FrameBuffer.h"

#include <cstdint>
#include <filesystem>

class ImageExporter final
{
public:	
	ImageExporter() = delete;
	~ImageExporter() = delete;
	
	ImageExporter(const ImageExporter&) = delete;
	ImageExporter(ImageExporter&&) = delete;
	ImageExporter& operator=(const ImageExporter&) = delete;
	ImageExporter& operator=(ImageExporter&&) = delete;
	
	static bool generate(const FrameBuffer<PixelFormat::RGBA>& buffer, std::filesystem::path filePath, bool bOverwrite = false);
};