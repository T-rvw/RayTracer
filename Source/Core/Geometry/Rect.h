#pragma once

#include "Triangle.h"

class Rect final : public Triangle
{
public:
    using Triangle::Triangle;
    virtual ~Rect() = default;

    Rect(const Rect& rhs) = delete;
    Rect(Rect&& rhs) = default;
    Rect& operator=(const Rect&) = delete;
    Rect& operator=(Rect&&) = default;

    virtual bool isHit(double w0, double w1, double w2) const override;
};