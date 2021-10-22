#pragma once

#include "GeometryBase.h"

class Rect final : public GeometryBase
{
public:
    Rect() = delete;
    explicit Rect(XYZ p0, XYZ p1, XYZ p2);
    virtual ~Rect() = default;

    Rect(const Rect& rhs) = delete;
    Rect(Rect&& rhs) = default;
    Rect& operator=(const Rect&) = delete;
    Rect& operator=(Rect&&) = default;

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    XYZ                 m_p0, m_p1, m_p2;
    XYZ                 m_normal;
    double              m_distance;

    // cahce
    XYZ                 m_p1p0;
    XYZ                 m_p2p0;
    double              m_dotP1Sqr;
    double              m_dotP2Sqr;
    double              m_dotP1P2;
    double              m_factor;
};