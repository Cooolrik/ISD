// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"
#include "ISD_Entity.h"

namespace ISD
	{
	class MemoryReadStream;

	class EntityReader
		{
		private:
			MemoryReadStream &sstream;

		public:
			EntityReader( MemoryReadStream &_sstream ) : sstream( _sstream ) {}

			// VT_Null
			bool ReadNull( const char *key, const uint8 key_length );

			// VT_Bool				 
			bool Read( const char *key, const uint8 key_length, bool &dest_variable );

			// VT_Int				 
			bool Read( const char *key, const uint8 key_length, int8 &dest_variable );
			bool Read( const char *key, const uint8 key_length, int16 &dest_variable );
			bool Read( const char *key, const uint8 key_length, int32 &dest_variable );
			bool Read( const char *key, const uint8 key_length, int64 &dest_variable );

			// VT_UInt				 
			bool Read( const char *key, const uint8 key_length, uint8 &dest_variable );
			bool Read( const char *key, const uint8 key_length, uint16 &dest_variable );
			bool Read( const char *key, const uint8 key_length, uint32 &dest_variable );
			bool Read( const char *key, const uint8 key_length, uint64 &dest_variable );

			// VT_Float				
			bool Read( const char *key, const uint8 key_length, float &dest_variable );
			bool Read( const char *key, const uint8 key_length, double &dest_variable );

			// VT_Vec2				 
			bool Read( const char *key, const uint8 key_length, fvec2 &dest_variable );
			bool Read( const char *key, const uint8 key_length, dvec2 &dest_variable );

			// VT_Vec3				  
			bool Read( const char *key, const uint8 key_length, fvec3 &dest_variable );
			bool Read( const char *key, const uint8 key_length, dvec3 &dest_variable );

			// VT_Vec4				  
			bool Read( const char *key, const uint8 key_length, fvec4 &dest_variable );
			bool Read( const char *key, const uint8 key_length, dvec4 &dest_variable );

			// VT_IVec2				 
			bool Read( const char *key, const uint8 key_length, i8vec2 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i16vec2 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i32vec2 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i64vec2 &dest_variable );

			// VT_IVec3				 
			bool Read( const char *key, const uint8 key_length, i8vec3 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i16vec3 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i32vec3 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i64vec3 &dest_variable );

			// VT_IVec4					   
			bool Read( const char *key, const uint8 key_length, i8vec4 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i16vec4 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i32vec4 &dest_variable );
			bool Read( const char *key, const uint8 key_length, i64vec4 &dest_variable );

			// VT_UVec2				   
			bool Read( const char *key, const uint8 key_length, u8vec2 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u16vec2 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u32vec2 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u64vec2 &dest_variable );

			// VT_UVec3				   
			bool Read( const char *key, const uint8 key_length, u8vec3 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u16vec3 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u32vec3 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u64vec3 &dest_variable );

			// VT_UVec4				   
			bool Read( const char *key, const uint8 key_length, u8vec4 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u16vec4 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u32vec4 &dest_variable );
			bool Read( const char *key, const uint8 key_length, u64vec4 &dest_variable );

			// VT_Mat2				   
			bool Read( const char *key, const uint8 key_length, fmat2 &dest_variable );
			bool Read( const char *key, const uint8 key_length, dmat2 &dest_variable );

			// VT_Mat3				   
			bool Read( const char *key, const uint8 key_length, fmat3 &dest_variable );
			bool Read( const char *key, const uint8 key_length, dmat3 &dest_variable );

			// VT_Mat4				   
			bool Read( const char *key, const uint8 key_length, fmat4 &dest_variable );
			bool Read( const char *key, const uint8 key_length, dmat4 &dest_variable );

			// VT_UUID				   
			bool Read( const char *key, const uint8 key_length, UUID &dest_variable );
		};
	};
