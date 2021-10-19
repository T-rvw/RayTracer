#include "AARect.h"
#include "HittableList.h"
#include "ImageExporter.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"

#include <iostream>

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
    XYZ unitDir = ray.direction();
    unitDir.normalize();
    double factor = 0.5 * (unitDir.y() + 1.0);
    return (1.0 - factor) * Color(1.0, 1.0, 1.0) + factor * Color(0.5, 0.7, 1.0);
}

int main()
{
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr int pixelNumber = imageHeight * imageWidth;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    ImageExporter imageExporter(imageWidth, imageHeight);

    // World
    HittableList hittableList;
    hittableList.reserve(2);
    hittableList.add(std::make_shared<Triangle>(XYZ(-0.2, -0.2, -1.0), XYZ(0.2, 0.2, -1.0), XYZ(-0.2, 0.2, -1.0)));
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, -100.5, -1.0), 100.0));

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
            size_t pixelIndex = pixelNumber - (jj + 1) * imageWidth + ii;
            imageExporter.fillColor(pixelIndex, getRayColor(ray, hittableList));

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