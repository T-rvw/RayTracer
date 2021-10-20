#include "Camera.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "Rect.h"
#include "Triangle.h"

int main()
{
    // World
    HittableList hittableList;

    auto blue = std::make_shared<Lambertian>(Color(0.0, 0.0, 1.0));
    constexpr double x = 0.25;
    hittableList.add(std::make_shared<Rect>(XYZ(-x, -x, -1.0), XYZ(-x, x, -1.0), XYZ(x, -x, -1.0))).setMaterial(blue);
    hittableList.add(std::make_shared<Triangle>(XYZ(-x, -x + 0.7, -1.0), XYZ(-x, x + 0.7, -1.0), XYZ(x, -x + 0.7, -1.0))).setMaterial(blue);

    // Camera
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(90.0, aspectRatio);

    // Init example and run
    ExampleBase example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}