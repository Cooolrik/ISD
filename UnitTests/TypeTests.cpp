// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\ISD\ISD_SHA256.h"

namespace UnitTests
	{
	TEST_CLASS(TypeTests)
		{
		public:
		
			TEST_METHOD( ISDTypes )
				{
				Assert::IsTrue(sizeof(u8) == 1);
				Assert::IsTrue(sizeof(u16) == 2);
				Assert::IsTrue(sizeof(u32) == 4);
				Assert::IsTrue(sizeof(u64) == 8);
				Assert::IsTrue(sizeof(i8) == 1);
				Assert::IsTrue(sizeof(i16) == 2);
				Assert::IsTrue(sizeof(i32) == 4);
				Assert::IsTrue(sizeof(i64) == 8);
				Assert::IsTrue(sizeof(uint) == 4);
				Assert::IsTrue(sizeof(UUID) == 16);

				// test widen()
				std::string str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
				std::wstring expected_wstr = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
				std::wstring wstr = widen( str );
				Assert::IsTrue( wstr == expected_wstr );

				// test full_path()
				std::wstring rel_path( L"." );
				std::wstring fpath = full_path( rel_path );
				wchar_t currpath[MAX_PATH];
				GetCurrentDirectoryW( MAX_PATH, currpath );
				std::wstring expected_fpath( currpath );
				Assert::IsTrue( fpath == expected_fpath );
				}

			TEST_METHOD(ByteswapFunctions)
				{
				const u16 u16val = 0x6712;
				const u32 u32val = 0x93671028;
				const u64 u64val = 0x2047239209461749;

				const u8 u16_be[] = {0x67, 0x12};
				const u8 u32_be[] = {0x93, 0x67, 0x10, 0x28};
				const u8 u64_be[] = {0x20, 0x47, 0x23, 0x92, 0x09, 0x46, 0x17, 0x49};

				u16 t16 = value_from_bigendian<u16>( u16_be );
				Assert::IsTrue(t16 == u16val);
				u32 t32 = value_from_bigendian<u32>( u32_be );
				Assert::IsTrue(t32 == u32val);
				u64 t64 = value_from_bigendian<u64>( u64_be );
				Assert::IsTrue(t64 == u64val);
			
				u8 res[8];
				bigendian_from_value<u16>( res , u16val );
				Assert::IsTrue(memcmp(res,u16_be,sizeof(u16_be)) == 0);
				bigendian_from_value<u32>( res , u32val );
				Assert::IsTrue(memcmp(res,u32_be,sizeof(u32_be)) == 0);
				bigendian_from_value<u64>( res , u64val );
				Assert::IsTrue(memcmp(res,u64_be,sizeof(u64_be)) == 0);

				t16 = u16val;
				swap_byte_order( &t16 );
				Assert::IsTrue( ((t16 >> 0) & 0xff) == ((u16val >>  8) & 0xff) );
				Assert::IsTrue( ((t16 >> 8) & 0xff) == ((u16val >>  0) & 0xff) );
				
				t32 = u32val;
				swap_byte_order( &t32 );
				Assert::IsTrue( ((t32 >>  0) & 0xff) == ((u32val >> 24) & 0xff) );
				Assert::IsTrue( ((t32 >>  8) & 0xff) == ((u32val >> 16) & 0xff) );
				Assert::IsTrue( ((t32 >> 16) & 0xff) == ((u32val >>  8) & 0xff) );
				Assert::IsTrue( ((t32 >> 24) & 0xff) == ((u32val >>  0) & 0xff) );

				t64 = u64val;
				swap_byte_order( &t64 );
				Assert::IsTrue( ((t64 >>  0) & 0xff) == ((u64val >> 56) & 0xff) );
				Assert::IsTrue( ((t64 >>  8) & 0xff) == ((u64val >> 48) & 0xff) );
				Assert::IsTrue( ((t64 >> 16) & 0xff) == ((u64val >> 40) & 0xff) );
				Assert::IsTrue( ((t64 >> 24) & 0xff) == ((u64val >> 32) & 0xff) );
				Assert::IsTrue( ((t64 >> 32) & 0xff) == ((u64val >> 24) & 0xff) );
				Assert::IsTrue( ((t64 >> 40) & 0xff) == ((u64val >> 16) & 0xff) );
				Assert::IsTrue( ((t64 >> 48) & 0xff) == ((u64val >>  8) & 0xff) );
				Assert::IsTrue( ((t64 >> 56) & 0xff) == ((u64val >>  0) & 0xff) );
				}

			TEST_METHOD( HexStringFunctions )
				{
				UUID id = { 0x12345678 , 0xa1a2 , 0xb1b2 , { 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8 } };
				std::wstring expected_hexid = L"12345678-a1a2-b1b2-c1c2-c3c4c5c6c7c8";
				Assert::IsTrue( value_to_hex_wstring( id ) == expected_hexid );

				u8 u8val = 0x13;
				std::wstring expected_hexu8 = L"13";
				Assert::IsTrue( value_to_hex_wstring( u8val ) == expected_hexu8 );

				u16 u16val = 0x1234;
				std::wstring expected_hexu16 = L"1234";
				Assert::IsTrue( value_to_hex_wstring( u16val ) == expected_hexu16 );

				u32 u32val = 0x0218a782;
				std::wstring expected_hexu32 = L"0218a782";
				Assert::IsTrue( value_to_hex_wstring( u32val ) == expected_hexu32 );

				u64 u64val = 0x35023218a7828505;
				std::wstring expected_hexu64 = L"35023218a7828505";
				Assert::IsTrue( value_to_hex_wstring( u64val ) == expected_hexu64 );
				}


			TEST_METHOD( SHA256Hashing )
				{
				if( true )
					{
					SHA256 sha;

					u8 testdata[] = {
						0x34,0x2b,0x1f,0x3e,0x61,
						0x4b,0x03,0x4b,0x02,0x36,
						0x05,0x5c,0x17,0x29,0x3d,
						0x53,0x0e,0x5e,0x5b,0x4d,
						0x52,0x5f,0x12,0x20,0x0a,
						0x56,0x31,0x3b,0x2c,0x06,
						0x51,0x28,0x28,0x5d,0x05,
						0x59,0x2b,0x41,0x0d,0x1f,
						0x01,0x01,0x1b,0x1f,0x09,
						0x2c,0x13,0x01,0x46,0x19,
						0x05,0x3e,0x3c,0x2d,0x58,
						0x16,0x5f,0x19,0x0f,0x07,
						0x39,0x48,0x46,0x4b,0x23,
						0x06,0x15,0x0b,0x44,0x18,
						0x0e,0x38,0x56,0x0e,0x0a,
						0x0e,0x54,0x43,0x0a,0x31,
						0x2d,0x51,0x0d,0x2a,0x5a,
						0x09,0x06,0x10,0x23,0x24,
						0x23,0x33,0x2e,0x1d,0x56,
						0x48,0x2f,0x4a,0x33,0x06
						};

					sha.Update( testdata, sizeof( testdata ) );

					u8 calc_hash[32];
					sha.GetDigest( calc_hash );

					u8 expected_hash[32] = {0xf6,0x48,0x54,0x2d,0xf8,0xcc,0xf2,0x1f,0xd3,0x4e,0x95,0xf6,0x7d,0xf5,0xf2,0xb4,0xf2,0x72,0x72,0xaa,0x14,0xf5,0x03,0x09,0x0c,0xc4,0x76,0x6f,0xe2,0x78,0xc4,0xb5};

					Assert::IsTrue( memcmp( calc_hash, expected_hash, 32 ) == 0 );
					}
				}
		};
	}














