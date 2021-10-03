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

	// creates a wstring from an array of bytes, in order
	std::wstring uint8_to_hex_string( uint8 value );
	std::wstring uint16_to_hex_string( uint16 value );
	std::wstring uint32_to_hex_string( uint32 value );
	std::wstring uuid_to_hex_string( UUID value );

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


	};