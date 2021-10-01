#include "AARect.h"
#include "Box.h"
#include "Camera.h"
#include "DiffuseLight.h"
#include "ExampleEmissive.h"
#include "Lambertian.h"

int main()
{
    // World
    HittableList hittableList;

    auto red   = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(30.0, 30.0, 30.0));

    // Five walls
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(0.0, 555.0, 555.0), 'x', 555.0)).setMaterial(green);
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(0.0, 555.0, 555.0), 'x', 0.0)).setMaterial(red);
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 0.0, 555.0), 'y', 0.0)).setMaterial(white);
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 0.0, 555.0), 'y', 555.0)).setMaterial(white);
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 555.0, 0.0), 'z', 555.0)).setMaterial(white);

    // Top light
    hittableList.add(std::make_shared<AARect>(XYZ(213.0, 0.0, 227.0), XYZ(343.0, 0.0, 332.0), 'y', 554.0)).setMaterial(light);

    hittableList.add(std::make_shared<Box>(XYZ(130.0, 0, 65.0), XYZ(295.0, 165.0, 230.0))).setMaterial(white);
    hittableList.add(std::make_shared<Box>(XYZ(265.0, 0, 295.0), XYZ(430.0, 330.0, 460.0))).setMaterial(white);

    // Camera
    XYZ lookFrom = XYZ(278.0, 278.0, -800.0);
    XYZ lookAt = XYZ(278.0, 278.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    double distToFocus = 10.0;
    double aperture = 0.1;

    constexpr int imageWidth = 300;
    constexpr int imageHeight = 300;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 40.0, aspectRatio);

    // Init example and run
    ExampleEmissive example(imageWidth, imageHeight);
    example.setSampleTimes(10000);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}