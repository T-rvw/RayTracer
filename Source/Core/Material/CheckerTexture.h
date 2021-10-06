#pragma once

#include "SolidColor.h"

class CheckerTexture : public Texture
{
public:
    CheckerTexture() = default;

    CheckerTexture(Texture* odd, Texture* even) :
        m_pOddTexture(odd),
        m_pEvenTexture(even)
    {
    }

    CheckerTexture(Color oddColor, Color evenColor) : 
        m_pOddTexture(new SolidColor(std::move(oddColor))),
        m_pEvenTexture(new SolidColor(std::move(evenColor)))
    {
    }

    virtual ~CheckerTexture() = default;

    CheckerTexture(const CheckerTexture&) = delete;
    CheckerTexture(CheckerTexture&&) = delete;
    CheckerTexture& operator=(const CheckerTexture&) = delete;
    CheckerTexture& operator=(CheckerTexture&&) = delete;

    virtual Color value(double u, double v, const XYZ& p) const;

private:
    Texture* m_pOddTexture;
    Texture* m_pEvenTexture;
};