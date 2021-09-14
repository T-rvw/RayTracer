#include "Camera.h"
#include "Dielectric.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "MathUtils.h"
#include "Metal.h"
#include "Sphere.h"

#include <iostream>
#include <vector>

int main()
{
    // World
    double radians = cos(PI * 0.25);
    auto leftMaterial = std::make_shared<Lambertian>(Color(0.0, 0.0, 1.0));
    auto rightMaterial = std::make_shared<Lambertian>(Color(1.0, 0.0, 0.0));
    
    HittableList hittableList;
    hittableList.reserve(2);
    hittableList.add(std::make_shared<Sphere>(XYZ(-radians, 0.0, -1.0), radians)).setMaterial(leftMaterial);
    hittableList.add(std::make_shared<Sphere>(XYZ(+radians, 0.0, -1.0), radians)).setMaterial(rightMaterial);

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