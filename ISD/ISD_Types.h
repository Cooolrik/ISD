// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include <utility>
#include <map>
#include <unordered_map>

#include <mutex>
#include <vector>

#include "ISD_DataTypes.h"
#include "ISD_Log.h"

#define ISDErrorLog Log::Error( __func__ , __FILE__ , __LINE__ ) 
#define ISDErrorLogEnd std::endl

#define ISDValidationError( errorid ) validator.ReportError( errorid , __func__ , __FILE__ , __LINE__ ) 
#define ISDValidationErrorEnd std::endl

#define ISDRuntimeCheck( statement , errorid , errortext ) if( !(statement) ) { ISDErrorLog << "Runtime check failed: (" #statement ") error id:" << (int)errorid << ISDErrorLogEnd; throw std::exception("Sanity debug check failed: (" #statement "): Error text: " #errortext ); }

#ifdef _DEBUG
#define ISDSanityCheckDebugMacro( statement ) if( !(statement) ) { ISDErrorLog << "Sanity debug check failed: (" #statement ")" << ISDErrorLogEnd; throw std::exception("Sanity debug check failed: (" #statement ")"); }
#define ISDSanityCheckCoreDebugMacro( statement ) if( !(statement) ) { ISDErrorLog << "Core debug sanity check failed: (" #statement ")" << ISDErrorLogEnd; throw std::exception("Core debug sanity check failed: (" #statement ")"); }
#else
#define ISDSanityCheckDebugMacro( statement ) 
#define ISDSanityCheckCoreDebugMacro( statement ) 
#endif

#define ISDKeyMacro( name ) name , (u8)(strlen(name))

#include "ISD_idx_vector.h"
#include "ISD_optional_idx_vector.h"
#include "ISD_optional_value.h"
#include "ISD_optional_vector.h"
#include "ISD_thread_safe_map.h"

