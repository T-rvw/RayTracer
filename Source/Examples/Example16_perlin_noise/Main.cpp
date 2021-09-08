#include "Camera.h"
#include "CheckerTexture.h"
#include "Dielectric.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "MarbledTexture.h"
#include "MathUtils.h"
#include "Metal.h"
#include "NoiseTexture.h"
#include "Sphere.h"
#include "TurbulenceTexture.h"

#include <iostream>
#include <vector>

class ExamplePerlinNoise : public ExampleBase
{
public:
    ExamplePerlinNoise(int width, int height) : ExampleBase(width, height) {}

    virtual Color getRayColor(const Ray& ray, const HittableList& world, int depth) override
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
    // World
    HittableList hittableList;
    std::shared_ptr<CheckerTexture> pCheckerTexture = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, -1000, 0.0), 1000.0, std::make_shared<Lambertian>(pCheckerTexture)));

    constexpr double noiseScale = 4.0;
    std::shared_ptr<NoiseTexture> pNoiseTexture = std::make_shared<NoiseTexture>(noiseScale);
    std::shared_ptr<MarbledTexture> pMarbledTexture = std::make_shared<MarbledTexture>(noiseScale);
    std::shared_ptr<TurbulenceTexture> pTurbulenceTexture = std::make_shared<TurbulenceTexture>(noiseScale);
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(1.0, 1.0, +2.1), 1.0, std::make_shared<Lambertian>(pNoiseTexture)));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(1.0, 1.0,  0.0), 1.0, std::make_shared<Lambertian>(pMarbledTexture)));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(1.0, 1.0, -2.1), 1.0, std::make_shared<Lambertian>(pTurbulenceTexture)));

    // Camera
    XYZ lookFrom = XYZ(13.0, 2.0, 3.0);
    XYZ lookAt = XYZ(0.0, 0.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    double distToFocus = 10.0;
    double aperture = 0.1;

    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 20.0, aspectRatio);

    // Init example and run
    ExamplePerlinNoise example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}