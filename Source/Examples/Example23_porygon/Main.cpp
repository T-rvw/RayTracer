#include "Box.h"
#include "Camera.h"
#include "DiffuseLight.h"
#include "ExampleEmissive.h"
#include "Lambertian.h"
#include "Rect.h"
#include "Triangle.h"

#include <fstream>
#include <iostream>

int main()
{
    // Porygon : multiple 
    std::vector<XYZ> vecPorygonPos;

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
        }
        
        ifs.close();
    }

    // World
    HittableList hittableList;

    auto blue = std::make_shared<Lambertian>(Color(0.0, 0.0, 1.0));
    for (size_t ii = 0; ii < vecPorygonPos.size(); ii += 3)
    {
        hittableList.add(std::make_shared<Triangle>(vecPorygonPos[ii], vecPorygonPos[ii + 1], vecPorygonPos[ii + 2])).setMaterial(blue);
    }

    constexpr double x = 0.25;
    hittableList.add(std::make_shared<Triangle>(XYZ(-x, -x + 0.7, -1.0), XYZ(-x, x + 0.7, -1.0), XYZ(x, -x + 0.7, -1.0))).setMaterial(blue);

    // Camera
    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(90.0, aspectRatio);

    // Init example and run
    ExampleBase example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}