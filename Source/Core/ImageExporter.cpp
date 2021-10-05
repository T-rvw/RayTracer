#include "ImageExporter.h"

#include <ctime>
#include <fstream>
#include <string>

#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

ImageExporter::ImageExporter(int width, int height, int component) :
m_imageWidth(width),
m_imageHeight(height),
m_component(component)
{
	// RGB
	m_imageData = new unsigned char[m_imageWidth * m_imageHeight * 3];
}

ImageExporter::~ImageExporter()
{
	delete [] m_imageData;
	m_imageData = nullptr;
}

void ImageExporter::fillColor(size_t index, const XYZ& color)
{
	uint8_t r = static_cast<uint8_t>(255.999 * color.x());
	uint8_t g = static_cast<uint8_t>(255.999 * color.y());
	uint8_t b = static_cast<uint8_t>(255.999 * color.z());

	size_t rgbIndex = index * 3;

	m_imageData[rgbIndex] = r;
	m_imageData[rgbIndex + 1] = g;
	m_imageData[rgbIndex + 2] = b;
}

bool ImageExporter::generate(std::filesystem::path filePath, bool bOverwrite)
{
	// If we don't want to overwrite image file with the same name, we can
	// append time stamp in the end of file name.
	if (std::filesystem::exists(filePath) && !bOverwrite)
	{
		time_t curTimeStamp = ::time(nullptr);
		filePath = filePath.stem().string() + "_" + std::to_string(curTimeStamp) + filePath.extension().string();
	}

	auto extension = filePath.extension();
	if (0 == extension.compare(".bmp") || 0 == extension.compare(".BMP"))
	{
		stbi_write_bmp(filePath.string().c_str(), m_imageWidth, m_imageHeight, m_component, m_imageData);
	}
	else if (0 == extension.compare(".png") || 0 == extension.compare(".PNG"))
	{
		stbi_write_png(filePath.string().c_str(), m_imageWidth, m_imageHeight, m_component, m_imageData, 0);
	}
	else if (0 == extension.compare(".jpg") || 0 == extension.compare(".JPG"))
	{
		stbi_write_jpg(filePath.string().c_str(), m_imageWidth, m_imageHeight, m_component, m_imageData, 0);
	}
	else
	{
		printf("Not supported file format. Please add it by yourself.");
		return false;
	}

	return true;
}