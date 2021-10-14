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

// define LITTLE_ENDIAN
#define LITTLE_ENDIAN

#include <utility>
#include <map>
#include <mutex>

namespace ISD
	{
	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;

	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;

	typedef unsigned int uint;

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

	};