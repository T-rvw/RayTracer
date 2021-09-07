#include "Camera.h"
#include "Dielectric.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "MathUtils.h"
#include "Metal.h"
#include "Sphere.h"

#include <iostream>
#include <vector>

class ExampleLotsOfSpheres : public ExampleBase
{
public:
    ExampleLotsOfSpheres(int width, int height) :
        ExampleBase(width, height)
    {
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
};

int main()
{
    // ImageExporter
    constexpr int imageWidth = 1200;
    constexpr int imageHeight = 800;

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

    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 20.0, aspectRatio);

    // Example init
    ExampleLotsOfSpheres example(imageWidth, imageHeight);
    example.setSampleTimes(500);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}