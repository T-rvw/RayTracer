#include "Perlin.h"

constexpr int perlinPointCount = 256;

Perlin::Perlin()
{
	m_vecRanfloat.reserve(perlinPointCount);
	for(int ii = 0; ii < perlinPointCount; ++ii)
	{
		m_vecRanfloat.push_back(randomDouble());
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
		std::swap(m_vecPermX[ii], m_vecPermX[randomInt(0, ii)]);
		std::swap(m_vecPermY[ii], m_vecPermY[randomInt(0, ii)]);
		std::swap(m_vecPermZ[ii], m_vecPermZ[randomInt(0, ii)]);
	}
}

double Perlin::noise(const XYZ& p) const
{
	constexpr bool bSmoothOut = true;
	if constexpr (bSmoothOut)
	{
		double u = p.x() - std::floor(p.x());
		double v = p.y() - std::floor(p.y());
		double w = p.z() - std::floor(p.z());

		int aa = static_cast<int>(std::floor(p.x()));
		int bb = static_cast<int>(std::floor(p.y()));
		int cc = static_cast<int>(std::floor(p.z()));

		double cacheResult[2][2][2];
		for (int di = 0; di < 2; ++di)
		{
			for (int dj = 0; dj < 2; ++dj)
			{
				for (int dk = 0; dk < 2; ++dk)
				{
					int index = m_vecPermX[(aa + di) & 255] ^
								m_vecPermY[(bb + dj) & 255] ^
								m_vecPermZ[(cc + dk) & 255];

					cacheResult[di][dj][dk] = m_vecRanfloat[index];
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
					accumulation += cacheResult[ii][jj][kk] *
									(ii * u + (1 - ii) * (1 - u)) *
									(jj * v + (1 - jj) * (1 - v)) *
									(kk * w + (1 - kk) * (1 - w));
				}
			}
		}

		return accumulation;
	}
	else
	{
		int aa = static_cast<int>(4 * p.x()) & 255;
		int bb = static_cast<int>(4 * p.y()) & 255;
		int cc = static_cast<int>(4 * p.z()) & 255;

		int index = m_vecPermX[aa] ^
					m_vecPermY[bb] ^
					m_vecPermZ[cc];

		return m_vecRanfloat[index];
	}
}