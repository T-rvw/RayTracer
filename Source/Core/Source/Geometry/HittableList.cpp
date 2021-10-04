#include "HittableList.h"

std::optional<HitRecord> HittableList::hit(const Ray& ray, double minT, double maxT) const
{
    bool hitAnything = false;
    double closestT = maxT;
    std::optional<HitRecord> finalResult = std::nullopt;

    for (const auto& pHittableObject : m_vecHittableObjects)
    {
        std::optional<HitRecord> tempResult = pHittableObject->hit(ray, minT, closestT);
        if (tempResult.has_value())
        {
            hitAnything = true;
            closestT = tempResult.value().rayT();
            finalResult = tempResult.value();
        }
    }

    return finalResult;
}

std::optional<AABB> HittableList::boundingBox(double t0, double t1) const
{
    std::optional<AABB> optOutputBox;
    for (const auto& pHittableObject : m_vecHittableObjects)
    {
        std::optional<AABB> optBox = pHittableObject->boundingBox(t0, t1);
        if (!optBox.has_value())
        {
            break;
        }

        if (!optOutputBox.has_value())
        {
            optOutputBox = std::move(optBox);
        }
        else
        {
            optOutputBox = AABB::merge(optOutputBox.value(), optBox.value());
        }
    }

    return optOutputBox;
}