#include "AARect.h"
#include "Camera.h"
#include "CheckerTexture.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "ExampleBase.h"
#include "ImageTexture.h"
#include "Lambertian.h"
#include "MarbledTexture.h"
#include "MathUtils.h"
#include "Metal.h"
#include "NoiseTexture.h"
#include "Sphere.h"
#include "TurbulenceTexture.h"

#include <iostream>
#include <vector>

class ExampleEmissiveLight: public ExampleBase
{
public:
    ExampleEmissiveLight(int width, int height) : ExampleBase(width, height) {}

    virtual Color getRayColor(const Ray& ray, const HittableList& world, int depth) override
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
        {
            return Color(0.0, 0.0, 0.0);
        }

        std::optional<HitRecord> optHitRecord = world.hit(ray, DOUBLE_EPS, DOUBLE_INFINITY);
        if (!optHitRecord.has_value())
        {
            // background
            return Color(0.0, 0.0, 0.0);
        }

        Color attenuation;
        Color emitted;
        Ray scattered;
        const HitRecord& hitRecord = optHitRecord.value();
        if (const Hittable* pHitObject = hitRecord.hitObject())
        {
            if (std::shared_ptr<Material> pMaterial = pHitObject->material())
            {
                XYZ hitPoint = hitRecord.hitPoint();
                UV uv = pHitObject->uv(hitPoint);
                emitted = pMaterial->emitted(uv.x(), uv.y(), hitPoint);

                if (!pMaterial->scatter(ray, hitRecord, attenuation, scattered))
                {
                    return emitted;
                }
            }
        }

        return emitted + attenuation * getRayColor(scattered, world, depth - 1);
    }
};

int main()
{
    // World
    HittableList hittableList;

    auto red   = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(30.0, 30.0, 30.0));

    // Five walls
    hittableList.appendOne(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(0.0, 555.0, 555.0), 'x', 555.0, green));
    hittableList.appendOne(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(0.0, 555.0, 555.0), 'x', 0.0, red));
    hittableList.appendOne(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 0.0, 555.0), 'y', 0.0, white));
    hittableList.appendOne(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 0.0, 555.0), 'y', 555.0, white));
    hittableList.appendOne(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 555.0, 0.0), 'z', 555.0, white));

    // Top light
    hittableList.appendOne(std::make_shared<AARect>(XYZ(213.0, 0.0, 227.0), XYZ(343.0, 0.0, 332.0), 'y', 554.0, light));

    std::shared_ptr<ImageTexture> pEarthTexture = std::make_shared<ImageTexture>("..\\..\\Resources\\earthmap.jpg");
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(378.0, 80.0, 279.5), 80.0, std::make_shared<Lambertian>(pEarthTexture)));

    XYZ albedo = XYZ(0.5, 0.5, 0.5);
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(168.0, 60.0, 179.5), 60.0, std::make_shared<Metal>(albedo, 0.0)));

    // Camera
    XYZ lookFrom = XYZ(278.0, 278.0, -800.0);
    XYZ lookAt = XYZ(278.0, 278.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    double distToFocus = 10.0;
    double aperture = 0.1;

    constexpr int imageWidth = 600;
    constexpr int imageHeight = 600;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 40.0, aspectRatio);

    // Init example and run
    ExampleEmissiveLight example(imageWidth, imageHeight);
    example.setSampleTimes(10000);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}