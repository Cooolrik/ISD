// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

// define GUID stuff from windows
#ifdef _WIN32
#include <guiddef.h>
#endif//_WIN32

// define UUID
#ifndef UUID_DEFINED
#define UUID_DEFINED
typedef GUID UUID;
#ifdef _WIN32
#ifndef uuid_t
#define uuid_t UUID
#endif//uuid_t
#endif//_WIN32
#endif

#include <utility>
#include <map>
#include <mutex>
#include <vector>

#include <glm/fwd.hpp>

#define ISDErrorLog Log::Error( __FUNCSIG__ , __FILE__ , __LINE__ ) 
#define ISDErrorLogEnd std::endl

#ifdef _DEBUG
#define ISDSanityCheckDebugMacro( statement ) if( !(statement) ) { ISDErrorLog << "Sanity debug check failed: (" #statement ")" << ISDErrorLogEnd; throw std::exception(); }
#define ISDSanityCheckCoreDebugMacro( statement ) if( !(statement) ) { ISDErrorLog << "Core debug sanity check failed: (" #statement ")" << ISDErrorLogEnd; throw std::exception(); }
#else
#define ISDSanityCheckDebugMacro( statement ) 
#define ISDSanityCheckCoreDebugMacro( statement ) 
#endif

namespace ISD
	{
	typedef std::int8_t	int8;
	typedef std::int16_t int16;
	typedef std::int32_t int32;
	typedef std::int64_t int64;
	typedef std::uint8_t uint8;
	typedef std::uint16_t uint16;
	typedef std::uint32_t uint32;
	typedef std::uint64_t uint64;

	typedef unsigned int uint;

	typedef glm::vec<2, glm::i8, glm::packed_highp> i8vec2;
	typedef glm::vec<3, glm::i8, glm::packed_highp> i8vec3;
	typedef glm::vec<4, glm::i8, glm::packed_highp> i8vec4;
	typedef glm::vec<2, glm::i16, glm::packed_highp> i16vec2;
	typedef glm::vec<3, glm::i16, glm::packed_highp> i16vec3;
	typedef glm::vec<4, glm::i16, glm::packed_highp> i16vec4;
	typedef glm::vec<2, glm::i32, glm::packed_highp> i32vec2;
	typedef glm::vec<3, glm::i32, glm::packed_highp> i32vec3;
	typedef glm::vec<4, glm::i32, glm::packed_highp> i32vec4;
	typedef glm::vec<2, glm::i64, glm::packed_highp> i64vec2;
	typedef glm::vec<3, glm::i64, glm::packed_highp> i64vec3;
	typedef glm::vec<4, glm::i64, glm::packed_highp> i64vec4;

	typedef glm::vec<2, glm::u8, glm::packed_highp> u8vec2;
	typedef glm::vec<3, glm::u8, glm::packed_highp> u8vec3;
	typedef glm::vec<4, glm::u8, glm::packed_highp> u8vec4;
	typedef glm::vec<2, glm::u16, glm::packed_highp> u16vec2;
	typedef glm::vec<3, glm::u16, glm::packed_highp> u16vec3;
	typedef glm::vec<4, glm::u16, glm::packed_highp> u16vec4;
	typedef glm::vec<2, glm::u32, glm::packed_highp> u32vec2;
	typedef glm::vec<3, glm::u32, glm::packed_highp> u32vec3;
	typedef glm::vec<4, glm::u32, glm::packed_highp> u32vec4;
	typedef glm::vec<2, glm::u64, glm::packed_highp> u64vec2;
	typedef glm::vec<3, glm::u64, glm::packed_highp> u64vec3;
	typedef glm::vec<4, glm::u64, glm::packed_highp> u64vec4;

	typedef glm::vec<2, glm::f32, glm::packed_highp> fvec2;
	typedef glm::vec<3, glm::f32, glm::packed_highp> fvec3;
	typedef glm::vec<4, glm::f32, glm::packed_highp> fvec4;
	typedef glm::vec<2, glm::f64, glm::packed_highp> dvec2;
	typedef glm::vec<3, glm::f64, glm::packed_highp> dvec3;
	typedef glm::vec<4, glm::f64, glm::packed_highp> dvec4;
						
	typedef glm::vec<2, glm::f32, glm::packed_highp> fvec2;
	typedef glm::vec<3, glm::f32, glm::packed_highp> fvec3;
	typedef glm::vec<4, glm::f32, glm::packed_highp> fvec4;
	typedef glm::vec<2, glm::f64, glm::packed_highp> dvec2;
	typedef glm::vec<3, glm::f64, glm::packed_highp> dvec3;
	typedef glm::vec<4, glm::f64, glm::packed_highp> dvec4;

	typedef glm::mat<2, 2, glm::f32, glm::packed_highp>	fmat2;
	typedef glm::mat<3, 3, glm::f32, glm::packed_highp>	fmat3;
	typedef glm::mat<4, 4, glm::f32, glm::packed_highp>	fmat4;
	typedef glm::mat<2, 2, glm::f64, glm::packed_highp>	dmat2;
	typedef glm::mat<3, 3, glm::f64, glm::packed_highp>	dmat3;
	typedef glm::mat<4, 4, glm::f64, glm::packed_highp>	dmat4;

	typedef glm::uvec2 uvec2;
	typedef glm::uvec3 uvec3;
	typedef glm::uvec4 uvec4;
	typedef glm::ivec2 ivec2;
	typedef glm::ivec3 ivec3;
	typedef glm::ivec4 ivec4;
	typedef glm::vec2 vec2;
	typedef glm::vec3 vec3;
	typedef glm::vec4 vec4;
	typedef glm::mat2 mat2;
	typedef glm::mat3 mat3;
	typedef glm::mat4 mat4;

	const size_t EntityMaxKeyLength = 40; // maximum size of a name in the entities

	enum class Status
		{
		Ok = 0, // no error
		EUndefined = -1, // undefined error
		EParam = -2, // invalid parameter in method call
		ENotInitialized = -3, // the system is not initialized
		EAlreadyInitialized = -4, // the system is not initialized
		ECantAllocate = -5, // cant allocate memory
		ECantOpen = -6, // cant open file or handle
		ECantRead = -7, // cant read from file or handle
		ECorrupted = -8, // a filed is corrupted (failes sha256 test)
		EInvalid = -9, // invalid file, not an ISD file
		};

	// reflection and serialization value types
	enum class ValueType
		{
		// -----------------------------------------------------------------------------------------------------------------------
		// --- Base value types up to 0x3f use the smaller encoding chunk, and are capped at less than 256 bytes in size

		// --- Base value types, valid range: 0x00 - 0x3f 
		VT_Null = 0x00, // empty value, can only have 0 size. Explicitly empty, as in that the key exists, but does not have a value.
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
		VT_UUID = 0x11, // a UUID, universally unique identifier, 16 bytes in size

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
		VT_Array_UUID = 0x51,  // array of VT_UUID 

		// --- Specific types: 0xd0 - 0xff
		VT_Subsection = 0xd0, // a named subsection, containins named values and nested subsections. 
		VT_Array_Subsection = 0xd1, // array of (unnamed) subsections
		VT_String = 0xe0, // a UTF-8 encoded string
		VT_Array_String = 0xe1, // array of strings
		};

	// widens utf-8 char string to wstring
	std::wstring widen( const std::string &str );

	// creates values from big-endian raw 2, 4 or 8 byte data (template implemented for uint16, uint32 and uint64)
	template <class T> T value_from_bigendian( const uint8 *src ) { static_assert(false, "value_from_bigendian template can only be used with uint16, uint32 or uint64"); }
	template <> inline uint16 value_from_bigendian<uint16>( const uint8 *src ) { return (uint16(src[0]) << 8) | uint16(src[1]); }
	template <> inline uint32 value_from_bigendian<uint32>( const uint8 *src ) { return (uint32(value_from_bigendian<uint16>( &src[0] )) << 16) | uint32(value_from_bigendian<uint16>( &src[2] )); }
	template <> inline uint64 value_from_bigendian<uint64>( const uint8 *src ) { return (uint64(value_from_bigendian<uint32>( &src[0] )) << 32) | uint64(value_from_bigendian<uint32>( &src[4] )); }

	// creates big-endian raw 2, 4 or 8 byte data from values (template implemented for uint16, uint32 and uint64)
	template <class T> void bigendian_from_value( uint8 *dst, T value ) { static_assert(false, "bigendian_from_value template can only be used with uint16, uint32 or uint64"); }
	template <> inline void bigendian_from_value<uint16>( uint8 *dst , uint16 value ) { dst[0] = uint8((value >> 8) & 0xff); dst[1] = uint8(value & 0xff); }
	template <> inline void bigendian_from_value<uint32>( uint8 *dst , uint32 value ) { bigendian_from_value<uint16>( &dst[0] , uint16((value >> 16) & 0xffff) ); bigendian_from_value<uint16>( &dst[2] , uint16(value & 0xffff) ); }
	template <> inline void bigendian_from_value<uint64>( uint8 *dst , uint64 value ) { bigendian_from_value<uint32>( &dst[0] , uint32((value >> 32) & 0xffffffff) ); bigendian_from_value<uint32>( &dst[4] , uint32(value & 0xffffffff) ); }

	// creates a wstring from an array of bytes, in order
	std::wstring bytes_to_hex_wstring( const void *bytes, size_t count );
	template <class T> std::wstring value_to_hex_wstring( T value ) { static_assert(false, "value_to_hex_wstring template can only be used with defined values, not a generic type T"); }
	template <> std::wstring value_to_hex_wstring<uint8>( uint8 value );
	template <> std::wstring value_to_hex_wstring<uint16>( uint16 value );
	template <> std::wstring value_to_hex_wstring<uint32>( uint32 value );
	template <> std::wstring value_to_hex_wstring<uint64>( uint64 value );
	template <> std::wstring value_to_hex_wstring<UUID>( UUID value );

	// converts file path in wstring to an absolute or full file path 
	std::wstring full_path( const std::wstring &path );

	// thread safe map, does not allow access to 
	// items, only insert, erase and find with value returned as copy 
	// all public methods are thread safe 
	// TODO: The inner workings of this class should be replaced with a lock-free substitute
	template<class _Kty, class _Ty> class thread_safe_map
		{
		private:
			using _Mybase = std::map<_Kty, _Ty>;
			using key_type = _Kty;
			using mapped_type = _Ty;
			using iterator = typename _Mybase::iterator;
			using value_type = std::pair<const _Kty, _Ty>;

			std::map<_Kty, _Ty> Data;
			std::mutex AccessMutex;

		public:
			std::pair<_Ty,bool> find( const _Kty &key )
				{
				std::lock_guard<std::mutex> guard(this->AccessMutex);
				iterator it = this->Data.find( key );
				if( it != this->Data.end() )
					{
					return std::make_pair(it->second,true);
					}
				return std::make_pair(_Ty(),false);
				}

			bool insert( const value_type &value )
				{
				std::lock_guard<std::mutex> guard(this->AccessMutex);
				return this->Data.insert( value ).second;
				}

			size_t erase( const _Kty &key )
				{
				std::lock_guard<std::mutex> guard(this->AccessMutex);
				return this->Data.erase( key );
				}
		};

	// swap byte order on one or multiple words
	inline void swap_bytes( uint8 *pA, uint8 *pB )
		{
		uint8 tmp = *pA;
		*pA = *pB;
		*pB = tmp;
		}

	template <class T> void swap_byte_order( T *dest ) { static_assert(false, "swap_byte_order template can only be used with uint16, uint32 or uint64"); }
	template<> inline void swap_byte_order<uint16>( uint16 *dest )
		{
		swap_bytes( &((uint8 *)dest)[0], &((uint8 *)dest)[1] );
		}
	template<> inline void swap_byte_order<uint32>( uint32 *dest )
		{
		swap_bytes( &((uint8 *)dest)[0], &((uint8 *)dest)[3] );
		swap_bytes( &((uint8 *)dest)[1], &((uint8 *)dest)[2] );
		}
	template<> inline void swap_byte_order<uint64>( uint64 *dest )
		{
		swap_bytes( &((uint8 *)dest)[0], &((uint8 *)dest)[7] );
		swap_bytes( &((uint8 *)dest)[1], &((uint8 *)dest)[6] );
		swap_bytes( &((uint8 *)dest)[2], &((uint8 *)dest)[5] );
		swap_bytes( &((uint8 *)dest)[3], &((uint8 *)dest)[4] );
		}
	template <class T> void swap_byte_order( T *dest , size_t count ) { static_assert(false, "swap_byte_order template can only be used with uint16, uint32 or uint64"); }
	template<> inline void swap_byte_order<uint16>( uint16 *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((uint8 *)dest)[0], &((uint8 *)dest)[1] );
			++dest;
			}
		}
	template<> inline void swap_byte_order<uint32>( uint32 *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((uint8 *)dest)[0], &((uint8 *)dest)[3] );
			swap_bytes( &((uint8 *)dest)[1], &((uint8 *)dest)[2] );
			++dest;
			}
		}
	template<> inline void swap_byte_order<uint64>( uint64 *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((uint8 *)dest)[0], &((uint8 *)dest)[7] );
			swap_bytes( &((uint8 *)dest)[1], &((uint8 *)dest)[6] );
			swap_bytes( &((uint8 *)dest)[2], &((uint8 *)dest)[5] );
			swap_bytes( &((uint8 *)dest)[3], &((uint8 *)dest)[4] );
			++dest;
			}
		}

	template<class T> class optional_value
		{
		private:
			T value = {};
			bool has_value = false;
		public:
			void Clear() { this->value = {};  this->has_value = false; }
			void Set( const T &_value ) { this->has_value = true; this->value = _value; }
			bool HasValue() const { return this->has_value; }
			const std::pair<bool, const T&> Value() const { return std::pair<bool, const T&>( this->has_value, this->value ); }
		};

	extern const std::vector<size_t> indexed_array_empty_index; // used as replacement reference when returning an empty index
	template<class T> class indexed_array
		{
		private:
			std::vector<T> data = {};
			std::vector<size_t> *index = nullptr;
			
		public:
			indexed_array( const std::vector<T> &_data ) : data( _data ) {}
			indexed_array( const std::vector<T> &_data, const std::vector<size_t> _index ) : data( _data ) { this->index = new std::vector<size_t>(_index); }
			~indexed_array() 
				{
				if( this->index ) { delete[] this->index; }
				}
			const std::vector<T> &Data() const { return this->data; }
			bool HasIndex() const { return (this->index != nullptr); }
			const std::pair<bool, const std::vector<size_t> &> Index() const 
				{ 
				if( this->index )
					{
					return std::pair<bool, const std::vector<size_t> &>( true, *(this->index) );
					}
				else
					{
					return std::pair<bool, const std::vector<size_t> &>( false, indexed_array_empty_index );
					}
				}
		};


	};