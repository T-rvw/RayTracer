#include "Camera.h"
#include "Dielectric.h"
#include "HittableList.h"
#include "Lambertian.h"
#include "MathUtils.h"
#include "Metal.h"
#include "PPMExporter.h"
#include "Ray.h"
#include "Sphere.h"

#include <iostream>
#include <vector>

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
        if (const Hittable* pHitObject = hitRecord.hitObject())
        {
            Ray scattered;
            Color attenuation;
            if (std::shared_ptr<Material> pMaterial = pHitObject->material())
            {
                if (pMaterial->scatter(ray, hitRecord, attenuation, scattered))
                {
                    return attenuation * getRayColor(scattered, world, depth - 1);
                }

                return Color(0.0, 0.0, 0.0);
            }
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
    constexpr int samplesPerPixel = 500;
    constexpr int maxRecursiveDepth = 50;

    // World
    auto groupMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));

    HittableList hittableList;
    hittableList.appendOne(std::make_shared<Sphere>(XYZ( 0.0, -1000, 0.0), 1000.0, groupMaterial));

    for (int ii = -11; ii < 11; ++ii)
    {
        for (int jj = -11; jj < 11; ++jj)
        {
            double chooseMaterial = randomDouble();
            XYZ center(ii + 0.9 * randomDouble(), 0.2, jj + 0.9 * randomDouble());

            if ((center - XYZ(4.0, 0.2, 0.0)).length() > 0.9)
            {
                std::shared_ptr<Material> sphereMaterial;
                if (chooseMaterial < 0.8)
                {
                    XYZ albedo = XYZ(randomDouble() * randomDouble(), randomDouble() * randomDouble(), randomDouble() * randomDouble());
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                }
                else if (chooseMaterial < 0.95)
                {
                    XYZ albedo = XYZ(randomDouble(0.5, 1.0), randomDouble(0.5, 1.0), randomDouble(0.5, 1.0));
                    double fuzz = randomDouble(0.0, 0.5);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                }
                else
                {
                    sphereMaterial = std::make_shared<Dielectric>(1.5);
                }
                hittableList.appendOne(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
            }
        }
    }

    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, 1.0, 0.0), 1.0, std::make_shared<Dielectric>(1.5)));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(-4.0, 1.0, 0.0), 1.0, std::make_shared<Lambertian>(XYZ(0.4, 0.2, 0.1))));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(4.0, 1.0, 0.0), 1.0, std::make_shared<Metal>(XYZ(0.7, 0.6, 0.5), 0.0)));

	// Camera
    XYZ lookFrom = XYZ(13.0, 2.0, 3.0);
    XYZ lookAt = XYZ(0.0, 0.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    double distToFocus = 10.0;
    double aperture = 0.1;

    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 20.0, 3.0 / 2.0);

    // PPMExporter
    constexpr uint16_t imageWidth = 1200;
    uint16_t imageHeight = static_cast<uint16_t>(imageWidth / camera.aspectRatio());
    PPMExporter ppmExporter(imageWidth, imageHeight);

	// Render
    size_t pixelNumber = imageHeight * imageWidth;
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
                pixelColor += getRayColor(ray, hittableList, maxRecursiveDepth);
            }

            // sample && gamma-correct(1/2).
            {
                double sampleScale = 1.0 / samplesPerPixel;
                pixelColor[0] = clamp(pow(pixelColor.x() * sampleScale, 0.5), 0.0, 1.0);
                pixelColor[1] = clamp(pow(pixelColor.y() * sampleScale, 0.5), 0.0, 1.0);
                pixelColor[2] = clamp(pow(pixelColor.z() * sampleScale, 0.5), 0.0, 1.0);
            }

            // (imageHeight - 1 - jj) * imageWidth + ii;
            size_t pixelIndex = pixelNumber - (jj + 1) * imageWidth + ii;
            ppmExporter.fillColor(pixelIndex, pixelColor);

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