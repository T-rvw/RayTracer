#include "Camera.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "Sphere.h"

int main()
{
    // World
    double radians = cos(PI * 0.25);
    auto leftMaterial = new Lambertian(Color(0.0, 0.0, 1.0));
    auto rightMaterial = new Lambertian(Color(1.0, 0.0, 0.0));
    
    HittableList hittableList;
    hittableList.reserve(2);
    hittableList.add(new Sphere(XYZ(-radians, 0.0, -1.0), radians)).setMaterial(leftMaterial);
    hittableList.add(new Sphere(XYZ(+radians, 0.0, -1.0), radians)).setMaterial(rightMaterial);

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