#pragma once

#include "Material.h"
#include "SolidColor.h"

class DiffuseLight : public Material
{
public:
    DiffuseLight() = delete;
    DiffuseLight(Color c) : m_pEmit(new SolidColor(std::move(c))) {}
	DiffuseLight(Texture* pTexture) : m_pEmit(pTexture) {}
    virtual ~DiffuseLight() = default;

    DiffuseLight(const DiffuseLight&) = delete;
    DiffuseLight(DiffuseLight&&) = delete;
    DiffuseLight& operator=(const DiffuseLight&) = delete;
    DiffuseLight& operator=(DiffuseLight&&) = delete;

    virtual bool scatter(const Ray& /*ray*/, const HitRecord& /*hitRecord*/, Color& /*attenuation*/, Ray& /*scattered*/) const override
	{
		return false;
	}

	virtual Color emitted(double u, double v, const XYZ& p) const override
	{
		return m_pEmit->value(u, v, p);
	}

private:
    Texture* m_pEmit;
};