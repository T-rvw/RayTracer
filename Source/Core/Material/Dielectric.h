#pragma once

#include "Material.h"

class Dielectric final : public Material
{
public:
    Dielectric() = delete;
    explicit Dielectric(double refractionIndex) : m_refractionIndex(refractionIndex) {}
    virtual ~Dielectric() = default;

    Dielectric(const Dielectric&) = delete;
    Dielectric(Dielectric&&) = delete;
    Dielectric& operator=(const Dielectric&) = delete;
    Dielectric& operator=(Dielectric&&) = delete;
	
    virtual bool scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override;

private:
    double m_refractionIndex;
};