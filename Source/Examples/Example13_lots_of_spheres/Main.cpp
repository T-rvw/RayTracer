#include "Camera.h"
#include "Dielectric.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Sphere.h"

int main()
{
    // World
    auto groupMaterial = new Lambertian(Color(0.5, 0.5, 0.5));

    HittableList hittableList;
    hittableList.add(new Sphere(XYZ( 0.0, -1000, 0.0), 1000.0)).setMaterial(groupMaterial);

    for (int ii = -11; ii < 11; ++ii)
    {
        for (int jj = -11; jj < 11; ++jj)
        {
            double chooseMaterial = MathUtils::randomDouble();
            XYZ center(ii + 0.9 * MathUtils::randomDouble(), 0.2, jj + 0.9 * MathUtils::randomDouble());

            if ((center - XYZ(4.0, 0.2, 0.0)).length() > 0.9)
            {
                Material* sphereMaterial = nullptr;
                if (chooseMaterial < 0.8)
                {
                    XYZ albedo = XYZ(MathUtils::randomDouble() * MathUtils::randomDouble(), MathUtils::randomDouble() * MathUtils::randomDouble(), MathUtils::randomDouble() * MathUtils::randomDouble());
                    sphereMaterial = new Lambertian(albedo);
                }
                else if (chooseMaterial < 0.95)
                {
                    XYZ albedo = XYZ(MathUtils::randomDouble(0.5, 1.0), MathUtils::randomDouble(0.5, 1.0), MathUtils::randomDouble(0.5, 1.0));
                    double fuzz = MathUtils::randomDouble(0.0, 0.5);
                    sphereMaterial = new Metal(albedo, fuzz);
                }
                else
                {
                    sphereMaterial = new Dielectric(1.5);
                }
                hittableList.add(new Sphere(center, 0.2)).setMaterial(sphereMaterial);
            }
        }
    }

    hittableList.add(new Sphere(XYZ(0.0, 1.0, 0.0), 1.0)).setMaterial(new Dielectric(1.5));
    hittableList.add(new Sphere(XYZ(-4.0, 1.0, 0.0), 1.0)).setMaterial(new Lambertian(XYZ(0.4, 0.2, 0.1)));
    hittableList.add(new Sphere(XYZ(4.0, 1.0, 0.0), 1.0)).setMaterial(new Metal(XYZ(0.7, 0.6, 0.5), 0.0));

	// Camera
    XYZ lookFrom = XYZ(13.0, 2.0, 3.0);
    XYZ lookAt = XYZ(0.0, 0.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    double distToFocus = 10.0;
    double aperture = 0.1;

    constexpr int imageWidth = 1200;
    constexpr int imageHeight = 800;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 20.0, aspectRatio);

    // Init example and run
    ExampleBase example(imageWidth, imageHeight);
    example.setSampleTimes(500);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}