namespace ISD
	{
	typedef unsigned int uint;
	using std::vector;

	// maximum size of a name of a value of subchunk in the entities
	const size_t EntityMaxKeyLength = 40; 

	// status message for functions that return more than a bool status
	enum class Status
		{
		Ok = 0, // no error
		EUndefined = -1, // undefined error
		EParam = -2, // invalid parameter in method call
		ENotInitialized = -3, // the system is not initialized
		EAlreadyInitialized = -4, // the system or class is already initialized or in a specific state
		ECantAllocate = -5, // cant allocate memory
		ECantOpen = -6, // cant open file or handle
		ECantRead = -7, // cant read from file or handle
		ECorrupted = -8, // a filed is corrupted (failes sha256 test)
		EInvalid = -9, // invalid file, not an ISD file
		};

	// A Note on how types are either stored in small encoding chunks, or large encoding chunks in the binary files:
	// 
	// Small encoding chunks, for any valuetype < 0x40:
	// * Small values of fixed maximum length, where the payload + length of the key is < 256 bytes
	// * Used mainly for single items of base types: bools, ints, floats, vec2s, vec3s, vec4s, UUIDs
	// * Layout:
	//		u8 Type; // types 0x00 - 0x3f
	//		u8 SizeInBytes; // can be used to skip value if not recognized
	//		u8 Value[]; // <- defined size, based on Type (or with extra info inside, if variable size)
	//		u8 KeyData[]; // the key of the value (EntityMaxKeyLength is the max length of any key)
	// 
	// Large encoding chunks, for any valuetype >= 0x40 
	// * Uses a u64 to define size, so basically any useful size
	// * Used for:
	//		- Nested chunks
	//		- Arrays of values
	//		- Strings (UTF-8 encoded)
	//		- Data that would not fit in 256 bytes
	// * Layout:
	//		u8 Type; // types 0x40 - 0xff
	//		u64 SizeInBytes; // to skip over rest of this block, size after this value  
	//		u8 KeySizeInBytes; // the size of the key of the value (EntityMaxKeyLength is the max length of any key)
	//		u8 KeyData[]; // the key of the value 
	//		u8 Value[]; // <- defined size, equal to the rest of SizeInBytes after the key data ( sizeof(KeySizeInBytes)=1 + KeySizeInBytes bytes) 

	// reflection and serialization value types
	enum class ValueType
		{
		// -----------------------------------------------------------------------------------------------------------------------
		// --- Base value types up to 0x3f use the smaller encoding chunk, and are capped at less than 256 bytes in size

		// --- Base value types, valid range: 0x00 - 0x3f 
		VT_Bool = 0x01, // boolean value, can only be 1 byte in size
		VT_Int = 0x02, // signed integer value, can be 1, 2, 4, and 8 or in size (8, 16, 32 or 64 bits)
		VT_UInt = 0x03, // unsigned integer value, can be 1, 2, 4, or 8 bytes in size (8, 16, 32 or 64 bits)
		VT_Float = 0x04, // floating point value, can be 4 or 8 bytes in size (float or double)
		VT_Vec2 = 0x05, // a 2-component floating point vector, can be 8 or 16 bytes in size (float or double per component)
		VT_Vec3 = 0x06, // a 3-component floating point vector, can be 12 or 24 bytes in size (float or double per component)
		VT_Vec4 = 0x07, // a 4-component floating point vector, can be 16 or 32 bytes in size (float or double per component)
		VT_IVec2 = 0x08, // a 2-component integer vector, can be 2, 4, 8 or 16 bytes in size (8, 16, 32 or 64 bits per component)
		VT_IVec3 = 0x09, // a 3-component integer vector, can be 3, 6, 12 or 24 bytes in size (8, 16, 32 or 64 bits per component) 
		VT_IVec4 = 0x0a, // a 4-component integer vector, can be 4, 8, 16 or 32 bytes in size (8, 16, 32 or 64 bits per component) 
		VT_UVec2 = 0x0b, // a 2-component unsigned integer vector, can be 2, 4, 8 or 16 bytes in size (8, 16, 32 or 64 bits per component) 
		VT_UVec3 = 0x0c, // a 3-component unsigned integer vector, can be 3, 6, 12 or 24 bytes in size (8, 16, 32 or 64 bits per component)
		VT_UVec4 = 0x0d, // a 4-component unsigned integer vector, can be 4, 8, 16 or 32 bytes in size (8, 16, 32 or 64 bits per component)
		VT_Mat2 = 0x0e, // a 2x2 matrix, can be 16 or 32 bytes in size (float or double per component)
		VT_Mat3 = 0x0f, // a 3x3 matrix, can be 36 or 72 bytes in size (float or double per component)
		VT_Mat4 = 0x10, // a 4x4 matrix, can be 64 or 128 bytes in size (float or double per component)
		VT_Quat = 0x11, // a quaternion, can be 16 or 32 bytes in size (float or double per component)
		VT_Uuid = 0x12, // a UUID, universally unique identifier, 16 bytes in size
		VT_Hash = 0x13, // a hash value, 32 bytes in size

		// -----------------------------------------------------------------------------------------------------------------------
		// --- All types 0x40 and up use the larger encoding chunk, and can be up to 2^64 bytes in size

		// --- Array of base value types, valid range: 0x40 - 0x7f
		VT_Array_Bool = 0x41,  // array of VT_Bool 
		VT_Array_Int = 0x42,   // array of VT_Int 
		VT_Array_UInt = 0x43,  // array of VT_UInt 
		VT_Array_Float = 0x44, // array of VT_Float
		VT_Array_Vec2 = 0x45,  // array of VT_Vec2 
		VT_Array_Vec3 = 0x46,  // array of VT_Vec3 
		VT_Array_Vec4 = 0x47,  // array of VT_Vec4 
		VT_Array_IVec2 = 0x48, // array of VT_IVec2
		VT_Array_IVec3 = 0x49, // array of VT_IVec3
		VT_Array_IVec4 = 0x4a, // array of VT_IVec4
		VT_Array_UVec2 = 0x4b, // array of VT_UVec2
		VT_Array_UVec3 = 0x4c, // array of VT_UVec3
		VT_Array_UVec4 = 0x4d, // array of VT_UVec4
		VT_Array_Mat2 = 0x4e,  // array of VT_Mat2 
		VT_Array_Mat3 = 0x4f,  // array of VT_Mat3 
		VT_Array_Mat4 = 0x50,  // array of VT_Mat4 
		VT_Array_Quat = 0x51,  // array of VT_Quat 
		VT_Array_Uuid = 0x52,  // array of VT_Uuid
		VT_Array_Hash = 0x53,  // array of VT_Hash

		// --- Specific types: 0xd0 - 0xff
		VT_Subsection = 0xd0, // a named subsection, containins named values and nested subsections. 
		VT_Array_Subsection = 0xd1, // array of (unnamed) subsections
		VT_String = 0xe0, // a UTF-8 encoded string
		VT_Array_String = 0xe1, // array of strings
		};

	// all container type indices
	enum class container_type_index
		{
		ct_none = 0x0,
		ct_optional_value = 0x1,
		ct_vector = 0x10,
		ct_optional_vector = 0x11,
		ct_idx_vector = 0x20,
		ct_optional_idx_vector = 0x21,
		};

	// widens utf-8 char string to wstring
	std::wstring widen( const std::string &str );

	// creates values from big-endian raw 2, 4 or 8 byte data (template implemented for u16, u32 and u64)
	template <class T> T value_from_bigendian( const u8 *src ) { static_assert(false, "value_from_bigendian template can only be used with u16, u32 or u64"); }
	template <> inline u16 value_from_bigendian<u16>( const u8 *src ) { return (u16(src[0]) << 8) | u16(src[1]); }
	template <> inline u32 value_from_bigendian<u32>( const u8 *src ) { return (u32(value_from_bigendian<u16>( &src[0] )) << 16) | u32(value_from_bigendian<u16>( &src[2] )); }
	template <> inline u64 value_from_bigendian<u64>( const u8 *src ) { return (u64(value_from_bigendian<u32>( &src[0] )) << 32) | u64(value_from_bigendian<u32>( &src[4] )); }

	// creates big-endian raw 2, 4 or 8 byte data from values (template implemented for u16, u32 and u64)
	template <class T> void bigendian_from_value( u8 *dst, T value ) { static_assert(false, "bigendian_from_value template can only be used with u16, u32 or u64"); }
	template <> inline void bigendian_from_value<u16>( u8 *dst , u16 value ) { dst[0] = u8((value >> 8) & 0xff); dst[1] = u8(value & 0xff); }
	template <> inline void bigendian_from_value<u32>( u8 *dst , u32 value ) { bigendian_from_value<u16>( &dst[0] , u16((value >> 16) & 0xffff) ); bigendian_from_value<u16>( &dst[2] , u16(value & 0xffff) ); }
	template <> inline void bigendian_from_value<u64>( u8 *dst , u64 value ) { bigendian_from_value<u32>( &dst[0] , u32((value >> 32) & 0xffffffff) ); bigendian_from_value<u32>( &dst[4] , u32(value & 0xffffffff) ); }

	// creates a string from an array of bytes, in order
	std::string bytes_to_hex_string( const void *bytes, size_t count );
	template <class T> std::string value_to_hex_string( T value ) { static_assert(false, "value_to_hex_string template can only be used with defined values, not a generic type T"); }
	template <> std::string value_to_hex_string<u8>( u8 value );
	template <> std::string value_to_hex_string<u16>( u16 value );
	template <> std::string value_to_hex_string<u32>( u32 value );
	template <> std::string value_to_hex_string<u64>( u64 value );
	template <> std::string value_to_hex_string<uuid>( uuid value );
	template <> std::string value_to_hex_string<hash>( hash value );

	// converts file path in wstring to an absolute or full file path 
	std::wstring full_path( const std::wstring &path );

	// swap byte order on one or multiple words
	inline void swap_bytes( u8 *pA, u8 *pB )
		{
		u8 tmp = *pA;
		*pA = *pB;
		*pB = tmp;
		}

	template <class T> void swap_byte_order( T *dest ) { static_assert(false, "swap_byte_order template can only be used with u16, u32 or u64"); }
	template<> inline void swap_byte_order<u16>( u16 *dest )
		{
		swap_bytes( &((u8 *)dest)[0], &((u8 *)dest)[1] );
		}
	template<> inline void swap_byte_order<u32>( u32 *dest )
		{
		swap_bytes( &((u8 *)dest)[0], &((u8 *)dest)[3] );
		swap_bytes( &((u8 *)dest)[1], &((u8 *)dest)[2] );
		}
	template<> inline void swap_byte_order<u64>( u64 *dest )
		{
		swap_bytes( &((u8 *)dest)[0], &((u8 *)dest)[7] );
		swap_bytes( &((u8 *)dest)[1], &((u8 *)dest)[6] );
		swap_bytes( &((u8 *)dest)[2], &((u8 *)dest)[5] );
		swap_bytes( &((u8 *)dest)[3], &((u8 *)dest)[4] );
		}
	template <class T> void swap_byte_order( T *dest , size_t count ) { static_assert(false, "swap_byte_order template can only be used with u16, u32 or u64"); }
	template<> inline void swap_byte_order<u16>( u16 *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((u8 *)dest)[0], &((u8 *)dest)[1] );
			++dest;
			}
		}
	template<> inline void swap_byte_order<u32>( u32 *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((u8 *)dest)[0], &((u8 *)dest)[3] );
			swap_bytes( &((u8 *)dest)[1], &((u8 *)dest)[2] );
			++dest;
			}
		}
	template<> inline void swap_byte_order<u64>( u64 *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((u8 *)dest)[0], &((u8 *)dest)[7] );
			swap_bytes( &((u8 *)dest)[1], &((u8 *)dest)[6] );
			swap_bytes( &((u8 *)dest)[2], &((u8 *)dest)[5] );
			swap_bytes( &((u8 *)dest)[3], &((u8 *)dest)[4] );
			++dest;
			}
		}


	};