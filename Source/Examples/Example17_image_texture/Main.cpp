#include "Camera.h"
#include "CheckerTexture.h"
#include "Dielectric.h"
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

int main()
{
    // World
    HittableList hittableList;
    auto pCheckerTexture = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, -1000, 0.0), 1000.0)).setMaterial(std::make_shared<Lambertian>(pCheckerTexture));

    std::shared_ptr<ImageTexture> pEarthTexture = std::make_shared<ImageTexture>("..\\..\\Resources\\earthmap.jpg");
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, 1.2, 0.0), 1.0)).setMaterial(std::make_shared<Lambertian>(pEarthTexture));

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
    ExampleBase example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}