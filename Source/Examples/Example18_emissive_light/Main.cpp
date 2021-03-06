#include "Camera.h"
#include "DiffuseLight.h"
#include "ExampleEmissive.h"
#include "ImageTexture.h"
#include "Lambertian.h"
#include "NoiseTexture.h"
#include "Rect.h"
#include "Sphere.h"

int main()
{
    // World
    HittableList hittableList;
    auto pNoiseTexture = std::make_shared<NoiseTexture>(4.0);
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, -1000, 0.0), 1000.0)).setMaterial(std::make_shared<Lambertian>(pNoiseTexture));

    auto pEarthTexture = std::make_shared<ImageTexture>("..\\..\\Resources\\earthmap.jpg");
    auto pEarthMaterial = std::make_shared<Lambertian>(pEarthTexture);
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, 2.0, 0.0), 2.0)).setMaterial(pEarthMaterial);

    std::shared_ptr<DiffuseLight> pDiffLightMaterial = std::make_shared<DiffuseLight>(Color(4.0, 4.0, 4.0));
    hittableList.add(std::make_shared<Rect>(XYZ(5.0, 1.0, -2.0), XYZ(3.0, 1.0, -2.0), XYZ(5.0, 3.0, -2.0))).setMaterial(pDiffLightMaterial);

    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, 8.0, 0.0), 2.0)).setMaterial(pDiffLightMaterial);

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