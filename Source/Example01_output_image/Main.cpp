#include "PPMExporter.h"

#include <iostream>

int main()
{
    constexpr uint16_t imageWidth = 256;
    constexpr uint16_t imageHeight = 256;
    PPMExporter ppmExporter(imageWidth, imageHeight);

    size_t pixelNumber = imageHeight * imageWidth;
#pragma omp parallel for
    for (int jj = imageHeight - 1; jj >= 0; --jj)
    {
        for (int ii = 0; ii < imageWidth; ++ii)
        {
            XYZ color(static_cast<double>(ii) / (imageWidth - 1),
                      static_cast<double>(jj) / (imageHeight - 1),
                      0.25);

            // (imageHeight - 1 - jj) * imageWidth + ii;
            size_t pixelIndex = pixelNumber - (jj + 1) * imageWidth + ii;
            ppmExporter.fillColor(pixelIndex, color);
            std::cout << "Fill color pixel placed at " << pixelIndex << std::endl;
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