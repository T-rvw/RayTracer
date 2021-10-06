#include "AARect.h"
#include "Box.h"
#include "BVHNode.h"
#include "Camera.h"
#include "ConstantMedium.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "ExampleEmissive.h"
#include "ImageTexture.h"
#include "Isotropic.h"
#include "Lambertian.h"
#include "Metal.h"
#include "NoiseTexture.h"
#include "Sphere.h"
#include "Rotate.h"
#include "Translate.h"

int main()
{
    // World
    HittableList world;
    
    // Floor boxes managed by BVH
    {
        HittableList boxes;
    
        auto ground = new Lambertian(Color(0.48, 0.83, 0.53));
        constexpr int boxesPerStride = 20;
        constexpr double w = 100.0;
        for (int ii = 0; ii < boxesPerStride; ++ii)
        {
            for (int jj = 0; jj < boxesPerStride; ++jj)
            {
                auto x0 = -1000.0 + ii * w;
                auto z0 = -1000.0 + jj * w;
                auto x1 = x0 + w;
                auto z1 = z0 + w;
    
                auto pBox = new Box(XYZ(x0, 0.0, z0), XYZ(x1, MathUtils::randomDouble(1.0, 101.0), z1));
                boxes.add(pBox).setMaterial(ground);
            }
        }
    
        world.add(new BVHNode(boxes, 0.0, 1.0));
    }

    // Top light
    auto light = new DiffuseLight(Color(7.0, 7.0, 7.0));
    world.add(new AARect(XYZ(123.0, 0.0, 147.0), XYZ(423.0, 0.0, 412.0), 'y', 554.0)).setMaterial(light);

    // Motion blur ball
    {
        auto pSphere = new Sphere(XYZ(400, 400, 200), 50.0);
        pSphere->setMoveInfo(XYZ(430, 400, 200), 0.0, 1.0);
        pSphere->setMaterial(new Lambertian(Color(0.7, 0.3, 0.1)));
        world.add(pSphere);
    }

    // Dielectric ball
    auto pDielectricMaterial = new Dielectric(1.5);
    {
        auto pSphere = new Sphere(XYZ(260.0, 150.0, 45.0), 50.0);
        pSphere->setMaterial(pDielectricMaterial);
        world.add(pSphere);
    }

    // Metal ball
    {
        auto pSphere = new Sphere(XYZ(0.0, 150.0, 145.0), 50.0);
        pSphere->setMaterial(new Metal(Color(0.8, 0.8, 0.9), 1.0));
        world.add(pSphere);
    }

    // Voxel rendering
    {
        auto boundary = new Sphere(XYZ(360.0, 150.0, 145.0), 70.0);
        boundary->setMaterial(pDielectricMaterial);
        auto pIsotropicColor = new Isotropic(new SolidColor(Color(0.2, 0.4, 0.9)));
        world.add(new ConstantMedium(boundary, 0.2)).setMaterial(pIsotropicColor);
    }
    
    {
        auto boundary = new Sphere(XYZ(0.0, 0.0, 0.0), 5000.0);
        boundary->setMaterial(pDielectricMaterial);
        auto pIsotropicColor = new Isotropic(new SolidColor(Color(1.0, 1.0, 1.0)));
        world.add(new ConstantMedium(boundary, 0.0001)).setMaterial(pIsotropicColor);
    }
    
    // Earth ball
    {
        auto pEarthMaterial = new Lambertian(new ImageTexture("..\\..\\Resources\\earthmap.jpg"));
        world.add(new Sphere(XYZ(400.0, 200.0, 400.0), 100.0)).setMaterial(pEarthMaterial);
    }

    // Perlin noise ball
    {
        auto pPerlinTexture = new NoiseTexture(0.1);
        auto pPerlinMaterial = new Lambertian(pPerlinTexture);
        world.add(new Sphere(XYZ(220.0, 280.0, 300.0), 80.0)).setMaterial(pPerlinMaterial);
    }

    // Multiple small balls managed by BVH
    {
        HittableList balls;
        auto white = new Lambertian(Color(0.73, 0.73, 0.73));
        for (int ii = 0; ii < 1000; ++ii)
        {
            balls.add(new Sphere(XYZ::random(0.0, 165.0), 10.0)).setMaterial(white);
        }
        
        auto pBVHNode = new BVHNode(balls, 0.0, 1.0);
        auto pRotateNode = new Rotate(pBVHNode, 15.0);
        auto pTranslateNode = new Translate(pRotateNode, XYZ(-100.0, 270.0, 395.0));
        world.add(pTranslateNode);
    }

    // Camera
    XYZ lookFrom = XYZ(478.0, 278.0, -600.0);
    XYZ lookAt = XYZ(278.0, 278.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);

    constexpr int imageWidth = 800;
    constexpr int imageHeight = 800;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, 40.0, aspectRatio);

    // Init example and run
    ExampleEmissive example(imageWidth, imageHeight);
    example.setSampleTimes(10000);
    example.setMaxRecursiveDepth(50);
    example.process(camera, world);
    example.generate("test.png");

    return 0;
}