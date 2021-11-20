// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

template <> bool random_value<bool>() { return (bool)(rand() & 0x1); } 

template <> int8 random_value<int8>() { return uint8_rand() ; }
template <> int16 random_value<int16>() { return uint16_rand() ; }
template <> int32 random_value<int32>() { return uint32_rand() ; }
template <> int64 random_value<int64>() { return uint64_rand() ; }

template <> uint8 random_value<uint8>() { return uint8_rand() ; }
template <> uint16 random_value<uint16>() { return uint16_rand() ; }
template <> uint32 random_value<uint32>() { return uint32_rand() ; }
template <> uint64 random_value<uint64>() { return uint64_rand() ; }

template <> float random_value<float>() { return float_rand() ; }
template <> double random_value<double>() { return double_rand() ; }

template <> fvec2 random_value<fvec2>() { return fvec2( float_rand(), float_rand() ) ; }
template <> dvec2 random_value<dvec2>() { return dvec2( double_rand(), double_rand() ) ; }
template <> fvec3 random_value<fvec3>() { return fvec3( float_rand(), float_rand(), float_rand() ) ; }
template <> dvec3 random_value<dvec3>() { return dvec3( double_rand(), double_rand(), double_rand() ) ; }
template <> fvec4 random_value<fvec4>() { return fvec4( float_rand(), float_rand(), float_rand(), float_rand() ) ; }
template <> dvec4 random_value<dvec4>() { return dvec4( double_rand(), double_rand(), double_rand(), double_rand() ) ; }

template <> i8vec2 random_value<i8vec2>() { return i8vec2( uint8_rand(), uint8_rand() ) ; }
template <> i16vec2 random_value<i16vec2>() { return i16vec2( uint16_rand(), uint16_rand() ) ; }
template <> i32vec2 random_value<i32vec2>() { return i32vec2( uint32_rand(), uint32_rand() ) ; }
template <> i64vec2 random_value<i64vec2>() { return i64vec2( uint64_rand(), uint64_rand() ) ; }
template <> i8vec3 random_value<i8vec3>() { return i8vec3( uint8_rand(), uint8_rand(), uint8_rand() ) ; }
template <> i16vec3 random_value<i16vec3>() { return i16vec3( uint16_rand(), uint16_rand(), uint16_rand() ) ; }
template <> i32vec3 random_value<i32vec3>() { return i32vec3( uint32_rand(), uint32_rand(), uint32_rand() ) ; }
template <> i64vec3 random_value<i64vec3>() { return i64vec3( uint64_rand(), uint64_rand(), uint64_rand() ) ; }
template <> i8vec4 random_value<i8vec4>() { return i8vec4( uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand() ) ; }
template <> i16vec4 random_value<i16vec4>() { return i16vec4( uint16_rand(), uint16_rand(), uint16_rand(), uint16_rand() ) ; }
template <> i32vec4 random_value<i32vec4>() { return i32vec4( uint32_rand(), uint32_rand(), uint32_rand(), uint32_rand() ) ; }
template <> i64vec4 random_value<i64vec4>() { return i64vec4( uint64_rand(), uint64_rand(), uint64_rand(), uint64_rand() ) ; }

template <> u8vec2 random_value<u8vec2>() { return u8vec2( uint8_rand(), uint8_rand() ) ; }
template <> u16vec2 random_value<u16vec2>() { return u16vec2( uint16_rand(), uint16_rand() ) ; }
template <> u32vec2 random_value<u32vec2>() { return u32vec2( uint32_rand(), uint32_rand() ) ; }
template <> u64vec2 random_value<u64vec2>() { return u64vec2( uint64_rand(), uint64_rand() ) ; }
template <> u8vec3 random_value<u8vec3>() { return u8vec3( uint8_rand(), uint8_rand(), uint8_rand() ) ; }
template <> u16vec3 random_value<u16vec3>() { return u16vec3( uint16_rand(), uint16_rand(), uint16_rand() ) ; }
template <> u32vec3 random_value<u32vec3>() { return u32vec3( uint32_rand(), uint32_rand(), uint32_rand() ) ; }
template <> u64vec3 random_value<u64vec3>() { return u64vec3( uint64_rand(), uint64_rand(), uint64_rand() ) ; }
template <> u8vec4 random_value<u8vec4>() { return u8vec4( uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand() ) ; }
template <> u16vec4 random_value<u16vec4>() { return u16vec4( uint16_rand(), uint16_rand(), uint16_rand(), uint16_rand() ) ; }
template <> u32vec4 random_value<u32vec4>() { return u32vec4( uint32_rand(), uint32_rand(), uint32_rand(), uint32_rand() ) ; }
template <> u64vec4 random_value<u64vec4>() { return u64vec4( uint64_rand(), uint64_rand(), uint64_rand(), uint64_rand() ) ; }

template <> fmat2 random_value<fmat2>() { return fmat2(
	float_rand(), float_rand(),
	float_rand(), float_rand()
) ; }
template <> dmat2 random_value<dmat2>() { return dmat2(
	double_rand(), double_rand(),
	double_rand(), double_rand()
) ; }
template <> fmat3 random_value<fmat3>() { return fmat3(
	float_rand(), float_rand(), float_rand(),
	float_rand(), float_rand(), float_rand(),
	float_rand(), float_rand(), float_rand()
) ; }
template <> dmat3 random_value<dmat3>() { return dmat3(
	double_rand(), double_rand(), double_rand(),
	double_rand(), double_rand(), double_rand(),
	double_rand(), double_rand(), double_rand()
) ; }
template <> fmat4 random_value<fmat4>() { return fmat4(
	float_rand(), float_rand(), float_rand(), float_rand(),
	float_rand(), float_rand(), float_rand(), float_rand(),
	float_rand(), float_rand(), float_rand(), float_rand(),
	float_rand(), float_rand(), float_rand(), float_rand()
) ; }
template <> dmat4 random_value<dmat4>() { return dmat4(
	double_rand(), double_rand(), double_rand(), double_rand(),
	double_rand(), double_rand(), double_rand(), double_rand(),
	double_rand(), double_rand(), double_rand(), double_rand(),
	double_rand(), double_rand(), double_rand(), double_rand()
) ; }

template <> UUID random_value<UUID>() { return uuid_rand() ; }