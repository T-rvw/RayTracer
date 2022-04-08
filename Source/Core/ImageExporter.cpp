#include "ImageExporter.h"

#include <ctime>
#include <fstream>
#include <string>

#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

bool ImageExporter::generate(const FrameBuffer<PixelFormat::RGBA>& buffer, std::filesystem::path filePath, bool bOverwrite)
{
	// If we don't want to overwrite image file with the same name, we can
	// append time stamp in the end of file name.
	if (std::filesystem::exists(filePath) && !bOverwrite)
	{
		time_t curTimeStamp = ::time(nullptr);
		filePath = filePath.stem().string() + "_" + std::to_string(curTimeStamp) + filePath.extension().string();
	}

	int imageWidth = buffer.getWidth();
	int imageHeight = buffer.getHeight();
	const uint8_t* imageData = buffer.getData();
	constexpr int dataComponent = 4;
	
	auto extension = filePath.extension();
	if (0 == extension.compare(".bmp") || 0 == extension.compare(".BMP"))
	{
		stbi_write_bmp(filePath.string().c_str(), imageWidth, imageHeight, dataComponent, imageData);
	}
	else if (0 == extension.compare(".png") || 0 == extension.compare(".PNG"))
	{
		stbi_write_png(filePath.string().c_str(), imageWidth, imageHeight, dataComponent, imageData, 0);
	}
	else if (0 == extension.compare(".jpg") || 0 == extension.compare(".JPG"))
	{
		stbi_write_jpg(filePath.string().c_str(), imageWidth, imageHeight, dataComponent, imageData, 0);
	}
	else
	{
		printf("Not supported file format. Please add it by yourself.");
		return false;
	}

	return true;
}