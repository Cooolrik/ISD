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

			// VT_Null
			bool WriteNull( const char *key, const uint8 key_length );

			// VT_Bool				 
			bool Write( const char *key, const uint8 key_length, const bool &value );
									 
			// VT_Int				 
			bool Write( const char *key, const uint8 key_length, const int8 &value );
			bool Write( const char *key, const uint8 key_length, const int16 &value );
			bool Write( const char *key, const uint8 key_length, const int32 &value );
			bool Write( const char *key, const uint8 key_length, const int64 &value );
									 
			// VT_UInt				 
			bool Write( const char *key, const uint8 key_length, const uint8 &value );
			bool Write( const char *key, const uint8 key_length, const uint16 &value );
			bool Write( const char *key, const uint8 key_length, const uint32 &value );
			bool Write( const char *key, const uint8 key_length, const uint64 &value );
									
			// VT_Float				
			bool Write( const char *key, const uint8 key_length, const float &value );
			bool Write( const char *key, const uint8 key_length, const double &value );
									 
			// VT_Vec2				 
			bool Write( const char *key, const uint8 key_length, const fvec2 &value );
			bool Write( const char *key, const uint8 key_length, const dvec2 &value );
									  
			// VT_Vec3				  
			bool Write( const char *key, const uint8 key_length, const fvec3 &value );
			bool Write( const char *key, const uint8 key_length, const dvec3 &value );
									  
			// VT_Vec4				  
			bool Write( const char *key, const uint8 key_length, const fvec4 &value );
			bool Write( const char *key, const uint8 key_length, const dvec4 &value );
									 
			// VT_IVec2				 
			bool Write( const char *key, const uint8 key_length, const i8vec2 &value );
			bool Write( const char *key, const uint8 key_length, const i16vec2 &value );
			bool Write( const char *key, const uint8 key_length, const i32vec2 &value );
			bool Write( const char *key, const uint8 key_length, const i64vec2 &value );
									 
			// VT_IVec3				 
			bool Write( const char *key, const uint8 key_length, const i8vec3 &value );
			bool Write( const char *key, const uint8 key_length, const i16vec3 &value );
			bool Write( const char *key, const uint8 key_length, const i32vec3 &value );
			bool Write( const char *key, const uint8 key_length, const i64vec3 &value );
										   
			// VT_IVec4					   
			bool Write( const char *key, const uint8 key_length, const i8vec4 &value );
			bool Write( const char *key, const uint8 key_length, const i16vec4 &value );
			bool Write( const char *key, const uint8 key_length, const i32vec4 &value );
			bool Write( const char *key, const uint8 key_length, const i64vec4 &value );
									   
			// VT_UVec2				   
			bool Write( const char *key, const uint8 key_length, const u8vec2 &value );
			bool Write( const char *key, const uint8 key_length, const u16vec2 &value );
			bool Write( const char *key, const uint8 key_length, const u32vec2 &value );
			bool Write( const char *key, const uint8 key_length, const u64vec2 &value );
									   
			// VT_UVec3				   
			bool Write( const char *key, const uint8 key_length, const u8vec3 &value );
			bool Write( const char *key, const uint8 key_length, const u16vec3 &value );
			bool Write( const char *key, const uint8 key_length, const u32vec3 &value );
			bool Write( const char *key, const uint8 key_length, const u64vec3 &value );
									   
			// VT_UVec4				   
			bool Write( const char *key, const uint8 key_length, const u8vec4 &value );
			bool Write( const char *key, const uint8 key_length, const u16vec4 &value );
			bool Write( const char *key, const uint8 key_length, const u32vec4 &value );
			bool Write( const char *key, const uint8 key_length, const u64vec4 &value );
									   
			// VT_Mat2				   
			bool Write( const char *key, const uint8 key_length, const fmat2 &value );
			bool Write( const char *key, const uint8 key_length, const dmat2 &value );
									   
			// VT_Mat3				   
			bool Write( const char *key, const uint8 key_length, const fmat3 &value );
			bool Write( const char *key, const uint8 key_length, const dmat3 &value );
									   
			// VT_Mat4				   
			bool Write( const char *key, const uint8 key_length, const fmat4 &value );
			bool Write( const char *key, const uint8 key_length, const dmat4 &value );
									   
			// VT_UUID				   
			bool Write( const char *key, const uint8 key_length, const UUID &value );
		};
	};
