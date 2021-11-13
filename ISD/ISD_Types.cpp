// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "ISD_Types.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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
	std::wstring bytes_to_hex_wstring( const void *bytes, size_t count )
		{
		static const wchar_t hexchars[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

		std::wstring ret;
		const uint8 *p = (const uint8 *)bytes;
		for( size_t i = 0; i < count; ++i )
			{
			ret += hexchars[((*p) >> 4) & 0xf]; // high nibble
			ret += hexchars[(*p) & 0xf]; // low nibble
			++p;
			}
		return ret;
		}


	template <> std::wstring value_to_hex_wstring<uint8>( uint8 value )
		{
		return bytes_to_hex_wstring( &value, sizeof(value) );
		}

	template <> std::wstring value_to_hex_wstring<uint16>( uint16 value )
		{
		bigendian_from_value( (uint8*)&value, value ); // in-place make sure big endian
		return bytes_to_hex_wstring( &value, sizeof(value) );
		}

	template <> std::wstring value_to_hex_wstring<uint32>( uint32 value )
		{
		bigendian_from_value( (uint8*)&value, value ); // in-place make sure big endian
		return bytes_to_hex_wstring( &value, sizeof(value) );
		}

	template <> std::wstring value_to_hex_wstring<uint64>( uint64 value )
		{
		bigendian_from_value( (uint8*)&value, value ); // in-place make sure big endian
		return bytes_to_hex_wstring( &value, sizeof(value) );
		}

	template <> std::wstring value_to_hex_wstring<UUID>( UUID value )
		{
		std::wstring ret;

		ret += value_to_hex_wstring<uint32>( value.Data1 );
		ret += L"-";
		ret += value_to_hex_wstring<uint16>( value.Data2 );
		ret += L"-";
		ret += value_to_hex_wstring<uint16>( value.Data3 );
		ret += L"-";
		ret += bytes_to_hex_wstring( value.Data4 , 2 );
		ret += L"-";
		ret += bytes_to_hex_wstring( &value.Data4[2] , 6 );

		return ret;
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

	const std::vector<size_t> indexed_array_empty_index; // used as replacement reference when returning empty index
	};
