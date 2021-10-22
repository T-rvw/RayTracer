#pragma once

#include "Texture.h"

class SolidColor final : public Texture
{
public:
    SolidColor() = default;
    explicit SolidColor(Color color) : m_color(std::move(color)) {}
    virtual ~SolidColor() = default;

    SolidColor(const SolidColor&) = delete;
    SolidColor(SolidColor&&) = delete;
    SolidColor& operator=(const SolidColor&) = delete;
    SolidColor& operator=(SolidColor&&) = delete;

    virtual Color value(double /*u*/, double /*v*/, const XYZ& /*p*/) const { return m_color; }

private:
    Color m_color;
};