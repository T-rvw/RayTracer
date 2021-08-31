#pragma once

#include "Material.h"

class Lambertian : public Material
{
public:
    Lambertian() = delete;
	Lambertian(Color albedo) : m_albedo(albedo) {} 
    virtual ~Lambertian() = default;

    Lambertian(const Lambertian&) = delete;
    Lambertian(Lambertian&&) = delete;
    Lambertian& operator=(const Lambertian&) = delete;
    Lambertian& operator=(Lambertian&&) = delete;
	
    virtual bool scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override;

private:
	Color m_albedo;
};