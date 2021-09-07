#include "ExampleBase.h"

#include <iostream>

ExampleBase::ExampleBase(int imageWidth, int imageHeight) :
    m_imageWidth(imageWidth),
    m_imageHeight(imageHeight),
    m_pixelNumber(imageWidth * imageHeight),
    m_imageExporter(imageWidth, imageHeight)
{
}

void ExampleBase::generate(const char* fileName)
{
    if (m_imageExporter.generate(fileName))
    {
        std::cout << "Succeed to generate image." << std::endl;
    }
    else
    {
        std::cout << "Failed to generate image." << std::endl;
    }
}

void ExampleBase::process(const Camera& camera, const HittableList& world)
{
    int curPixelCount = 0;
#pragma omp parallel for
    for (int jj = m_imageHeight - 1; jj >= 0; --jj)
    {
        for (int ii = 0; ii < m_imageWidth; ++ii)
        {
            Color pixelColor(0.0, 0.0, 0.0);

            for (int sampleTimes = 0; sampleTimes < m_sampleTimes; ++sampleTimes)
            {
                double u = static_cast<double>(ii + randomDouble()) / (m_imageWidth - 1);
                double v = static_cast<double>(jj + randomDouble()) / (m_imageHeight - 1);
                Ray ray = camera.getRay(u, v);
                pixelColor += getRayColor(ray, world, m_maxRecursiveDepth);
            }

            // sample && gamma-correct(1/2).
            {
                double sampleScale = 1.0 / m_sampleTimes;
                pixelColor[0] = clamp(pow(pixelColor.x() * sampleScale, 0.5), 0.0, 1.0);
                pixelColor[1] = clamp(pow(pixelColor.y() * sampleScale, 0.5), 0.0, 1.0);
                pixelColor[2] = clamp(pow(pixelColor.z() * sampleScale, 0.5), 0.0, 1.0);
            }

            // (imageHeight - 1 - jj) * imageWidth + ii;
            size_t pixelIndex = m_pixelNumber - (jj + 1) * m_imageWidth + ii;
            m_imageExporter.fillColor(pixelIndex, pixelColor);

            std::cout << std::format("Fill color pixel placed at {}, progress = {}/{}\n", pixelIndex, ++curPixelCount, m_pixelNumber);
        }
    }
}