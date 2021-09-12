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
#include "Rect2D.h"
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
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, -1000, 0.0), 1000.0, std::make_shared<Lambertian>(std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9)))));

    std::shared_ptr<ImageTexture> pEarthTexture = std::make_shared<ImageTexture>("..\\..\\Resources\\earthmap.jpg");
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, 2.0, 0.0), 2.0, std::make_shared<Lambertian>(pEarthTexture)));

    //std::shared_ptr<MarbledTexture> pNoiseTexture = std::make_shared<MarbledTexture>(4.0);
    //hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, -1000, 0.0), 1000.0, std::make_shared<Lambertian>(pNoiseTexture)));
    //hittableList.appendOne(std::make_shared<Sphere>(XYZ(0, 2.0, 0), 2.0, std::make_shared<Lambertian>(pNoiseTexture)));

    std::shared_ptr<DiffuseLight> pDiffLightMaterial = std::make_shared<DiffuseLight>(Color(4.0, 4.0, 4.0));
    hittableList.appendOne(std::make_shared<Rect2D>(3.0, 5.0, 1.0, 3.0, -2.0, pDiffLightMaterial));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, 8.0, 0.0), 2.0, pDiffLightMaterial));

    // Camera
    XYZ lookFrom = XYZ(26.0, 3.0, 6.0);
    XYZ lookAt = XYZ(0.0, 2.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    double distToFocus = 10.0;
    double aperture = 0.1;

    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 20.0, aspectRatio);

    // Init example and run
    ExampleEmissiveLight example(imageWidth, imageHeight);
    example.setSampleTimes(400);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}