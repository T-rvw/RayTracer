#include "Camera.h"
#include "HittableList.h"
#include "MathUtils.h"
#include "PPMExporter.h"
#include "Ray.h"
#include "Sphere.h"

#include <iostream>
#include <vector>

XYZ randomInUnitSphere()
{
    while (true)
    {
        XYZ p = XYZ::random(-1.0, 1.0);
        if (p.lengthSquare() >= 1.0)
        {
            continue;
        }

        return p;
    }
}

Color getRayColor(const Ray& ray, const HittableList& world, int depth)
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return Color(0.0, 0.0, 0.0);
    }

    std::optional<HitRecord> optHitRecord = world.hit(ray, DOUBLE_EPS, DOUBLE_INFINITY);
    if (optHitRecord.has_value())
    {
        const HitRecord& hitRecord = optHitRecord.value();
        const XYZ& hitPoint = hitRecord.hitPoint();
        const XYZ& normal = hitRecord.normal();

        constexpr bool bUseLambertian = false;
        constexpr bool bUseHemisphereScatter = false;
        XYZ randomReflection = randomInUnitSphere();
        if constexpr (!bUseHemisphereScatter)
        {
            XYZ target = hitPoint + normal + (bUseLambertian ? unit(randomReflection) : randomReflection);
            return 0.5 * getRayColor(Ray(hitPoint, target - hitPoint), world, depth - 1);
        }
        else
        {
            double reflectionDirection = dot(randomReflection, normal) > 0.0 ? 1.0 : -1.0;
            XYZ target = hitPoint + reflectionDirection * randomReflection;
            return 0.5 * getRayColor(Ray(hitPoint, target - hitPoint), world, depth - 1);
        }
    }

    // background
	XYZ unitDir = unit(ray.direction());
	double factor = 0.5 * (unitDir.y() + 1.0);
	return (1.0 - factor) * Color(1.0, 1.0, 1.0) + factor * Color(0.5, 0.7, 1.0);
}

int main()
{
    // Config
    constexpr int samplesPerPixel = 100;
    constexpr int maxRecursiveDepth = 10;

    // World
    HittableList hittableList;
    hittableList.reserve(2);
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, 0.0, -1.0), 0.5));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, -100.5, -1.0), 100.0));

	// Camera
    Camera camera;

    // PPMExporter
    constexpr uint16_t imageWidth = 400;
    constexpr uint16_t imageHeight = static_cast<uint16_t>(imageWidth / camera.aspectRatio());
    PPMExporter ppmExporter(imageWidth, imageHeight);

	// Render
    size_t pixelIndex = 0;
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
                pixelColor += getRayColor(ray, hittableList, maxRecursiveDepth);
            }

            // sample && gamma-correct(1/2).
            {
                double sampleScale = 1.0 / samplesPerPixel;
                pixelColor[0] = clamp(pow(pixelColor.x() * sampleScale, 0.5), 0.0, 1.0);
                pixelColor[1] = clamp(pow(pixelColor.y() * sampleScale, 0.5), 0.0, 1.0);
                pixelColor[2] = clamp(pow(pixelColor.z() * sampleScale, 0.5), 0.0, 1.0);
            }

            ppmExporter.fillColor(pixelIndex, pixelColor);

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