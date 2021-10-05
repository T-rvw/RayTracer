#pragma once

#include "Material.h"

class Metal : public Material
{
public:
    Metal() = delete;
    Metal(Color albedo, double fuzz = 0.0) : m_albedo(std::move(albedo)), m_fuzz(fuzz) {}
    virtual ~Metal() = default;

    Metal(const Metal&) = delete;
    Metal(Metal&&) = delete;
    Metal& operator=(const Metal&) = delete;
    Metal& operator=(Metal&&) = delete;
	
    double fuzz() const { return m_fuzz; }
    virtual bool scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override;

private:
	Color   m_albedo;
    double  m_fuzz;
};