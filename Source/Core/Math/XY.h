#pragma once

#include "MathUtils.h"

class XY final
{
public:
    XY() : m_value{ 0, 0 } {}
    XY(double x, double y) : m_value{ x, y } {}
    ~XY() = default;

    XY(const XY&) = default;
    XY(XY&&) = default;
    XY& operator=(const XY&) = default;
    XY& operator=(XY&&) = default;

    double x() const { return m_value[0]; }
    double y() const { return m_value[1]; }

    double operator[](int index) const { return m_value[index]; }
    double& operator[](int index) { return m_value[index]; }
    XY operator-() const { return XY(-x(), -y()); }

    XY& operator+=(const XY& rhs)
    {
        m_value[0] += rhs.x();
        m_value[1] += rhs.y();
        return *this;
    }

    XY& operator-=(const XY& rhs)
    {
        m_value[0] -= rhs.x();
        m_value[1] -= rhs.y();
        return *this;
    }

    XY& operator*=(double t)
    {
        m_value[0] *= t;
        m_value[1] *= t;
        return *this;
    }

    XY& operator/=(double t)
    {
        return *this *= (1 / t);
    }

    XY& inverse()
    {
        m_value[0] = -x();
        m_value[1] = -y();
        return *this;
    }

    bool isZero() const
    {
        return std::abs(x()) < DOUBLE_EPS &&
               std::abs(y()) < DOUBLE_EPS;
    }

    double length() const { return std::sqrt(lengthSquare()); }

    double lengthSquare() const
    {
        return m_value[0] * m_value[0] +
               m_value[1] * m_value[1];
    }

    static XY random()
    {
        return XY(MathUtils::randomDouble(), MathUtils::randomDouble());
    }

    static XY random(double min, double max)
    {
        return XY(MathUtils::randomDouble(min, max), MathUtils::randomDouble(min, max));
    }

private:
    double m_value[2];
};

// type alias
using UV = XY;

inline XY operator+(const XY& lhs, const XY& rhs)
{
    return XY(lhs.x() + rhs.x(), lhs.y() + rhs.y());
}

inline XY operator-(const XY& lhs, const XY& rhs)
{
    return XY(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

inline XY operator*(const XY& lhs, const XY& rhs)
{
    return XY(lhs.x() * rhs.x(), lhs.y() * rhs.y());
}

inline XY operator*(double t, const XY& rhs)
{
    return XY(t * rhs.x(), t * rhs.y());
}

inline XY operator*(const XY& lhs, double t)
{
    return t * lhs;
}

inline XY operator/(const XY& lhs, double t)
{
    return (1 / t) * lhs;
}

inline bool operator==(const XY& lhs, const XY& rhs)
{
    return lhs.x() == rhs.x() &&
           lhs.y() == rhs.y();
}

inline double dot(const XY& lhs, const XY& rhs)
{
    return lhs.x() * rhs.x() + lhs.y() * rhs.y();
}

inline XY unit(XY xy)
{
    return xy / xy.length();
}