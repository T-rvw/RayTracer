#include "ImageExporter.h"

#include <iostream>

int main()
{
    constexpr int imageWidth = 256;
    constexpr int imageHeight = 256;
    constexpr int pixelNumber = imageHeight * imageWidth;
    ImageExporter imageExporter(imageWidth, imageHeight);

    int curPixelCount = 0;
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
            imageExporter.fillColor(pixelIndex, color);

            printf("Fill color pixel placed at %d, progress = %d/%d\n", static_cast<int>(pixelIndex), ++curPixelCount, pixelNumber);
        }
    }

    if (imageExporter.generate("test.png"))
    {
        std::cout << "Succeed to generate image." << std::endl;
    }
    else
    {
        std::cout << "Failed to generate image." << std::endl;
    }

    return 0;
}