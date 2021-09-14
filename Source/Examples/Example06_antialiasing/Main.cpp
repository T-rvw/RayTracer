#include "Camera.h"
#include "ExampleBase.h"
#include "MathUtils.h"
#include "Sphere.h"

#include <iostream>
#include <vector>

class ExampleAntialiasing : public ExampleBase
{
public:
    ExampleAntialiasing(int width, int height) : ExampleBase(width, height) {}

    virtual Color getRayColor(const Ray& ray, const HittableList& world, int /*depth*/) override
    {
        std::optional<HitRecord> optHitRecord = world.hit(ray, 0, DOUBLE_INFINITY);
        if (optHitRecord.has_value())
        {
            const HitRecord& hitRecord = optHitRecord.value();
            const XYZ& normal = hitRecord.normal();
            // Map normal's xyz to rgb
            return 0.5 * Color(normal.x() + 1.0, normal.y() + 1.0, normal.z() + 1.0);
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
    hittableList.reserve(2);
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, 0.0, -1.0), 0.5));
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, -100.5, -1.0), 100.0));

	// Camera
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(90.0, aspectRatio);

    // Init example and run
    ExampleAntialiasing example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}