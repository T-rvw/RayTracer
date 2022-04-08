#include "ImageExporter.h"

#include <iostream>

int main()
{
    constexpr int imageWidth = 256;
    constexpr int imageHeight = 256;
    constexpr int pixelNumber = imageHeight * imageWidth;
    FrameBuffer frameBuffer(imageWidth, imageHeight);

    int curPixelCount = 0;
#pragma omp parallel for
    for (int ii = 0; ii < imageWidth; ++ii)
    {
        for (int jj = 0; jj < imageHeight; ++jj)
        {
            XYZ color(static_cast<double>(ii) / (imageWidth - 1),
                static_cast<double>(jj) / (imageHeight - 1),
                0.25);

            int pixelIndex = frameBuffer.fill(ii, jj, color);

            printf("Fill color pixel placed at %d, progress = %d/%d\n", static_cast<int>(pixelIndex), ++curPixelCount, pixelNumber);
        }
    }
    

    if (ImageExporter::generate(frameBuffer, "test.png"))
    {
        std::cout << "Succeed to generate image." << std::endl;
    }
    else
    {
        std::cout << "Failed to generate image." << std::endl;
    }

    return 0;
}