#include <cstdint>

#ifndef __NOISE_H__92374092374021784309287349023874__
#define __NOISE_H__92374092374021784309287349023874__

namespace ktools
{
	static double noise1D(uint32_t x, uint64_t Seed=15731) {
		x = (x<<13) ^ x;
		return (1.0 - ( (x * (x * x * Seed + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
	}

	static double noiseNormal(uint32_t x, uint64_t Seed=15731) {
		x = (x<<13) ^ x;
		return (1.0 - ( (x * (x * x * Seed + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0) *.5 + .5f;    
	}

	static double noise2D(uint32_t x, uint32_t y, uint32_t nWidth, uint64_t Seed=15731 ) {
		x += (y * nWidth);
		x = (x<<13) ^ x;
		return ( 1.0 - ( (x * (x * x * Seed + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
	}

	static double noise3D( uint32_t x, uint32_t y, uint32_t z, uint32_t nWidth, uint32_t nHeight, uint64_t Seed=15731 ) {
		x += (y*nWidth+(z*nHeight*nWidth));
		x = (x<<13) ^ x;
		return ( 1.0 - ( (x*(x * x * Seed + 789221) + 1376312589 ) & 0x7fffffff) / 1073741824.0);    
	}
}

#endif // __NOISE_H__92374092374021784309287349023874__