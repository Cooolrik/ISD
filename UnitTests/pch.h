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

const i64 global_random_seed = 12876123876;
const size_t global_number_of_passes = 1;

inline void setup_random_seed()
	{
	i64 seed = (global_random_seed == -1) ? i64( time( nullptr ) ) : global_random_seed;
	srand(uint( seed & 0xffffffff ));
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
	size_t strl = (rand() % (max_len-min_len)) + min_len;
	std::string str;
	str.resize( strl );
	for( size_t i = 0; i < strl; ++i )
		{
		str[i] = (char)((rand() % 0x60) + 0x20); // generate ASCII values in the range 0x20 - 0x7f
		}
	return str;
	}

inline size_t capped_rand( size_t minv, size_t maxv )
	{
	return (u64_rand() % (maxv - minv)) + minv;
	}

template<class T> T random_value();

template<class T> optional_value<T> random_optional_value()
	{
	optional_value<T> val;
	if( random_value<bool>() )
		{
		val.set( random_value<T>() );
		}
	return val;
	}

template <class T> void random_vector( std::vector<T> &dest , size_t minc = 10, size_t maxc = 1000 )
	{
	size_t len = capped_rand( minc, maxc );
	dest.resize(len);
	for( size_t i = 0; i < len; ++i )
		{
		dest[i] = random_value<T>();
		}
	}

template<class T> void random_optional_vector( optional_vector<T> &dest, size_t minc = 10, size_t maxc = 1000 )
	{
	if( random_value<bool>() )
		{
		dest.set();
		random_vector<T>( dest.vector(), minc, maxc );
		}
	else
		{
		dest.reset();
		}
	}

template <class T> void random_idx_vector( idx_vector<T> &dest, size_t minc = 10, size_t maxc = 1000 )
	{
	// generate a random indexed array
	random_vector<T>( dest.values(), minc, maxc );
	i32 index_maxval = (i32)dest.values().size();
	auto indices = dest.index();
	if( index_maxval > 0 )
		{
		size_t index_len = capped_rand( minc, maxc );
		indices.resize( index_len );
		for( size_t i = 0; i < index_len; ++i )
			{
			indices[i] = (i32)capped_rand( 0, index_maxval ); // make sure all indices are actually valid
			}
		}
	else
		{
		indices.clear();
		}
	}

template<class T> void random_optional_idx_vector( optional_idx_vector<T> &dest, size_t minc = 10, size_t maxc = 1000 )
	{
	if( random_value<bool>() )
		{
		dest.set();
		random_idx_vector<T>( dest.vector(), minc, maxc );
		}
	else
		{
		dest.reset();
		}
	}

#endif //PCH_H
