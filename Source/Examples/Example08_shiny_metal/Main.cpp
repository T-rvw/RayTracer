#include "Camera.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "MathUtils.h"
#include "Metal.h"
#include "Sphere.h"

#include <iostream>
#include <vector>

class ExampleShinyMetal : public ExampleBase
{
public:
    ExampleShinyMetal(int width, int height) : ExampleBase(width, height) {}

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
    auto groudMaterial = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto centerMaterial = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto leftMaterial = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
    auto rightMaterial = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);
    
    HittableList hittableList;
    hittableList.reserve(4);
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(+0.0, -100.5, -1.0), 100.0, groudMaterial));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(+0.0,    0.0, -1.0),   0.5, centerMaterial));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(-1.2,    0.0, -1.0),   0.5, leftMaterial));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(+1.2,    0.0, -1.0),   0.5, rightMaterial));

	// Camera
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(90.0, aspectRatio);

    // Init example and run
    ExampleShinyMetal example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}