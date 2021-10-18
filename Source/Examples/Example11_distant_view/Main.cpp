#include "Camera.h"
#include "Dielectric.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Sphere.h"

int main()
{
    // World
    auto groupMaterial = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto centerMaterial = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto leftMaterial = std::make_shared<Dielectric>(1.5);
    auto rightMaterial = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);
    
    HittableList hittableList;
    hittableList.reserve(5);
    hittableList.add(std::make_shared<Sphere>(XYZ( 0.0, -100.5, -1.0), 100.0)).setMaterial(groupMaterial);
    hittableList.add(std::make_shared<Sphere>(XYZ( 0.0,    0.0, -1.0),   0.5)).setMaterial(centerMaterial);
    hittableList.add(std::make_shared<Sphere>(XYZ(-1.0,    0.0, -1.0),   0.5)).setMaterial(leftMaterial);
    hittableList.add(std::make_shared<Sphere>(XYZ(-1.0,    0.0, -1.0), -0.45)).setMaterial(leftMaterial);
    hittableList.add(std::make_shared<Sphere>(XYZ( 1.0,    0.0, -1.0),   0.5)).setMaterial(rightMaterial);

	// Camera
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(XYZ(-2.0, 2.0, 1.0), XYZ(0.0, 0.0, -1.0), XYZ(0.0, 1.0, 0.0), 20, aspectRatio);

    // Init example and run
    ExampleBase example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}