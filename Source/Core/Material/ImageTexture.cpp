#include "ImageTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageTexture::ImageTexture(const char* fileName)
{
    int componentsPerPixel = 3; // R-G-B
    m_imageData = stbi_load(fileName, &m_imageWidth, &m_imageHeight, &componentsPerPixel, componentsPerPixel);
}

Color ImageTexture::value(double u, double v, const XYZ& /*p*/) const
{
    if (nullptr == m_imageData)
    {
        return Color(0.0, 0.0, 1.0);
    }
    
    // UV begins from left-bottom corner but image coordinates begins from left-top corner.
    // So it is necessary to filp v to (1 - v)
    u = std::clamp(u, 0.0, 1.0);
    v = 1.0 - std::clamp(v, 0.0, 1.0);

    int ii = static_cast<int>(u * (static_cast<double>(m_imageWidth) - 0.0001));
    int jj = static_cast<int>(v * (static_cast<double>(m_imageHeight) - 0.0001));

    int startIndex = (jj * m_imageWidth + ii) * 3;

    constexpr double factor = 1.0 / 255.0;
    double r = factor * m_imageData[startIndex];
    double g = factor * m_imageData[startIndex + 1];
    double b = factor * m_imageData[startIndex + 2];

    return Color(r, g, b);
}