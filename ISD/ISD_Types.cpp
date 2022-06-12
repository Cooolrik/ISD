// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "ISD_Types.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Rpc.h>

#include "ISD_TypesAllocation.inl"

namespace ISD
	{
	std::wstring widen( const std::string &str )
		{
		std::wstring ret;

		if( !str.empty() )
			{
			size_t wsize;
			mbstowcs_s( &wsize, nullptr, 0, str.c_str(), 0 );

			size_t alloc_wsize = wsize + 1;
			wchar_t *wstr = new wchar_t[alloc_wsize];
			size_t conv_count;
			mbstowcs_s( &conv_count, wstr, alloc_wsize, str.c_str(), wsize );

			ret = std::wstring( wstr );
			delete[] wstr;
			}

		return ret;
		}

	// writes array of bytes to string of hex values. the hex values will be
	// in the same order as the bytes, so if you need to convert a litte-endian
	// word into hex, be sure to flip the byte order before.
	std::string bytes_to_hex_string( const void *bytes, size_t count )
		{
		static const char hexchars[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

		std::string ret;
		const u8 *p = (const u8 *)bytes;
		for( size_t i = 0; i < count; ++i )
			{
			ret += hexchars[((*p) >> 4) & 0xf]; // high nibble
			ret += hexchars[(*p) & 0xf]; // low nibble
			++p;
			}
		return ret;
		}

	template <> std::string value_to_hex_string<u8>( u8 value )
		{
		return bytes_to_hex_string( &value, sizeof(value) );
		}

	template <> std::string value_to_hex_string<u16>( u16 value )
		{
		bigendian_from_value( (u8*)&value, value ); // in-place make sure big endian
		return bytes_to_hex_string( &value, sizeof(value) );
		}

	template <> std::string value_to_hex_string<u32>( u32 value )
		{
		bigendian_from_value( (u8*)&value, value ); // in-place make sure big endian
		return bytes_to_hex_string( &value, sizeof(value) );
		}

	template <> std::string value_to_hex_string<u64>( u64 value )
		{
		bigendian_from_value( (u8*)&value, value ); // in-place make sure big endian
		return bytes_to_hex_string( &value, sizeof(value) );
		}

	template <> std::string value_to_hex_string<uuid>( uuid value )
		{
		std::string ret;

		ret += value_to_hex_string<u32>( value.Data1 );
		ret += "-";
		ret += value_to_hex_string<u16>( value.Data2 );
		ret += "-";
		ret += value_to_hex_string<u16>( value.Data3 );
		ret += "-";
		ret += bytes_to_hex_string( value.Data4 , 2 );
		ret += "-";
		ret += bytes_to_hex_string( &value.Data4[2] , 6 );

		return ret;
		}

	template <> std::string value_to_hex_string<hash>( hash value )
		{
		static_assert(sizeof( hash ) == 32, "Error: hash is assumed to be of size 32.");
		return bytes_to_hex_string( &value, 32 );
		}


	std::wstring full_path( const std::wstring &path )
		{
		std::wstring ret;

		wchar_t *buffer = new wchar_t[32768];
		DWORD len = GetFullPathNameW( path.c_str(), 32768, buffer, nullptr );
		if( len > 0 )
			{
			ret = std::wstring( buffer );
			}
		delete[] buffer;

		return ret;
		}

	entity_ref entity_ref::make_ref()
		{
		entity_ref ref;

		RPC_STATUS stat = ::UuidCreate( &ref.id_m );
		if( stat != RPC_S_OK 
			|| ref.id_m == uuid_zero )
			{
			throw std::exception( "Failed to generate a uuid through ::UuidCreate()" );
			}

		return ref;
		}

	};
