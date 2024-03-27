#pragma once

#include "MathUtils.h"

class XYZ final
{
public:
    XYZ() : m_value{ 0, 0, 0 } {}
    explicit XYZ(double v) : m_value{ v, v, v } {}
    explicit XYZ(double x, double y, double z) : m_value{ x, y, z } {}
    ~XYZ() = default;

    XYZ(const XYZ&) = default;
    XYZ(XYZ&&) = default;
    XYZ& operator=(const XYZ&) = default;
    XYZ& operator=(XYZ&&) = default;

    double x() const { return m_value[0]; }
    double y() const { return m_value[1]; }
    double z() const { return m_value[2]; }

    XYZ& normalize() { *this /= length(); return *this; }

    double operator[](int index) const { return m_value[index]; }
    double& operator[](int index) { return m_value[index]; }
    XYZ operator-() const { return XYZ(-x(), -y(), -z()); }

    XYZ& operator+=(double v)
    {
        m_value[0] += v;
        m_value[1] += v;
        m_value[2] += v;
        return *this;
    }

    XYZ& operator+=(const XYZ& rhs)
    {
        m_value[0] += rhs.x();
        m_value[1] += rhs.y();
        m_value[2] += rhs.z();
        return *this;
    }

    XYZ& operator-=(const XYZ& rhs)
    {
        m_value[0] -= rhs.x();
        m_value[1] -= rhs.y();
        m_value[2] -= rhs.z();
        return *this;
    }

    XYZ& operator*=(double t)
    {
        m_value[0] *= t;
        m_value[1] *= t;
        m_value[2] *= t;
        return *this;
    }

    XYZ& operator/=(double t)
    {
        return *this *= (1 / t);
    }

    XYZ& inverse()
    {
        m_value[0] = -x();
        m_value[1] = -y();
        m_value[2] = -z();
        return *this;
    }

    bool isZero() const
    {
        return std::abs(x()) < DOUBLE_EPS &&
               std::abs(y()) < DOUBLE_EPS &&
               std::abs(z()) < DOUBLE_EPS;
    }

    double length() const { return std::sqrt(lengthSquare()); }

    double lengthSquare() const
    {
        return m_value[0] * m_value[0] +
               m_value[1] * m_value[1] +
               m_value[2] * m_value[2];
    }

    static double dot(const XYZ& lhs, const XYZ& rhs)
    {
        return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
    }

    static XYZ cross(const XYZ& lhs, const XYZ& rhs)
    {
        return XYZ(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
                   lhs.z() * rhs.x() - lhs.x() * rhs.z(),
                   lhs.x() * rhs.y() - lhs.y() * rhs.x());
    }

    static XYZ random()
    {
        return XYZ(MathUtils::randomDouble(), MathUtils::randomDouble(), MathUtils::randomDouble());
    }

    static XYZ random(double min, double max)
    {
        return XYZ(MathUtils::randomDouble(min, max), MathUtils::randomDouble(min, max), MathUtils::randomDouble(min, max));
    }

private:
    double m_value[3];
};

// type alias
using Color = XYZ;

inline XYZ operator+(const XYZ& lhs, const XYZ& rhs)
{
    return XYZ(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

inline XYZ operator-(const XYZ& lhs, const XYZ& rhs)
{
    return XYZ(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

inline XYZ operator*(const XYZ& lhs, const XYZ& rhs)
{
    return XYZ(lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z());
}

inline XYZ operator*(double t, const XYZ& rhs)
{
    return XYZ(t * rhs.x(), t * rhs.y(), t * rhs.z());
}

inline XYZ operator*(const XYZ& lhs, double t)
{
    return t * lhs;
}

inline XYZ operator/(const XYZ& lhs, double t)
{
    return (1.0 / t) * lhs;
}

inline bool operator==(const XYZ& lhs, const XYZ& rhs)
{
    return std::abs(lhs.x() - rhs.x()) < DOUBLE_EPS &&
           std::abs(lhs.y() - rhs.y()) < DOUBLE_EPS &&
           std::abs(lhs.z() - rhs.z()) < DOUBLE_EPS;
}

inline XYZ randomInUnitSphere()
{
    while (true)
    {
        XYZ p = XYZ::random(-1.0, 1.0);
        if (p.lengthSquare() < 1.0)
        {
            return p;
        }
    }
}

inline XYZ randomInUnitDisk()
{
    while (true)
    {
        XYZ p = XYZ(MathUtils::randomDouble(-1.0, 1.0), MathUtils::randomDouble(-1.0, 1.0), 0.0);
        if (p.lengthSquare() < 1.0)
        {
            return p;
        }
    }
}

inline XYZ reflect(const XYZ& rayIn, const XYZ& normal)
{
    return rayIn - XYZ::dot(rayIn, normal) * 2 * normal;
}

inline XYZ refract(const XYZ& uv, const XYZ& normal, double etaiOverEtat)
{
    double cosTheta = fmin(XYZ::dot(-uv, normal), 1.0);
    XYZ rayOutPerp = etaiOverEtat * (uv + cosTheta * normal);
    XYZ rayOutParallel = -std::sqrt(fabs(1.0 - rayOutPerp.lengthSquare())) * normal;
    return rayOutPerp + rayOutParallel;
}
