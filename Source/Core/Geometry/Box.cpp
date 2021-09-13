#include "HitRecord.h"
#include "AARect.h"
#include "Box.h"

#include <cmath>

Box::Box(XYZ p0, XYZ p1, std::shared_ptr<Material> pMaterial) :
    m_p0(p0),
	m_p1(p1),
    m_pMaterial(pMaterial)
{
    m_hittableList.appendOne(std::make_shared<AARect>(p0, p1, 'x', p0.x(), pMaterial));
    m_hittableList.appendOne(std::make_shared<AARect>(p0, p1, 'x', p0.x(), pMaterial));
    m_hittableList.appendOne(std::make_shared<AARect>(p0, p1, 'y', p0.y(), pMaterial));
    m_hittableList.appendOne(std::make_shared<AARect>(p0, p1, 'y', p1.y(), pMaterial));
    m_hittableList.appendOne(std::make_shared<AARect>(p0, p1, 'z', p0.z(), pMaterial));
    m_hittableList.appendOne(std::make_shared<AARect>(p0, p1, 'z', p1.z(), pMaterial));
}

std::optional<HitRecord> Box::hit(const Ray& ray, double minT, double maxT) const
{
    return m_hittableList.hit(ray, minT, maxT);
}