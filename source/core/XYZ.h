#pragma once

#include <cmath>

#define XYZ_DOUBLE
#ifdef XYZ_DOUBLE
typedef double XYZPrecision;
#else
typedef float XYZPrecision;
#endif

class XYZ final
{
public:
    XYZ() : m_value{ 0, 0, 0 } {}
    XYZ(XYZPrecision x, XYZPrecision y, XYZPrecision z) : m_value{ x, y, z } {}

    XYZ(const XYZ&) = default;
    XYZ& operator=(const XYZ&) = default;
    XYZ(XYZ&&) = default;
    XYZ& operator=(XYZ&&) = default;
    ~XYZ() = default;
    
    XYZPrecision x() const { return m_value[0]; }
    XYZPrecision y() const { return m_value[1]; }
    XYZPrecision z() const { return m_value[2]; }

    XYZPrecision operator[](int index) const { return m_value[index]; }
    XYZPrecision& operator[](int index) { return m_value[index]; }
    XYZ operator-() const { return XYZ(-x(), -y(), -z()); }

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

    XYZ& operator*=(XYZPrecision t)
    {
        m_value[0] *= t;
        m_value[1] *= t;
        m_value[2] *= t;
        return *this;
    }

    XYZ& operator/=(XYZPrecision t)
    {
        return *this *= (1 / t);
    }

#ifdef XYZ_DOUBLE
    XYZPrecision length() const { return sqrt(lengthSquare()); }
#else
    XYZPrecision length() const { return sqrtf(lengthSquare()); }
#endif

    XYZPrecision lengthSquare() const
    { 
        return m_value[0] * m_value[0] + 
               m_value[1] * m_value[1] +
               m_value[2] * m_value[2];
    }

private:
    XYZPrecision m_value[3];
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
    return (1 / t) * lhs;
}

inline double dot(const XYZ& lhs, const XYZ& rhs)
{
    return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

inline XYZ cross(const XYZ& lhs, const XYZ& rhs)
{
    return XYZ(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
               lhs.z() * rhs.x() - lhs.x() * rhs.z(),
               lhs.x() * rhs.y() - lhs.y() * rhs.x());
}

inline XYZ unit(XYZ xyz)
{
    return xyz / xyz.length();
}