#include "PPMExporter.h"
#include "Ray.h"
#include "Sphere.h"
#include "XYZ.h"

#include <iostream>
#include <vector>

Color getRayColor(const Ray& ray)
{
    std::vector<Sphere*> vecSpheres;
    Sphere sphere1(XYZ(-0.35, 0.0, -1.0), 0.3);
    Sphere sphere2(XYZ(+0.35, 0.0, -1.0), 0.3);
    vecSpheres.push_back(&sphere1);
    vecSpheres.push_back(&sphere2);

    for (size_t ii = 0; ii < vecSpheres.size(); ++ii)
    {
        Sphere* pSphere = vecSpheres[ii];

        std::optional<HitRecord> optHitRecord = pSphere->hit(ray);
        if (optHitRecord.has_value())
        {
            const HitRecord& hitRecord = optHitRecord.value();
            const XYZ& normal = hitRecord.normal();
            // Map normal's xyz to rgb
            return 0.5 * Color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
        }
    }

    // background
	XYZ unitDir = unit(ray.direction());
	double factor = 0.5 * (unitDir.y() + 1.0);
	return (1.0 - factor) * Color(1.0, 1.0, 1.0) + factor * Color(0.5, 0.7, 1.0);
}

int main()
{
	constexpr double aspectRatio = 16.0 / 9.0;
    constexpr uint16_t imageWidth = 400;
    constexpr uint16_t imageHeight = static_cast<uint16_t>(imageWidth / aspectRatio);
    PPMExporter ppmExporter(imageWidth, imageHeight);

	// Camera
	double viewPortHeight = 2.0;
	double viewPortWidth = aspectRatio * viewPortHeight;
	double focalLength = 1.0;
	
	XYZ origin(0.0, 0.0, 0.0);
	XYZ horizontal(viewPortWidth, 0.0, 0.0);
	XYZ vertical(0.0, viewPortHeight, 0.0);
	XYZ leftDownCorner = origin - horizontal * 0.5 - vertical * 0.5 - XYZ(0.0, 0.0, focalLength);

	// Render
    size_t pixelIndex = 0;
    for (int jj = imageHeight - 1; jj >= 0; --jj)
    {
        for (int ii = 0; ii < imageWidth; ++ii)
        {
			double u = static_cast<double>(ii) / (imageWidth - 1);
			double v = static_cast<double>(jj) / (imageHeight - 1);
            
			Ray ray(origin, leftDownCorner + u * horizontal + v * vertical - origin);
            ppmExporter.fillColor(pixelIndex, getRayColor(ray));
            std::cout << "Fill color pixel placed at " << pixelIndex << std::endl;
            ++pixelIndex;
        }
    }    
	
    PPMResult result = ppmExporter.generate("test.ppm");
    if (PPMResult::TotalSuccess == result)
    {
        std::cout << "Succeed to generate ppm totally." << std::endl;
    }
    else if (PPMResult::PartialSuccess == result)
    {
        std::cout << "Succeed to generate ppm partially." << std::endl;
    }
    else if (PPMResult::Failure == result)
    {
        std::cout << "Failed to generate ppm." << std::endl;
    }

    return 0;
}