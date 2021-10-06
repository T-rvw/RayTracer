#include "AARect.h"
#include "Camera.h"
#include "DiffuseLight.h"
#include "ExampleEmissive.h"
#include "ImageTexture.h"
#include "Lambertian.h"
#include "NoiseTexture.h"
#include "Sphere.h"

int main()
{
    // World
    HittableList hittableList;
    auto pNoiseTexture = new NoiseTexture(4.0);
    hittableList.add(new Sphere(XYZ(0.0, -1000, 0.0), 1000.0)).setMaterial(new Lambertian(pNoiseTexture));

    auto pEarthTexture = new ImageTexture("..\\..\\Resources\\earthmap.jpg");
    hittableList.add(new Sphere(XYZ(0.0, 2.0, 0.0), 2.0)).setMaterial(new Lambertian(pEarthTexture));

    auto pDiffLightMaterial = new DiffuseLight(Color(4.0, 4.0, 4.0));
    hittableList.add(new AARect(XYZ(3.0, 1.0, 0.0), XYZ(5.0, 3.0, 0.0), 'z', - 2.0)).setMaterial(pDiffLightMaterial);
    hittableList.add(new Sphere(XYZ(0.0, 8.0, 0.0), 2.0)).setMaterial(pDiffLightMaterial);

    // Camera
    XYZ lookFrom = XYZ(26.0, 3.0, 6.0);
    XYZ lookAt = XYZ(0.0, 2.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    double distToFocus = 10.0;
    double aperture = 0.1;

    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 20.0, aspectRatio);

    // Init example and run
    ExampleEmissive example(imageWidth, imageHeight);
    example.setSampleTimes(400);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}