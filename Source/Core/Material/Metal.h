#pragma once

#include "Material.h"

class Metal : public Material
{
public:
    Metal() = delete;
    Metal(Color albedo) : m_albedo(albedo) {}
    virtual ~Metal() = default;

    Metal(const Metal&) = delete;
    Metal(Metal&&) = delete;
    Metal& operator=(const Metal&) = delete;
    Metal& operator=(Metal&&) = delete;
	
    virtual bool scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override;

private:
	Color m_albedo;
};