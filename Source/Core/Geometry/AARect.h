#pragma once

#include "Hittable.h"

#include <memory>

class AARect : public Hittable
{
public:
    AARect() = delete;
    AARect(XYZ p0, XYZ p1, char kIndex, double k, std::shared_ptr<Material> pMaterial = nullptr);
    virtual ~AARect() = default;

    //AARect(const AARect& rhs) {  }
    AARect(AARect&& rhs) = default;
    AARect& operator=(const AARect&) = default;
    AARect& operator=(AARect&&) = default;

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
    XYZ						    m_p0, m_p1;
    double						m_k;
    int                         m_verticalAxisIndex;
    XYZ                         m_moveEndPos;
    double                      m_moveBeginTime = 0.0;
    double                      m_moveEndTime = 0.0;
    std::shared_ptr<Material>   m_pMaterial;
};