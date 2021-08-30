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