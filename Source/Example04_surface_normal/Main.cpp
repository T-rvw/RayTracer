#include "PPMExporter.h"
#include "Ray.h"
#include "XYZ.h"

#include <iostream>

std::optional<double> hitSphere(const XYZ& center, double radius, const Ray& ray)
{
    XYZ ray2Sphere = ray.origin() - center;
    double a = ray.direction().lengthSquare();
    double half_b = dot(ray2Sphere, ray.direction());
    double c = ray2Sphere.lengthSquare() - radius * radius;
    double discriminant = half_b * half_b - a * c;
    
    if (discriminant < 0.0)
    {
        return std::nullopt;
    }

    return (-half_b - sqrt(discriminant)) / a;
}

Color getRayColor(const Ray& ray)
{
    std::optional<double> t = hitSphere(XYZ(0.0, 0.0, -1.0), 0.5, ray);
    if (t.has_value())
    {
        XYZ normal = unit(ray.at(t.value()) - XYZ(0.0, 0.0, -1.0));
        // Map normal's xyz to rgb
        return 0.5 * Color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
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
    size_t pixelNumber = imageHeight * imageWidth;
#pragma omp parallel for
    for (int jj = imageHeight - 1; jj >= 0; --jj)
    {
        for (int ii = 0; ii < imageWidth; ++ii)
        {
			double u = static_cast<double>(ii) / (imageWidth - 1);
			double v = static_cast<double>(jj) / (imageHeight - 1);
            
			Ray ray(origin, leftDownCorner + u * horizontal + v * vertical - origin);

            // (imageHeight - 1 - jj) * imageWidth + ii;
            size_t pixelIndex = pixelNumber - (jj + 1) * imageWidth + ii;
            ppmExporter.fillColor(pixelIndex, getRayColor(ray));
            std::cout << std::format("Fill color pixel placed at {}\n", pixelIndex);
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