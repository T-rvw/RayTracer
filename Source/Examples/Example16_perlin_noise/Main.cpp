#include "Camera.h"
#include "CheckerTexture.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "MarbledTexture.h"
#include "NoiseTexture.h"
#include "Sphere.h"
#include "TurbulenceTexture.h"

int main()
{
    // World
    HittableList hittableList;
    std::shared_ptr<CheckerTexture> pCheckerTexture = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, -1000, 0.0), 1000.0)).setMaterial(std::make_shared<Lambertian>(pCheckerTexture));

    constexpr double noiseScale = 4.0;
    std::shared_ptr<NoiseTexture> pNoiseTexture = std::make_shared<NoiseTexture>(noiseScale);
    std::shared_ptr<MarbledTexture> pMarbledTexture = std::make_shared<MarbledTexture>(noiseScale);
    std::shared_ptr<TurbulenceTexture> pTurbulenceTexture = std::make_shared<TurbulenceTexture>(noiseScale);
    hittableList.add(std::make_shared<Sphere>(XYZ(1.0, 1.0, +2.1), 1.0)).setMaterial(std::make_shared<Lambertian>(pNoiseTexture));
    hittableList.add(std::make_shared<Sphere>(XYZ(1.0, 1.0,  0.0), 1.0)).setMaterial(std::make_shared<Lambertian>(pMarbledTexture));
    hittableList.add(std::make_shared<Sphere>(XYZ(1.0, 1.0, -2.1), 1.0)).setMaterial(std::make_shared<Lambertian>(pTurbulenceTexture));

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