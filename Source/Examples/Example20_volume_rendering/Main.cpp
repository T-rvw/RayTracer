#include "AARect.h"
#include "Box.h"
#include "Camera.h"
#include "ConstantMedium.h"
#include "DiffuseLight.h"
#include "ExampleEmissive.h"
#include "Isotropic.h"
#include "Lambertian.h"
#include "Rotate.h"
#include "Translate.h"

int main()
{
    // World
    HittableList hittableList;

    auto red   = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(7.0, 7.0, 7.0));

    // Five walls
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(0.0, 555.0, 555.0), 'x', 555.0)).setMaterial(green);
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(0.0, 555.0, 555.0), 'x', 0.0)).setMaterial(red);
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 0.0, 555.0), 'y', 0.0)).setMaterial(white);
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 0.0, 555.0), 'y', 555.0)).setMaterial(white);
    hittableList.add(std::make_shared<AARect>(XYZ(0.0, 0.0, 0.0), XYZ(555.0, 555.0, 0.0), 'z', 555.0)).setMaterial(white);

    // Top light
    hittableList.add(std::make_shared<AARect>(XYZ(113, 0.0, 127.0), XYZ(443.0, 0.0, 432.0), 'y', 554.0)).setMaterial(light);

    // Boxes
    {
        std::shared_ptr<GeometryBase> pBox = std::make_shared<Box>(XYZ(0.0, 0.0, 0.0), XYZ(165.0, 330.0, 165.0));
        pBox = std::make_shared<Rotate>(pBox, 15);
        pBox = std::make_shared<Translate>(pBox, XYZ(265.0, 0, 295.0));
        auto pIsotropicColor = std::make_shared<Isotropic>(std::make_shared<SolidColor>(Color(0.0, 0.0, 0.0)));
        hittableList.add(std::make_shared<ConstantMedium>(pBox, 0.01)).setMaterial(pIsotropicColor);
    }

    {
        std::shared_ptr<GeometryBase> pBox = std::make_shared<Box>(XYZ(0.0, 0.0, 0.0), XYZ(165.0, 165.0, 165.0));
        pBox = std::make_shared<Rotate>(pBox, -18);
        pBox = std::make_shared<Translate>(pBox, XYZ(130.0, 0, 65.0));
        auto pIsotropicColor = std::make_shared<Isotropic>(std::make_shared<SolidColor>(Color(1.0, 1.0, 1.0)));
        hittableList.add(std::make_shared<ConstantMedium>(pBox, 0.01)).setMaterial(pIsotropicColor);
    }

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
    example.setSampleTimes(500);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}