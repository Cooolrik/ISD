// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"
#include "ISD_Entity.h"

namespace ISD
	{
	class MemoryWriteStream;

	class EntityWriter
		{
		private:
			MemoryWriteStream &dstream;

		public:
			EntityWriter( MemoryWriteStream &_dstream ) : dstream( _dstream ) {}

			// VT_Bool				 
			bool Write( const char *key, const uint8 key_length, const bool &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<bool> &value );

			// VT_Int				 
			bool Write( const char *key, const uint8 key_length, const int8 &value );
			bool Write( const char *key, const uint8 key_length, const int16 &value );
			bool Write( const char *key, const uint8 key_length, const int32 &value );
			bool Write( const char *key, const uint8 key_length, const int64 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<int8> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<int16> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<int32> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<int64> &value );

			// VT_UInt				 
			bool Write( const char *key, const uint8 key_length, const uint8 &value );
			bool Write( const char *key, const uint8 key_length, const uint16 &value );
			bool Write( const char *key, const uint8 key_length, const uint32 &value );
			bool Write( const char *key, const uint8 key_length, const uint64 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<uint8> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<uint16> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<uint32> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<uint64> &value );

			// VT_Float				
			bool Write( const char *key, const uint8 key_length, const float &value );
			bool Write( const char *key, const uint8 key_length, const double &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<float> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<double> &value );

			// VT_Vec2				 
			bool Write( const char *key, const uint8 key_length, const fvec2 &value );
			bool Write( const char *key, const uint8 key_length, const dvec2 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<fvec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<dvec2> &value );

			// VT_Vec3				  
			bool Write( const char *key, const uint8 key_length, const fvec3 &value );
			bool Write( const char *key, const uint8 key_length, const dvec3 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<fvec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<dvec3> &value );

			// VT_Vec4				  
			bool Write( const char *key, const uint8 key_length, const fvec4 &value );
			bool Write( const char *key, const uint8 key_length, const dvec4 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<fvec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<dvec4> &value );

			// VT_IVec2				 
			bool Write( const char *key, const uint8 key_length, const i8vec2 &value );
			bool Write( const char *key, const uint8 key_length, const i16vec2 &value );
			bool Write( const char *key, const uint8 key_length, const i32vec2 &value );
			bool Write( const char *key, const uint8 key_length, const i64vec2 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i8vec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i16vec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i32vec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i64vec2> &value );

			// VT_IVec3				 
			bool Write( const char *key, const uint8 key_length, const i8vec3 &value );
			bool Write( const char *key, const uint8 key_length, const i16vec3 &value );
			bool Write( const char *key, const uint8 key_length, const i32vec3 &value );
			bool Write( const char *key, const uint8 key_length, const i64vec3 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i8vec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i16vec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i32vec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i64vec3> &value );

			// VT_IVec4					   
			bool Write( const char *key, const uint8 key_length, const i8vec4 &value );
			bool Write( const char *key, const uint8 key_length, const i16vec4 &value );
			bool Write( const char *key, const uint8 key_length, const i32vec4 &value );
			bool Write( const char *key, const uint8 key_length, const i64vec4 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i8vec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i16vec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i32vec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<i64vec4> &value );

			// VT_UVec2				   
			bool Write( const char *key, const uint8 key_length, const u8vec2 &value );
			bool Write( const char *key, const uint8 key_length, const u16vec2 &value );
			bool Write( const char *key, const uint8 key_length, const u32vec2 &value );
			bool Write( const char *key, const uint8 key_length, const u64vec2 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u8vec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u16vec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u32vec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u64vec2> &value );

			// VT_UVec3				   
			bool Write( const char *key, const uint8 key_length, const u8vec3 &value );
			bool Write( const char *key, const uint8 key_length, const u16vec3 &value );
			bool Write( const char *key, const uint8 key_length, const u32vec3 &value );
			bool Write( const char *key, const uint8 key_length, const u64vec3 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u8vec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u16vec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u32vec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u64vec3> &value );

			// VT_UVec4				   
			bool Write( const char *key, const uint8 key_length, const u8vec4 &value );
			bool Write( const char *key, const uint8 key_length, const u16vec4 &value );
			bool Write( const char *key, const uint8 key_length, const u32vec4 &value );
			bool Write( const char *key, const uint8 key_length, const u64vec4 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u8vec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u16vec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u32vec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<u64vec4> &value );

			// VT_Mat2				   
			bool Write( const char *key, const uint8 key_length, const fmat2 &value );
			bool Write( const char *key, const uint8 key_length, const dmat2 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<fmat2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<dmat2> &value );

			// VT_Mat3				   
			bool Write( const char *key, const uint8 key_length, const fmat3 &value );
			bool Write( const char *key, const uint8 key_length, const dmat3 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<fmat3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<dmat3> &value );

			// VT_Mat4				   
			bool Write( const char *key, const uint8 key_length, const fmat4 &value );
			bool Write( const char *key, const uint8 key_length, const dmat4 &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<fmat4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<dmat4> &value );

			// VT_UUID				   
			bool Write( const char *key, const uint8 key_length, const UUID &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<UUID> &value );

			// VT_Array_Bool			
			bool Write( const char *key, const uint8 key_length, const indexed_array<bool> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<bool>> &value );

			// VT_Array_Int				 
			bool Write( const char *key, const uint8 key_length, const indexed_array<int8> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<int16> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<int32> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<int64> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<int8>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<int16>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<int32>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<int64>> &value );

			// VT_Array_UInt				 
			bool Write( const char *key, const uint8 key_length, const indexed_array<uint8> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<uint16> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<uint32> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<uint64> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<uint8>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<uint16>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<uint32>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<uint64>> &value );

			// VT_Array_Float				
			bool Write( const char *key, const uint8 key_length, const indexed_array<float> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<double> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<float>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<double>> &value );

			// VT_Array_Vec2				 
			bool Write( const char *key, const uint8 key_length, const indexed_array<fvec2> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<dvec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<fvec2>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<dvec2>> &value );

			// VT_Array_Vec3				  
			bool Write( const char *key, const uint8 key_length, const indexed_array<fvec3> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<dvec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<fvec3>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<dvec3>> &value );

			// VT_Array_Vec4				  
			bool Write( const char *key, const uint8 key_length, const indexed_array<fvec4> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<dvec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<fvec4>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<dvec4>> &value );

			// VT_Array_IVec2				 
			bool Write( const char *key, const uint8 key_length, const indexed_array<i8vec2> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i16vec2> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i32vec2> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i64vec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i8vec2>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i16vec2>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i32vec2>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i64vec2>> &value );

			// VT_Array_IVec3				 
			bool Write( const char *key, const uint8 key_length, const indexed_array<i8vec3> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i16vec3> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i32vec3> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i64vec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i8vec3>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i16vec3>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i32vec3>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i64vec3>> &value );

			// VT_Array_IVec4					   
			bool Write( const char *key, const uint8 key_length, const indexed_array<i8vec4> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i16vec4> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i32vec4> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<i64vec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i8vec4>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i16vec4>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i32vec4>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<i64vec4>> &value );

			// VT_Array_UVec2				   
			bool Write( const char *key, const uint8 key_length, const indexed_array<u8vec2> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u16vec2> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u32vec2> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u64vec2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u8vec2>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u16vec2>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u32vec2>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u64vec2>> &value );

			// VT_Array_UVec3				   
			bool Write( const char *key, const uint8 key_length, const indexed_array<u8vec3> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u16vec3> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u32vec3> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u64vec3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u8vec3>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u16vec3>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u32vec3>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u64vec3>> &value );

			// VT_Array_UVec4				   
			bool Write( const char *key, const uint8 key_length, const indexed_array<u8vec4> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u16vec4> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u32vec4> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<u64vec4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u8vec4>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u16vec4>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u32vec4>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<u64vec4>> &value );

			// VT_Array_Mat2				   
			bool Write( const char *key, const uint8 key_length, const indexed_array<fmat2> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<dmat2> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<fmat2>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<dmat2>> &value );

			// VT_Array_Mat3				   
			bool Write( const char *key, const uint8 key_length, const indexed_array<fmat3> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<dmat3> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<fmat3>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<dmat3>> &value );

			// VT_Array_Mat4				   
			bool Write( const char *key, const uint8 key_length, const indexed_array<fmat4> &value );
			bool Write( const char *key, const uint8 key_length, const indexed_array<dmat4> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<fmat4>> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<dmat4>> &value );

			// VT_Array_UUID				   
			bool Write( const char *key, const uint8 key_length, const indexed_array<UUID> &value );
			bool Write( const char *key, const uint8 key_length, const optional_value<indexed_array<UUID>> &value );


		};
	};
