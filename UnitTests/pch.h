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

inline void random_seed( i64 seed = -1 )
	{
	if( seed == -1 )
		{
		seed = i64(time( nullptr ));
		srand(uint( seed & 0xffffffff ));
		}
	}


// add headers that you want to pre-compile here
inline u8 u8_rand() { return (u8)(rand() & 0xff); } 
inline u16 u16_rand() { return (rand() << 4) ^ rand(); } 
inline u32 u32_rand() { return u32(u16_rand()) << 16 | u32(u16_rand()); } 
inline u64 u64_rand() { return u64(u32_rand()) << 32 | u64(u32_rand()); } 
inline float float_rand() { return float(u64_rand()); } 
inline double double_rand() { return double(u64_rand()); } 

inline UUID uuid_rand() 
	{ 
	const UUID id = {u32_rand(), u16_rand(), u16_rand(), { u8_rand(), u8_rand(), u8_rand(), u8_rand(), u8_rand(), u8_rand(), u8_rand(), u8_rand() }};
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
	return (u64_rand() % (maxv - minv)) + minv;
	}

template<class T> T random_value();

template<class T> optional_value<T> optional_random_value()
	{
	optional_value<T> val;
	if( random_value<bool>() )
		{
		val.set( random_value<T>() );
		}
	return val;
	}

template <class T> std::vector<T> random_vector( size_t minc = 10, size_t maxc = 1000 )
	{
	size_t len = capped_rand( minc, maxc );
	std::vector<T> vals(len);
	for( size_t i = 0; i < len; ++i )
		{
		vals[i] = random_value<T>();
		}
	return vals;
	}

template<class T> optional_value<std::vector<T>> optional_random_vector()
	{
	optional_value<std::vector<T>> val;
	if( random_value<bool>() )
		{
		val.set( random_vector<T>() );
		}
	return val;
	}

template <class T> indexed_array<T> random_indexed_array( size_t minc = 10, size_t maxc = 1000 )
	{
	// generate a random indexed array
	std::vector<T> vals = random_vector<T>( minc, maxc );
	size_t len = vals.size();

	size_t index_len = capped_rand( minc, maxc );
	std::vector<size_t> indices(index_len);
	for( size_t i = 0; i < index_len; ++i )
		{
		indices[i] = capped_rand( 0, len ); // make sure all indices are actually valid
		}
	return indexed_array<T>(vals,indices);
	}

template<class T> optional_value<indexed_array<T>> optional_random_indexed_array()
	{
	optional_value<indexed_array<T>> val;
	if( random_value<bool>() )
		{
		val.set( random_indexed_array<T>() );
		}
	return val;
	}

#endif //PCH_H
