#include "Camera.h"
#include "CheckerTexture.h"
#include "Dielectric.h"
#include "ExampleBase.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Sphere.h"

class ExampleCheckerTexture : public ExampleBase
{
public:
    ExampleCheckerTexture(int width, int height) : ExampleBase(width, height) {}

    virtual Color getRayColor(const Ray& ray, const HittableList& world, int depth) override
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
        {
            return Color(0.0, 0.0, 0.0);
        }

        std::optional<HitRecord> optHitRecord = world.hit(ray, DOUBLE_EPS, DOUBLE_INFINITY);
        if (optHitRecord.has_value())
        {
            const HitRecord& hitRecord = optHitRecord.value();
            if (const GeometryBase* pHitObject = hitRecord.hitObject())
            {
                Ray scattered;
                Color attenuation;
                if (std::shared_ptr<Material> pMaterial = pHitObject->material())
                {
                    if (pMaterial->scatter(ray, hitRecord, attenuation, scattered))
                    {
                        return attenuation * getRayColor(scattered, world, depth - 1);
                    }

                    return Color(0.0, 0.0, 0.0);
                }
            }
        }

        // background
        XYZ unitDir = unit(ray.direction());
        double factor = 0.5 * (unitDir.y() + 1.0);
        return (1.0 - factor) * Color(1.0, 1.0, 1.0) + factor * Color(0.5, 0.7, 1.0);
    }
};

int main()
{
    // World
    std::shared_ptr<CheckerTexture> pCheckerTexture = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    std::shared_ptr<Lambertian> pGroupMaterial = std::make_shared<Lambertian>(pCheckerTexture);

    HittableList hittableList;
    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, -1000, 0.0), 1000.0)).setMaterial(pGroupMaterial);

    for (int ii = -11; ii < 11; ++ii)
    {
        for (int jj = -11; jj < 11; ++jj)
        {
            double chooseMaterial = randomDouble();
            XYZ center(ii + 0.9 * randomDouble(), 0.2, jj + 0.9 * randomDouble());

            if ((center - XYZ(4.0, 0.2, 0.0)).length() > 0.9)
            {
                bool isMoveable = false;
                std::shared_ptr<Material> sphereMaterial;
                if (chooseMaterial < 0.8)
                {
                    XYZ albedo = XYZ(randomDouble() * randomDouble(), randomDouble() * randomDouble(), randomDouble() * randomDouble());
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    isMoveable = true;
                }
                else if (chooseMaterial < 0.95)
                {
                    XYZ albedo = XYZ(randomDouble(0.5, 1.0), randomDouble(0.5, 1.0), randomDouble(0.5, 1.0));
                    double fuzz = randomDouble(0.0, 0.5);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                }
                else
                {
                    sphereMaterial = std::make_shared<Dielectric>(1.5);
                }

                std::shared_ptr<Sphere> pSphere = std::make_shared<Sphere>(center, 0.2);
                pSphere->setMaterial(sphereMaterial);
                if (isMoveable)
                {
                    XYZ endCenterPos = center + XYZ(0.0, randomDouble(0.0, 0.5), 0.0);
                    pSphere->setMoveInfo(endCenterPos, 0.0, 1.0);
                }
                hittableList.add(pSphere);
            }
        }
    }

    hittableList.add(std::make_shared<Sphere>(XYZ(0.0, 1.0, 0.0), 1.0)).setMaterial(std::make_shared<Dielectric>(1.5));
    hittableList.add(std::make_shared<Sphere>(XYZ(-4.0, 1.0, 0.0), 1.0)).setMaterial(std::make_shared<Lambertian>(XYZ(0.4, 0.2, 0.1)));
    hittableList.add(std::make_shared<Sphere>(XYZ(4.0, 1.0, 0.0), 1.0)).setMaterial(std::make_shared<Metal>(XYZ(0.7, 0.6, 0.5), 0.0));

    // Camera
    XYZ lookFrom = XYZ(13.0, 2.0, 3.0);
    XYZ lookAt = XYZ(0.0, 0.0, 0.0);
    XYZ vup = XYZ(0.0, 1.0, 0.0);
    double distToFocus = 10.0;
    double aperture = 0.1;

    constexpr int imageWidth = 400;
    constexpr int imageHeight = 225;
    constexpr double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    Camera camera(lookFrom, lookAt, vup, aperture, distToFocus, 20.0, aspectRatio);
    camera.setShutterTime(0.0, 1.0);

    // Init example and run
    ExampleCheckerTexture example(imageWidth, imageHeight);
    example.setSampleTimes(100);
    example.setMaxRecursiveDepth(50);
    example.process(camera, hittableList);
    example.generate("test.png");

    return 0;
}