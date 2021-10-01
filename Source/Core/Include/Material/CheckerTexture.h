#pragma once

#include "SolidColor.h"

#include <memory>

class CheckerTexture : public Texture
{
public:
    CheckerTexture() = default;

    CheckerTexture(std::shared_ptr<Texture> odd, std::shared_ptr<Texture> even) :
        m_pOddTexture(odd),
        m_pEvenTexture(even)
    {
    }

    CheckerTexture(Color oddColor, Color evenColor) : 
        m_pOddTexture(std::make_shared<SolidColor>(std::move(oddColor))),
        m_pEvenTexture(std::make_shared<SolidColor>(std::move(evenColor)))
    {
    }

    virtual ~CheckerTexture() = default;

    CheckerTexture(const CheckerTexture&) = delete;
    CheckerTexture(CheckerTexture&&) = delete;
    CheckerTexture& operator=(const CheckerTexture&) = delete;
    CheckerTexture& operator=(CheckerTexture&&) = delete;

    virtual Color value(double u, double v, const XYZ& p) const;

private:
    std::shared_ptr<Texture> m_pOddTexture;
    std::shared_ptr<Texture> m_pEvenTexture;
};