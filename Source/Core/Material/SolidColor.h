#pragma once

#include "Texture.h"

class SolidColor : public Texture
{
public:
    SolidColor() = default;
    SolidColor(Color color) : m_color(color) {}
    virtual ~SolidColor() = default;

    SolidColor(const SolidColor&) = delete;
    SolidColor(SolidColor&&) = delete;
    SolidColor& operator=(const SolidColor&) = delete;
    SolidColor& operator=(SolidColor&&) = delete;

    virtual Color value(double /*u*/, double /*v*/, const XYZ& /*p*/) const { return m_color; }

private:
    Color m_color;
};