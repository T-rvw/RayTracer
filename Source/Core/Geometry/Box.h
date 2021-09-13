#pragma once

#include "Hittable.h"
#include "HittableList.h"

#include <memory>

class Box : public Hittable
{
public:
    Box() = delete;
    Box(XYZ p0, XYZ p1, std::shared_ptr<Material> pMaterial = nullptr);
    virtual ~Box() = default;

    //AARect(const AARect& rhs) {  }
    Box(Box&& rhs) = default;
    Box& operator=(const Box&) = default;
    Box& operator=(Box&&) = default;

    bool isStatic() const { return m_moveBeginTime == m_moveEndTime; }
    void setMoveInfo(XYZ endPos, double beginTime, double endTime)
    {
        m_moveEndPos = std::move(endPos);
        m_moveBeginTime = beginTime;
        m_moveEndTime = endTime;
    };

    virtual std::shared_ptr<Material> material() const { return m_pMaterial; }

    virtual std::optional<HitRecord> hit(const Ray& ray, double minT = -10000000.0, double maxT = +10000000.0) const override;

private:
    XYZ						    m_p0, m_p1;
    HittableList                m_hittableList;

    XYZ                         m_moveEndPos;
    double                      m_moveBeginTime = 0.0;
    double                      m_moveEndTime = 0.0;

    std::shared_ptr<Material>   m_pMaterial;
};