#include "PPMExporter.h"
#include <ctime>
#include <fstream>

namespace
{
	std::string getEncodeTypeData(PPMEncode encodeType)
	{
		if (PPMEncode::ASCII == encodeType)
		{
			return "P3";
		}

		// no other encode types now
		return "P3";
	}
}

PPMExporter::PPMExporter(uint16_t width, uint16_t height, uint8_t maxColor, PPMEncode encode) :
m_pixelMapWidth(width),
m_pixelMapHeight(height),
m_maxColor(maxColor),
m_encodeType(encode),
m_errorPixelCount(0)
{
	// RGB
	m_vecPixelMap.resize(m_pixelMapWidth * m_pixelMapHeight * 3);
}

void PPMExporter::fillColor(size_t index, uint8_t r, uint8_t g, uint8_t b)
{
	// We can do some sanity checks before using data
	if (index < 0 || index >= m_pixelMapWidth * m_pixelMapHeight)
	{
		// Out of boundary
		++m_errorPixelCount;
	}

	size_t rgbIndex = index * 3;

	m_vecPixelMap[rgbIndex]		= r;
	m_vecPixelMap[rgbIndex + 1]	= g;
	m_vecPixelMap[rgbIndex + 2]	= b;
}

PPMResult PPMExporter::generate(std::filesystem::path filePath, bool bOverwrite)
{
	if (filePath.extension().compare("ppm") == std::string::npos ||
		filePath.extension().compare("PPM") == std::string::npos)
	{
		return PPMResult::Failure;
	}

	if (std::filesystem::exists(filePath) && !bOverwrite)
	{
		time_t curTimeStamp = ::time(nullptr);
		filePath = std::format("{}_{}.ppm", filePath.stem().string(), curTimeStamp);
	}
	
	std::ofstream ppmFile;
	ppmFile.open(filePath);
	ppmFile << getEncodeTypeData(m_encodeType) << "\n" << m_pixelMapWidth << " " << m_pixelMapHeight << "\n" << static_cast<int>(m_maxColor) << "\n";
	for (size_t ii = 0, pixelNumber = m_vecPixelMap.size(); ii < pixelNumber; ii += 3)
	{
		int r = static_cast<int>(m_vecPixelMap[ii]);
		int g = static_cast<int>(m_vecPixelMap[ii + 1]);
		int b = static_cast<int>(m_vecPixelMap[ii + 2]);
		ppmFile << r << " " << g << " " << b << " \n";
	}
	ppmFile.close();

	if (m_errorPixelCount > 0)
	{
		return PPMResult::PartialSuccess;
	}

	return PPMResult::TotalSuccess;
}