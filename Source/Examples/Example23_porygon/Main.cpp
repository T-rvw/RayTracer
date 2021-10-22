#include "Box.h"
#include "BVHNode.h"
#include "Camera.h"
#include "DiffuseLight.h"
#include "ExampleEmissive.h"
#include "ImageTexture.h"
#include "Lambertian.h"
#include "MarbledTexture.h"
#include "Metal.h"
#include "Rect.h"
#include "Rotate.h"
#include "Translate.h"
#include "Triangle.h"

#include <fstream>
#include <iostream>

int main()
{
    XYZ total(0.0, 0.0, 0.0);

    // Porygon : multiple 
    std::vector<XYZ> vecPorygonPos;
    XYZ minP(DOUBLE_INFINITY);
    XYZ maxP(-DOUBLE_INFINITY);
    {
        // Read from txt file
        std::ifstream ifs("..\\..\\Resources\\porygon.txt");
        int vertexCount = 0;
        ifs >> vertexCount;
        vecPorygonPos.reserve(vertexCount);

        //char skipInput;
        double p0, p1, p2;
        for (int ii = 0; ii < vertexCount; ++ii)
        {
            ifs >> p0;
            ifs >> p1;
            ifs >> p2;
            vecPorygonPos.emplace_back(p0, p1, p2);

            total[0] += p0;
            total[1] += p1;
            total[2] += p2;

            minP[0] = std::min(minP[0], p0);
            minP[1] = std::min(minP[1], p1);
            minP[2] = std::min(minP[2], p2);

            maxP[0] = std::max(maxP[0], p0);
            maxP[1] = std::max(maxP[1], p1);
            maxP[2] = std::max(maxP[2], p2);
        }

        total /= vertexCount;

        ifs.close();
    }

    // World
    HittableList hittableList;

    auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(30.0, 30.0, 30.0));

    // Five walls
    constexpr double wallLength = 5.55;
    hittableList.add(std::make_shared<Rect>(XYZ(wallLength, 0.0, 0.0), XYZ(wallLength, wallLength, 0.0), XYZ(wallLength, 0.0, wallLength))).setMaterial(green);
    hittableList.add(std::make_shared<Rect>(XYZ(0.0, 0.0, 0.0), XYZ(0.0, wallLength, 0.0), XYZ(0.0, 0.0, wallLength))).setMaterial(red);
    hittableList.add(std::make_shared<Rect>(XYZ(0.0, 0.0, 0.0), XYZ(wallLength, 0.0, 0.0), XYZ(0.0, 0.0, wallLength))).setMaterial(white);
    hittableList.add(std::make_shared<Rect>(XYZ(0.0, wallLength, 0.0), XYZ(0.0, wallLength, wallLength), XYZ(wallLength, wallLength, 0.0))).setMaterial(white);
    hittableList.add(std::make_shared<Rect>(XYZ(0.0, 0.0, wallLength), XYZ(wallLength, 0.0, wallLength), XYZ(0.0, wallLength, wallLength))).setMaterial(white);

    // Top light
    hittableList.add(std::make_shared<Rect>(XYZ(2.13, 5.54, 2.27), XYZ(2.13, 5.54, 3.32), XYZ(3.43, 5.54, 2.27))).setMaterial(light);

    // [-3.30, -4.24, -6.66]
    // [3.30, 6.53, 1.66]
    constexpr double a = 2.78;

    HittableList porygon;
    double scale = 0.1;
    auto pMetalMaterial = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    for (size_t ii = 0; ii < vecPorygonPos.size(); ii += 3)
    {
        std::shared_ptr<GeometryBase> pTriangle = std::make_shared<Triangle>(vecPorygonPos[ii] * scale, vecPorygonPos[ii + 1] * scale, vecPorygonPos[ii + 2] * scale);
        pTriangle->setMaterial(white);
        pTriangle = std::make_shared<Translate>(pTriangle, XYZ(a + 3.30 * scale - 0.3, a + 4.24 * scale - 1.4, 6.66 * scale - 2.2));
        porygon.add(pTriangle);
    }
    
    std::shared_ptr<GeometryBase> pBVHTree = std::make_shared<BVHNode>(porygon, -DOUBLE_INFINITY, DOUBLE_INFINITY);
    //pBVHTree = std::make_shared<Rotate>(pBVHTree, 15);
    hittableList.add(pBVHTree);

    //auto pEarthMaterial = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("..\\..\\Resources\\earthmap.jpg"));
    //auto pMarbledTexture = std::make_shared<MarbledTexture>(4.0);
    //auto pMarbledMaterial = std::make_shared<Lambertian>(pMarbledTexture);
    //{
    //    double factor = 0.1;
    //    std::shared_ptr<GeometryBase> pBox = std::make_shared<Box>(XYZ(-3.30 * factor, -4.24 * factor, -6.66 * factor), XYZ(3.30 * factor, 6.53 * factor, 1.66 * factor));
    //    pBox->setMaterial(pEarthMaterial);
    //    pBox = std::make_shared<Translate>(pBox, XYZ(a+3.30 * factor - 0.3, a+4.24 * factor - 1.4, 6.66 * factor - 1.8));
    //    //pBox = std::make_shared<Rotate>(pBox, 30);
    //    hittableList.add(pBox);
    //}

    // Camera
    XYZ lookFrom = XYZ(a, a, -6.0);
    XYZ lookAt = XYZ(a, a, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    constexpr double distToFocus = 10.0;
    constexpr double aperture = 0.0;

    constexpr int imageWidth = 300;
    constexpr int imageHeight = 300;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 40.0, aspectRatio);

    // Init example and run
    ExampleBase example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}