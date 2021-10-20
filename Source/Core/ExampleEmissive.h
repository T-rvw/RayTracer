#pragma once

#include "ExampleBase.h"
#include "Material.h"

class ExampleEmissive : public ExampleBase
{
public:
    using ExampleBase::ExampleBase;

    virtual Color getRayColor(const Ray& ray, const HittableList& world, int depth) override
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
        {
            return Color(1.0, 1.0, 1.0);
        }

        std::optional<HitRecord> optHitRecord = world.hit(ray, DOUBLE_EPS, DOUBLE_INFINITY);
        if (!optHitRecord.has_value())
        {
            // background
            return Color(0.0, 0.0, 0.0);
        }

        Color attenuation;
        Color emitted;
        Ray scattered;
        const HitRecord& hitRecord = optHitRecord.value();
        if (const GeometryBase* pHitObject = hitRecord.hitObject())
        {
            if (std::shared_ptr<Material> pMaterial = pHitObject->material())
            {
                const XYZ& hitPoint = hitRecord.hitPoint();
                UV uv = pHitObject->uv(hitPoint);
                emitted = pMaterial->emitted(uv.x(), uv.y(), hitPoint);

                if (!pMaterial->scatter(ray, hitRecord, attenuation, scattered))
                {
                    return emitted;
                }
            }
        }

        return emitted + attenuation * getRayColor(scattered, world, depth - 1);
    }
};