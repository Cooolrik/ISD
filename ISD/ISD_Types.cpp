
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


	static std::wstring bytes_to_hex_string( const void *bytes, uint size )
		{
		static const wchar_t hexchars[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

		std::wstring ret;
		const uint8 *p = (const uint8 *)bytes;
		for( uint i = 0; i < size; ++i )
			{
			ret += hexchars[((*p) >> 4) & 0xf]; // high nibble
			ret += hexchars[(*p) & 0xf]; // low nibble
			++p;
			}
		return ret;
		}


	std::wstring uint8_to_hex_string( uint8 value )
		{
		return bytes_to_hex_string( &value, sizeof(value) );
		}

	std::wstring uint16_to_hex_string( uint16 value )
		{
#ifdef LITTLE_ENDIAN
		value = flip_byte_order_uint16( value );
#endif//LITTLE_ENDIAN
		return bytes_to_hex_string( &value, sizeof(value) );
		}

	std::wstring uint32_to_hex_string( uint32 value )
		{
#ifdef LITTLE_ENDIAN
		value = flip_byte_order_uint32( value );
#endif//LITTLE_ENDIAN
		return bytes_to_hex_string( &value, sizeof(value) );
		}

	std::wstring uint64_to_hex_string( uint64 value )
		{
#ifdef LITTLE_ENDIAN
		value = flip_byte_order_uint64( value );
#endif//LITTLE_ENDIAN
		return bytes_to_hex_string( &value, sizeof(value) );
		}

	std::wstring uuid_to_hex_string( UUID value )
		{
		std::wstring ret;

		ret += uint32_to_hex_string( value.Data1 );
		ret += L"-";
		ret += uint16_to_hex_string( value.Data2 );
		ret += L"-";
		ret += uint16_to_hex_string( value.Data3 );
		ret += L"-";
		ret += bytes_to_hex_string( value.Data4 , 2 );
		ret += L"-";
		ret += bytes_to_hex_string( &value.Data4[2] , 6 );

		return ret;
		}

	uint16 flip_byte_order_uint16( uint16 value )
		{
		uint16 ret;
		((uint8 *)&ret)[0] = ((uint8 *)&value)[1];
		((uint8 *)&ret)[1] = ((uint8 *)&value)[0];
		return ret;
		}

	uint32 flip_byte_order_uint32( uint32 value )
		{
		uint32 ret;
		((uint8 *)&ret)[0] = ((uint8 *)&value)[3];
		((uint8 *)&ret)[1] = ((uint8 *)&value)[2];		
		((uint8 *)&ret)[2] = ((uint8 *)&value)[1];		
		((uint8 *)&ret)[3] = ((uint8 *)&value)[0];
		return ret;
		}
			
	uint64 flip_byte_order_uint64( uint64 value )
		{
		uint64 ret;
		((uint8 *)&ret)[0] = ((uint8 *)&value)[7];
		((uint8 *)&ret)[1] = ((uint8 *)&value)[6];		
		((uint8 *)&ret)[2] = ((uint8 *)&value)[5];		
		((uint8 *)&ret)[3] = ((uint8 *)&value)[4];
		((uint8 *)&ret)[4] = ((uint8 *)&value)[3];
		((uint8 *)&ret)[5] = ((uint8 *)&value)[2];		
		((uint8 *)&ret)[6] = ((uint8 *)&value)[1];		
		((uint8 *)&ret)[7] = ((uint8 *)&value)[0];
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



	};
