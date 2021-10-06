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
    CheckerTexture* pCheckerTexture = new CheckerTexture(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    hittableList.add(new Sphere(XYZ(0.0, -1000, 0.0), 1000.0)).setMaterial(new Lambertian(pCheckerTexture));

    constexpr double noiseScale = 4.0;
    auto pNoiseTexture = new NoiseTexture(noiseScale);
    auto pMarbledTexture = new MarbledTexture(noiseScale);
    auto pTurbulenceTexture = new TurbulenceTexture(noiseScale);
    hittableList.add(new Sphere(XYZ(1.0, 1.0, +2.1), 1.0)).setMaterial(new Lambertian(pNoiseTexture));
    hittableList.add(new Sphere(XYZ(1.0, 1.0,  0.0), 1.0)).setMaterial(new Lambertian(pMarbledTexture));
    hittableList.add(new Sphere(XYZ(1.0, 1.0, -2.1), 1.0)).setMaterial(new Lambertian(pTurbulenceTexture));

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