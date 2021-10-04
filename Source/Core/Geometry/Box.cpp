#include "HitRecord.h"
#include "AARect.h"
#include "Box.h"

Box::Box(XYZ p0, XYZ p1) :
    m_p0(p0),
	m_p1(p1)
{
    m_hittableList.add(std::make_shared<AARect>(p0, p1, 'x', p0.x()));
    m_hittableList.add(std::make_shared<AARect>(p0, p1, 'x', p0.x()));
    m_hittableList.add(std::make_shared<AARect>(p0, p1, 'y', p0.y()));
    m_hittableList.add(std::make_shared<AARect>(p0, p1, 'y', p1.y()));
    m_hittableList.add(std::make_shared<AARect>(p0, p1, 'z', p0.z()));
    m_hittableList.add(std::make_shared<AARect>(p0, p1, 'z', p1.z()));
}

std::optional<HitRecord> Box::hit(const Ray& ray, double minT, double maxT) const
{
    return m_hittableList.hit(ray, minT, maxT);
}

UV Box::uv(const XYZ& /*point*/) const
{
    // UV coordinates is calculated by AARect
    return UV(0.0, 0.0);
}

std::optional<AABB> Box::boundingBox(double /*t0*/, double /*t1*/) const
{
    return AABB(m_p0, m_p1);
}

void Box::setMaterial(std::shared_ptr<Material> pMaterial)
{
    for (int ii = 0; ii < m_hittableList.size(); ++ii)
    {
        if (m_hittableList[ii])
        {
            m_hittableList[ii]->setMaterial(pMaterial);
        }
    }
}