#include "Camera.h"
#include "HittableList.h"
#include "MathUtils.h"
#include "ImageExporter.h"
#include "Ray.h"
#include "Sphere.h"

#include <iostream>
#include <vector>

Color getRayColor(const Ray& ray, const HittableList& world)
{
    std::optional<HitRecord> optHitRecord = world.hit(ray, 0, DOUBLE_INFINITY);
    if (optHitRecord.has_value())
    {
        const HitRecord& hitRecord = optHitRecord.value();
        const XYZ& normal = hitRecord.normal();
        // Map normal's xyz to rgb
        return 0.5 * Color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
    }

    // background
	XYZ unitDir = unit(ray.direction());
	double factor = 0.5 * (unitDir.y() + 1.0);
	return (1.0 - factor) * Color(1.0, 1.0, 1.0) + factor * Color(0.5, 0.7, 1.0);
}

int main()
{
    // ImageExporter
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr int pixelNumber = imageHeight * imageWidth;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    ImageExporter imageExporter(imageWidth, imageHeight);

    // Config
    constexpr int samplesPerPixel = 100;

    // World
    HittableList hittableList;
    hittableList.reserve(2);
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, 0.0, -1.0), 0.5));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, -100.5, -1.0), 100.0));

	// Camera
    Camera camera(90.0, aspectRatio);

	// Render
    int curPixelCount = 0;
#pragma omp parallel for
    for (int jj = imageHeight - 1; jj >= 0; --jj)
    {
        for (int ii = 0; ii < imageWidth; ++ii)
        {
            Color pixelColor(0.0, 0.0, 0.0);

            for (int sampleTimes = 0; sampleTimes < samplesPerPixel; ++sampleTimes)
            {
                double u = static_cast<double>(ii + randomDouble()) / (imageWidth - 1);
                double v = static_cast<double>(jj + randomDouble()) / (imageHeight - 1);
                Ray ray = camera.getRay(u, v);
                pixelColor += getRayColor(ray, hittableList);
            }

            // sample
            {
                double sampleScale = 1.0 / samplesPerPixel;
                pixelColor[0] = clamp(pixelColor.x() * sampleScale, 0.0, 1.0);
                pixelColor[1] = clamp(pixelColor.y() * sampleScale, 0.0, 1.0);
                pixelColor[2] = clamp(pixelColor.z() * sampleScale, 0.0, 1.0);
            }

            // (imageHeight - 1 - jj) * imageWidth + ii;
            size_t pixelIndex = pixelNumber - (jj + 1) * imageWidth + ii;
            imageExporter.fillColor(pixelIndex, pixelColor);

            std::cout << std::format("Fill color pixel placed at {}, progress = {}/{}\n", pixelIndex, ++curPixelCount, pixelNumber);
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