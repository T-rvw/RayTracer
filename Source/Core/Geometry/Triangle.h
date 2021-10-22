#pragma once

#include "GeometryBase.h"

class Triangle : public GeometryBase
{
public:
    Triangle() = delete;
    explicit Triangle(XYZ p0, XYZ p1, XYZ p2);
    virtual ~Triangle() = default;

    Triangle(const Triangle& rhs) = delete;
    Triangle(Triangle&& rhs) = default;
    Triangle& operator=(const Triangle&) = delete;
    Triangle& operator=(Triangle&&) = default;

    virtual std::optional<AABB> boundingBox(double t0, double t1) const override;
    virtual std::optional<HitRecord> hit(const Ray& ray, double minT, double maxT) const override;
    virtual UV uv(const XYZ& point) const override;

private:
    XYZ     m_p0, m_p1, m_p2;
    XYZ     m_normal;
    double  m_distance;

    // cahce
    XYZ                 m_p1p0;
    XYZ                 m_p2p0;
    double              m_dotP1Sqr;
    double              m_dotP2Sqr;
    double              m_dotP1P2;
    double              m_factor;
};