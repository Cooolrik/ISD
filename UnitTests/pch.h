// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "..\ISD\ISD_Types.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>

using namespace ISD;

// add headers that you want to pre-compile here
inline uint8 uint8_rand() { return (uint8)(rand() & 0xff); } 
inline uint16 uint16_rand() { return (rand() << 4) ^ rand(); } 
inline uint32 uint32_rand() { return uint32(uint16_rand()) << 16 | uint32(uint16_rand()); } 
inline uint64 uint64_rand() { return uint64(uint32_rand()) << 32 | uint64(uint32_rand()); } 
inline float float_rand() { return float(uint64_rand()); } 
inline double double_rand() { return double(uint64_rand()); } 

inline UUID uuid_rand() 
	{ 
	const UUID id = {uint32_rand(), uint16_rand(), uint16_rand(), { uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand() }};
	return id;
	}

//// random string
inline std::string str_rand( size_t min_len = 0 , size_t max_len = 1000 )
	{
	std::string str;
	size_t strl = (rand() % (max_len-min_len)) + min_len;
	for( size_t i = 0; i < strl; ++i )
		{
		char s = (char)((rand() % 0x60) + 0x20); // generate ASCII values in the range 0x20 - 0x7f
		str += s;
		}
	}

inline size_t capped_rand( size_t minv, size_t maxv )
	{
	return (uint64_rand() % (maxv - minv)) + minv;
	}

template<class T> T random_value();

template <class T> indexed_array<T> random_array( size_t minc = 100, size_t maxc = 1000 )
	{
	// generate a random array, either indexed or not
	std::vector<T> vals;
	size_t len = capped_rand( minc, maxc );
	for( size_t i = 0; i < len; ++i )
		{
		vals.push_back(random_value<T>());
		}

	if( random_value<bool>() )
		{
		std::vector<size_t> indices;
		size_t index_len = capped_rand( minc, maxc );
		for( size_t i = 0; i < len; ++i )
			{
			indices.push_back(capped_rand( 0, len )); // make sure all indices are actually valid
			}
		return indexed_array<T>(vals,indices);
		}
	else
		{
		return indexed_array<T>(vals);
		}
	}


#endif //PCH_H
