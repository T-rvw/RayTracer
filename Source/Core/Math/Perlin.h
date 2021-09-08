#pragma once

#include "XYZ.h"

#include <vector>

class Perlin final
{
public:
	Perlin();
	~Perlin() = default;

	Perlin(const Perlin&) = delete;
	Perlin(Perlin&&) = delete;
	Perlin& operator=(const Perlin&) = delete;
	Perlin& operator=(Perlin&&) = delete;

	double noise(const XYZ& p) const;

private:
	std::vector<int>		m_vecPermX, m_vecPermY, m_vecPermZ;
	std::vector<double> 	m_vecRanfloat;
};