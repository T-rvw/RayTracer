#include "Camera.h"
#include "ExampleBase.h"
#include "MathUtils.h"
#include "Sphere.h"

#include <iostream>
#include <vector>

class ExampleDiffuseMaterial : public ExampleBase
{
public:
    ExampleDiffuseMaterial(int width, int height) : ExampleBase(width, height) {}

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
};

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
    constexpr int maxRecursiveDepth = 50;

    // World
    HittableList hittableList;
    hittableList.reserve(2);
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, 0.0, -1.0), 0.5));
    hittableList.appendOne(std::make_shared<Sphere>(XYZ(0.0, -100.5, -1.0), 100.0));

	// Camera
    Camera camera(90.0, aspectRatio);

    // Init example and run
    ExampleDiffuseMaterial example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}