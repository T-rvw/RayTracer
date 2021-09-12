#pragma once

#include "Hittable.h"

#include <memory>

class Rect2D : public Hittable
{
public:
    Rect2D() = delete;
    Rect2D(double x0, double x1, double y0, double y1, double z, std::shared_ptr<Material> pMaterial = nullptr);
    virtual ~Rect2D() = default;

    //Rect2D(const Rect2D& rhs) {  }
    Rect2D(Rect2D&& rhs) = default;
    Rect2D& operator=(const Rect2D&) = default;
    Rect2D& operator=(Rect2D&&) = default;

    bool isStatic() const { return m_moveBeginTime == m_moveEndTime; }
    void setMoveInfo(XYZ endPos, double beginTime, double endTime)
    {
        m_moveEndPos = std::move(endPos);
        m_moveBeginTime = beginTime;
        m_moveEndTime = endTime;
    };

    XYZ center(double curTime = 0.0) const;
    virtual std::shared_ptr<Material> material() const override { return m_pMaterial; }

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT = -10000000.0, double maxT = +10000000.0) const override;

    virtual UV uv(const XYZ& point) const override;

private:
    double						m_x0, m_y0, m_x1, m_y1, m_z;
    XYZ                         m_moveEndPos;
    double                      m_moveBeginTime = 0.0;
    double                      m_moveEndTime = 0.0;
    std::shared_ptr<Material>   m_pMaterial;
};