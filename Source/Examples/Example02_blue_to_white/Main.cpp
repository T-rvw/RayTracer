#include "ImageExporter.h"
#include "Ray.h"

#include <iostream>

Color getRayColor(const Ray& ray, const Color& srcColor, const Color& dstColor)
{
	XYZ unitDir = ray.direction();
    unitDir.normalize();
	double t = 0.5 * (unitDir.y() + 1.0);
	return (1.0 - t) * srcColor + t * dstColor;
}

int main()
{
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr int pixelNumber = imageHeight * imageWidth;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    FrameBuffer<PixelFormat::RGBA> frameBuffer(imageWidth, imageHeight);
    Color source(1.0, 1.0, 1.0);
    Color destination(0.5, 0.7, 1.0);

	// Camera
	double viewPortHeight = 2.0;
	double viewPortWidth = aspectRatio * viewPortHeight;
	double focalLength = 1.0;
	
	XYZ origin(0.0, 0.0, 0.0);
	XYZ horizontal(viewPortWidth, 0.0, 0.0);
	XYZ vertical(0.0, viewPortHeight, 0.0);
	XYZ leftDownCorner = origin - horizontal * 0.5 - vertical * 0.5 - XYZ(0.0, 0.0, focalLength);

	// Render
    int curPixelCount = 0;
#pragma omp parallel for
    for (int jj = imageHeight - 1; jj >= 0; --jj)
    {
        for (int ii = 0; ii < imageWidth; ++ii)
        {
			double u = static_cast<double>(ii) / (imageWidth - 1);
			double v = static_cast<double>(jj) / (imageHeight - 1);
            
			Ray ray(origin, leftDownCorner + u * horizontal + v * vertical - origin);

            // (imageHeight - 1 - jj) * imageWidth + ii;
            int pixelIndex = pixelNumber - (jj + 1) * imageWidth + ii;
            frameBuffer.fill(pixelIndex, getRayColor(ray, source, destination));

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