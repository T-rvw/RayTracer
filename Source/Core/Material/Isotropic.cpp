#pragma once

#include "HitRecord.h"
#include "Isotropic.h"

bool Isotropic::scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
{
    scattered = Ray(hitRecord.hitPoint(), randomInUnitSphere(), ray.delayTime());
    const XY& uv = hitRecord.hitPointUV();
    attenuation = m_pAlbedo->value(uv.x(), uv.y(), hitRecord.hitPoint());

    return true;
}