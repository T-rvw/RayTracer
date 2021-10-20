#include "HitRecord.h"
#include "Box.h"
#include "Rect.h"

Box::Box(XYZ p0, XYZ p1) :
    m_p0(std::move(p0)),
	m_p1(std::move(p1))
{
    double x0 = m_p0.x();
    double y0 = m_p0.y();
    double z0 = m_p0.z();

    double x1 = m_p1.x();
    double y1 = m_p1.y();
    double z1 = m_p1.z();

    m_hittableList.add(std::make_shared<Rect>(XYZ(x0, y0, z0), XYZ(x1, y0, z0), XYZ(x0, y1, z0)));
    m_hittableList.add(std::make_shared<Rect>(XYZ(x0, y0, z1), XYZ(x1, y0, z1), XYZ(x0, y1, z1)));
    m_hittableList.add(std::make_shared<Rect>(XYZ(x0, y0, z0), XYZ(x0, y0, z1), XYZ(x0, y1, z0)));
    m_hittableList.add(std::make_shared<Rect>(XYZ(x1, y0, z0), XYZ(x1, y0, z1), XYZ(x1, y1, z0)));
    m_hittableList.add(std::make_shared<Rect>(XYZ(x0, y0, z0), XYZ(x0, y0, z1), XYZ(x1, y0, z0)));
    m_hittableList.add(std::make_shared<Rect>(XYZ(x0, y1, z0), XYZ(x0, y1, z1), XYZ(x1, y1, z0)));
}

std::optional<HitRecord> Box::hit(const Ray& ray, double minT, double maxT) const
{
    return m_hittableList.hit(ray, minT, maxT);
}

UV Box::uv(const XYZ& /*point*/) const
{
    // UV coordinates is calculated by Rect
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