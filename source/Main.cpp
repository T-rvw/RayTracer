#include "PPMExporter.h"

#include <iostream>

int main()
{
    constexpr uint16_t imageWidth = 256;
    constexpr uint16_t imageHeight = 256;
    PPMExporter ppmExporter(imageWidth, imageHeight);
    for (int ii = 0; ii < imageWidth; ++ii)
    {
        for (int jj = 0; jj < imageHeight; ++jj)
        {
            uint8_t r = static_cast<uint8_t>(ii);
            uint8_t g = static_cast<uint8_t>(jj);
            uint8_t b = 0;

            ppmExporter.fillColor(ii * imageHeight + jj, r, g, b);
        }
    }

    size_t pixelIndex = 0;
    for (int jj = imageHeight - 1; jj >= 0; --jj)
    {
        for (int ii = 0; ii < imageWidth; ++ii)
        {
            double r = static_cast<double>(ii) / (imageWidth - 1);
            double g = static_cast<double>(jj) / (imageHeight - 1);
            double b = 0.25;

            uint8_t ir = static_cast<uint8_t>(255.999 * r);
            uint8_t ig = static_cast<uint8_t>(255.999 * g);
            uint8_t ib = static_cast<uint8_t>(255.999 * b);

            
            ppmExporter.fillColor(pixelIndex, ir, ig, ib);
            std::cout << "Fill color pixel placed at " << pixelIndex << std::endl;
            ++pixelIndex;
        }
    }

    PPMResult result = ppmExporter.generate("test.ppm");
    if (PPMResult::TotalSuccess == result)
    {
        std::cout << "Succeed to generate ppm totally." << std::endl;
    }
    else if (PPMResult::PartialSuccess == result)
    {
        std::cout << "Succeed to generate ppm partially." << std::endl;
    }
    else if (PPMResult::Failure == result)
    {
        std::cout << "Failed to generate ppm." << std::endl;
    }

    return 0;
}