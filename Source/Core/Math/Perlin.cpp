#include "Perlin.h"

constexpr int perlinPointCount = 256;

Perlin::Perlin()
{
	m_vecRandom.reserve(perlinPointCount);
	for(int ii = 0; ii < perlinPointCount; ++ii)
	{
		m_vecRandom.push_back(XYZ::random());
	}
	
	m_vecPermX.reserve(perlinPointCount);
	m_vecPermY.reserve(perlinPointCount);
	m_vecPermZ.reserve(perlinPointCount);
	for(int ii = 0; ii < perlinPointCount; ++ii)
	{
		m_vecPermX.push_back(ii);
		m_vecPermY.push_back(ii);
		m_vecPermZ.push_back(ii);
	}
	
	for(int ii = perlinPointCount - 1; ii > 0 ; --ii)
	{
		std::swap(m_vecPermX[ii], m_vecPermX[MathUtils::randomInt(0, ii)]);
		std::swap(m_vecPermY[ii], m_vecPermY[MathUtils::randomInt(0, ii)]);
		std::swap(m_vecPermZ[ii], m_vecPermZ[MathUtils::randomInt(0, ii)]);
	}
}

double Perlin::noise(const XYZ& p) const
{
	double u = p.x() - std::floor(p.x());
	double v = p.y() - std::floor(p.y());
	double w = p.z() - std::floor(p.z());

	constexpr bool bHermitianSmooth = true;
	if constexpr (bHermitianSmooth)
	{
		u = u * u * (3 - 2 * u);
		v = v * v * (3 - 2 * v);
		w = w * w * (3 - 2 * w);
	}

	int aa = static_cast<int>(std::floor(p.x()));
	int bb = static_cast<int>(std::floor(p.y()));
	int cc = static_cast<int>(std::floor(p.z()));

	XYZ cacheResult[2][2][2];
	for (int di = 0; di < 2; ++di)
	{
		for (int dj = 0; dj < 2; ++dj)
		{
			for (int dk = 0; dk < 2; ++dk)
			{
				int index = m_vecPermX[(aa + di) & 255] ^
							m_vecPermY[(bb + dj) & 255] ^
							m_vecPermZ[(cc + dk) & 255];

				cacheResult[di][dj][dk] = m_vecRandom[index];
			}
		}
	}

	// Trilinear interpolation
	double accumulation = 0.0;
	for (int ii = 0; ii < 2; ++ii)
	{
		for (int jj = 0; jj < 2; ++jj)
		{
			for (int kk = 0; kk < 2; ++kk)
			{
				XYZ weight(u - ii, v - jj, w - kk);
				accumulation += XYZ::dot(cacheResult[ii][jj][kk], weight) *
								(ii * u + (1 - ii) * (1 - u)) *
								(jj * v + (1 - jj) * (1 - v)) *
								(kk * w + (1 - kk) * (1 - w));
			}
		}
	}

	return accumulation;
}

double Perlin::trubulence(const XYZ& p, int depth) const
{
	double accumulation = 0.0;
	double weight = 1.0;
	XYZ originPoint = p;
	
	for(int ii = 0; ii < depth; ++ii)
	{
		accumulation += weight * noise(p);
		weight *= 0.5;
		originPoint *= 2;
	}
	
	return std::fabs(accumulation);
}