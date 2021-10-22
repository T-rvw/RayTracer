#pragma once

#include "Material.h"
#include "SolidColor.h"

class Isotropic final : public Material
{
public:
    Isotropic() = delete;
    explicit Isotropic(Color color) : m_pAlbedo(std::make_shared<SolidColor>(color)) {}
    explicit Isotropic(std::shared_ptr<Texture> pTexture) : m_pAlbedo(pTexture) {}
    virtual ~Isotropic() = default;

    Isotropic(const Isotropic&) = delete;
    Isotropic(Isotropic&&) = delete;
    Isotropic& operator=(const Isotropic&) = delete;
    Isotropic& operator=(Isotropic&&) = delete;
	
    virtual bool scatter(const Ray& ray, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override;

private:
    std::shared_ptr<Texture> m_pAlbedo;
};