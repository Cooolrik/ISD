// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "pch.h"
#include "CppUnitTest.h"
#include "..\ISD\ISD_MemoryReadStream.h"
#include "..\ISD\ISD_MemoryWriteStream.h"
#include "..\ISD\ISD_SHA256.h"
#include "..\ISD\ISD_EntityWriter.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace ISD;

namespace UnitTests
	{
	TEST_CLASS(UnitTests)
		{
		public:
			uint8 uint8_rand() { return (uint8)(rand() & 0xff); }
			uint16 uint16_rand() { return (rand() << 4) ^ rand(); }
			uint32 uint32_rand() { return uint32(uint16_rand()) << 16 | uint32(uint16_rand()); }
			uint64 uint64_rand() { return uint64(uint32_rand()) << 32 | uint64(uint32_rand()); }
			float float_rand() { return float(uint64_rand()); }
			double double_rand() { return double(uint64_rand()); }
			UUID uuid_rand() 
				{ 
				const UUID id = {uint32_rand(), uint16_rand(), uint16_rand(), { uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand() }};
				return id;
				}

			TEST_METHOD( ISDTypes )
				{
				Assert::IsTrue(sizeof(uint8) == 1);
				Assert::IsTrue(sizeof(uint16) == 2);
				Assert::IsTrue(sizeof(uint32) == 4);
				Assert::IsTrue(sizeof(uint64) == 8);
				Assert::IsTrue(sizeof(int8) == 1);
				Assert::IsTrue(sizeof(int16) == 2);
				Assert::IsTrue(sizeof(int32) == 4);
				Assert::IsTrue(sizeof(int64) == 8);
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
				const uint16 u16 = 0x6712;
				const uint32 u32 = 0x93671028;
				const uint64 u64 = 0x2047239209461749;

				const uint8 u16_be[] = {0x67, 0x12};
				const uint8 u32_be[] = {0x93, 0x67, 0x10, 0x28};
				const uint8 u64_be[] = {0x20, 0x47, 0x23, 0x92, 0x09, 0x46, 0x17, 0x49};

				uint16 t16 = value_from_bigendian<uint16>( u16_be );
				Assert::IsTrue(t16 == u16);
				uint32 t32 = value_from_bigendian<uint32>( u32_be );
				Assert::IsTrue(t32 == u32);
				uint64 t64 = value_from_bigendian<uint64>( u64_be );
				Assert::IsTrue(t64 == u64);
			
				uint8 res[8];
				bigendian_from_value<uint16>( res , u16 );
				Assert::IsTrue(memcmp(res,u16_be,sizeof(u16_be)) == 0);
				bigendian_from_value<uint32>( res , u32 );
				Assert::IsTrue(memcmp(res,u32_be,sizeof(u32_be)) == 0);
				bigendian_from_value<uint64>( res , u64 );
				Assert::IsTrue(memcmp(res,u64_be,sizeof(u64_be)) == 0);

				t16 = u16;
				swap_byte_order( &t16 );
				Assert::IsTrue( ((t16 >> 0) & 0xff) == ((u16 >>  8) & 0xff) );
				Assert::IsTrue( ((t16 >> 8) & 0xff) == ((u16 >>  0) & 0xff) );
				
				t32 = u32;
				swap_byte_order( &t32 );
				Assert::IsTrue( ((t32 >>  0) & 0xff) == ((u32 >> 24) & 0xff) );
				Assert::IsTrue( ((t32 >>  8) & 0xff) == ((u32 >> 16) & 0xff) );
				Assert::IsTrue( ((t32 >> 16) & 0xff) == ((u32 >>  8) & 0xff) );
				Assert::IsTrue( ((t32 >> 24) & 0xff) == ((u32 >>  0) & 0xff) );

				t64 = u64;
				swap_byte_order( &t64 );
				Assert::IsTrue( ((t64 >>  0) & 0xff) == ((u64 >> 56) & 0xff) );
				Assert::IsTrue( ((t64 >>  8) & 0xff) == ((u64 >> 48) & 0xff) );
				Assert::IsTrue( ((t64 >> 16) & 0xff) == ((u64 >> 40) & 0xff) );
				Assert::IsTrue( ((t64 >> 24) & 0xff) == ((u64 >> 32) & 0xff) );
				Assert::IsTrue( ((t64 >> 32) & 0xff) == ((u64 >> 24) & 0xff) );
				Assert::IsTrue( ((t64 >> 40) & 0xff) == ((u64 >> 16) & 0xff) );
				Assert::IsTrue( ((t64 >> 48) & 0xff) == ((u64 >>  8) & 0xff) );
				Assert::IsTrue( ((t64 >> 56) & 0xff) == ((u64 >>  0) & 0xff) );
				}

			TEST_METHOD( HexStringFunctions )
				{
				UUID id = { 0x12345678 , 0xa1a2 , 0xb1b2 , { 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8 } };
				std::wstring expected_hexid = L"12345678-a1a2-b1b2-c1c2-c3c4c5c6c7c8";
				Assert::IsTrue( value_to_hex_wstring( id ) == expected_hexid );

				uint8 u8 = 0x13;
				std::wstring expected_hexu8 = L"13";
				Assert::IsTrue( value_to_hex_wstring( u8 ) == expected_hexu8 );

				uint16 u16 = 0x1234;
				std::wstring expected_hexu16 = L"1234";
				Assert::IsTrue( value_to_hex_wstring( u16 ) == expected_hexu16 );

				uint32 u32 = 0x0218a782;
				std::wstring expected_hexu32 = L"0218a782";
				Assert::IsTrue( value_to_hex_wstring( u32 ) == expected_hexu32 );

				uint64 u64 = 0x35023218a7828505;
				std::wstring expected_hexu64 = L"35023218a7828505";
				Assert::IsTrue( value_to_hex_wstring( u64 ) == expected_hexu64 );
				}

			template<class T> void AsserteReadValueIs( MemoryReadStream *rs , T ref_value )
				{
				T val = rs->Read<T>();
				Assert::IsTrue( val == ref_value );
				}

			TEST_METHOD( MemoryWriteReadStream )
				{
				// run twice, one with flipped, one with non-flipped byte order
				for( int byteorder = 0; byteorder < 2; ++byteorder )
					{
					// random values
					const uint8 u8 = uint8_rand();
					const uint16 u16 = uint16_rand();
					const uint32 u32 = uint32_rand();
					const uint64 u64 = uint64_rand();
					const float f32 = float_rand();
					const double f64 = double_rand();
					const UUID id = {uint32_rand(), uint16_rand(), uint16_rand(), { uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand() }};
				
					//// random string
					//std::string str;
					//int strl = rand() % 100 + 10;
					//for( int i = 0; i < strl; ++i )
					//	{
					//	char s = (char)(rand() % 60 + 20);
					//	str += s;
					//	}

					// set up a random order of the values to write and read
					const uint num_values = 100000;
					int order[num_values];

					// write random stuff to write stream
					uint64 expected_size = 0;
					MemoryWriteStream *ws = new MemoryWriteStream();
					ws->SetFlipByteOrder( (bool)byteorder );
					for( int i = 0; i < num_values; ++i )
						{
						int item_type = rand() % 7;
						order[i] = item_type;

						switch( item_type )
							{
							case 0:
								ws->Write( u8 ); expected_size += 1;
								break;
							case 1:
								ws->Write( u16 ); expected_size += 2;
								break;
							case 2:
								ws->Write( u32 ); expected_size += 4;
								break;
							case 3:
								ws->Write( u64 ); expected_size += 8;
								break;
							case 4:
								ws->Write( f32 ); expected_size += 4;
								break;
							case 5:
								ws->Write( f64 ); expected_size += 8;
								break;
							case 6:
								ws->Write( id ); expected_size += 16;
								break;
//							case 7:
//								ws->Write( str ); expected_size += (str.length() + 8); // length of string plus 8 bytes for the uint64 size of the string
//								break; 
							}
						}

					// check the expected size
					Assert::IsTrue(expected_size == ws->GetSize());

					// get the data, and set up a read stream
					std::vector<uint8> memdata;
					memdata.resize( expected_size );
					memcpy( memdata.data(), ws->GetData(), expected_size );
					delete ws;
					ws = nullptr;

					// read back everything in the same order
					MemoryReadStream *rs = new MemoryReadStream(memdata.data(),expected_size);
					rs->SetFlipByteOrder( (bool)byteorder );
					for( int i = 0; i < num_values; ++i )
						{
						int item_type = order[i];

						switch( item_type )
							{
							case 0:
								AsserteReadValueIs( rs, u8 );
								break;
							case 1:
								AsserteReadValueIs( rs, u16 );
								break;
							case 2:
								AsserteReadValueIs( rs, u32 );
								break;
							case 3:
								AsserteReadValueIs( rs, u64 );
								break;
							case 4:
								AsserteReadValueIs( rs, f32 );
								break;
							case 5:
								AsserteReadValueIs( rs, f64 );
								break;
							case 6:
								AsserteReadValueIs( rs, id );
								break;
//							case 7:
//								AsserteReadValueIs( rs, str );
//								break; 
							}
						}
		
					Assert::IsTrue( rs->GetPosition() == expected_size );
					delete rs;
					rs = nullptr;
					}
				}

			TEST_METHOD( SHA256Hashing )
				{
				if( true )
					{
					SHA256 sha;

					uint8 testdata[] = {
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

					uint8 calc_hash[32];
					sha.GetDigest( calc_hash );

					uint8 expected_hash[32] = {0xf6,0x48,0x54,0x2d,0xf8,0xcc,0xf2,0x1f,0xd3,0x4e,0x95,0xf6,0x7d,0xf5,0xf2,0xb4,0xf2,0x72,0x72,0xaa,0x14,0xf5,0x03,0x09,0x0c,0xc4,0x76,0x6f,0xe2,0x78,0xc4,0xb5};

					Assert::IsTrue( memcmp( calc_hash, expected_hash, 32 ) == 0 );
					}
				}

			template<class T> void TestEntityWriter_TestWriteValue( const MemoryWriteStream &ws , EntityWriter &ew , const std::vector<std::string> &key_names , const T value )
				{
				const std::string key = key_names[rand() % key_names.size()];
				uint64 curr_pos = ws.GetPosition();
				uint64 expected_pos = curr_pos + 2 + sizeof( value ) + key.size();
				ew.Write( key.c_str(), (uint8)key.size(), value );
				Assert::IsTrue( ws.GetPosition() == expected_pos );
				}

			TEST_METHOD( TestEntityWriter )
				{
				if( true )
					{
					MemoryWriteStream ws;
					EntityWriter ew(ws);

					std::vector<std::string> key_names =
						{
						std::string("PLbYYDnVEpoPO2Yz"),
						std::string("h3HHExIVS4eCngO1UZr4"),
						std::string("At1w2H4jZe"),
						std::string("Hi2I"),
						std::string("uGp3TU67GSkitXB"),
						std::string("c"),
						std::string("PQkmX7Og"),
						std::string("hellofoobar"),
						std::string("ARJVMxS6yF6lasdjllg8jE292A7"),
						std::string("z8ERgfAM8"),
						};

					TestEntityWriter_TestWriteValue<bool>( ws, ew, key_names, true );
					TestEntityWriter_TestWriteValue<bool>( ws, ew, key_names, false );

					TestEntityWriter_TestWriteValue<int8>( ws, ew, key_names, uint8_rand() );
					TestEntityWriter_TestWriteValue<int16>( ws, ew, key_names, uint16_rand() );
					TestEntityWriter_TestWriteValue<int32>( ws, ew, key_names, uint32_rand() );
					TestEntityWriter_TestWriteValue<int64>( ws, ew, key_names, uint64_rand() );

					TestEntityWriter_TestWriteValue<uint8>( ws, ew, key_names, uint8_rand() );
					TestEntityWriter_TestWriteValue<uint16>( ws, ew, key_names, uint16_rand() );
					TestEntityWriter_TestWriteValue<uint32>( ws, ew, key_names, uint32_rand() );
					TestEntityWriter_TestWriteValue<uint64>( ws, ew, key_names, uint64_rand() );

					TestEntityWriter_TestWriteValue<float>( ws, ew, key_names, float_rand() );
					TestEntityWriter_TestWriteValue<double>( ws, ew, key_names, double_rand() );

					TestEntityWriter_TestWriteValue<fvec2>( ws, ew, key_names, fvec2(float_rand(),float_rand()) );
					TestEntityWriter_TestWriteValue<dvec2>( ws, ew, key_names, dvec2(double_rand(),double_rand()) );
					TestEntityWriter_TestWriteValue<fvec3>( ws, ew, key_names, fvec3(float_rand(),float_rand(),float_rand()) );
					TestEntityWriter_TestWriteValue<dvec3>( ws, ew, key_names, dvec3(double_rand(),double_rand(),double_rand()) );
					TestEntityWriter_TestWriteValue<fvec4>( ws, ew, key_names, fvec4(float_rand(),float_rand(),float_rand(),float_rand()) );
					TestEntityWriter_TestWriteValue<dvec4>( ws, ew, key_names, dvec4(double_rand(),double_rand(),double_rand(),double_rand()) );

					TestEntityWriter_TestWriteValue<i8vec2> ( ws, ew, key_names, i8vec2( uint8_rand(), uint8_rand()) );
					TestEntityWriter_TestWriteValue<i16vec2>( ws, ew, key_names, i16vec2(uint16_rand(),uint16_rand()) );
					TestEntityWriter_TestWriteValue<i32vec2>( ws, ew, key_names, i32vec2(uint32_rand(),uint32_rand()) );
					TestEntityWriter_TestWriteValue<i64vec2>( ws, ew, key_names, i64vec2(uint64_rand(),uint64_rand()) );
					TestEntityWriter_TestWriteValue<i8vec3> ( ws, ew, key_names, i8vec3( uint8_rand(), uint8_rand(), uint8_rand()) );
					TestEntityWriter_TestWriteValue<i16vec3>( ws, ew, key_names, i16vec3(uint16_rand(),uint16_rand(),uint16_rand()) );
					TestEntityWriter_TestWriteValue<i32vec3>( ws, ew, key_names, i32vec3(uint32_rand(),uint32_rand(),uint32_rand()) );
					TestEntityWriter_TestWriteValue<i64vec3>( ws, ew, key_names, i64vec3(uint64_rand(),uint64_rand(),uint64_rand()) );
					TestEntityWriter_TestWriteValue<i8vec4> ( ws, ew, key_names, i8vec4( uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand()) );
					TestEntityWriter_TestWriteValue<i16vec4>( ws, ew, key_names, i16vec4(uint16_rand(),uint16_rand(),uint16_rand(),uint16_rand()) );
					TestEntityWriter_TestWriteValue<i32vec4>( ws, ew, key_names, i32vec4(uint32_rand(),uint32_rand(),uint32_rand(),uint32_rand()) );
					TestEntityWriter_TestWriteValue<i64vec4>( ws, ew, key_names, i64vec4(uint64_rand(),uint64_rand(),uint64_rand(),uint64_rand()) );

					TestEntityWriter_TestWriteValue<u8vec2> ( ws, ew, key_names, u8vec2( uint8_rand(), uint8_rand()) );
					TestEntityWriter_TestWriteValue<u16vec2>( ws, ew, key_names, u16vec2(uint16_rand(),uint16_rand()) );
					TestEntityWriter_TestWriteValue<u32vec2>( ws, ew, key_names, u32vec2(uint32_rand(),uint32_rand()) );
					TestEntityWriter_TestWriteValue<u64vec2>( ws, ew, key_names, u64vec2(uint64_rand(),uint64_rand()) );
					TestEntityWriter_TestWriteValue<u8vec3> ( ws, ew, key_names, u8vec3( uint8_rand(), uint8_rand(), uint8_rand()) );
					TestEntityWriter_TestWriteValue<u16vec3>( ws, ew, key_names, u16vec3(uint16_rand(),uint16_rand(),uint16_rand()) );
					TestEntityWriter_TestWriteValue<u32vec3>( ws, ew, key_names, u32vec3(uint32_rand(),uint32_rand(),uint32_rand()) );
					TestEntityWriter_TestWriteValue<u64vec3>( ws, ew, key_names, u64vec3(uint64_rand(),uint64_rand(),uint64_rand()) );
					TestEntityWriter_TestWriteValue<u8vec4> ( ws, ew, key_names, u8vec4( uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand()) );
					TestEntityWriter_TestWriteValue<u16vec4>( ws, ew, key_names, u16vec4(uint16_rand(),uint16_rand(),uint16_rand(),uint16_rand()) );
					TestEntityWriter_TestWriteValue<u32vec4>( ws, ew, key_names, u32vec4(uint32_rand(),uint32_rand(),uint32_rand(),uint32_rand()) );
					TestEntityWriter_TestWriteValue<u64vec4>( ws, ew, key_names, u64vec4(uint64_rand(),uint64_rand(),uint64_rand(),uint64_rand()) );

					TestEntityWriter_TestWriteValue<fmat2>( ws, ew, key_names, fmat2( 
						float_rand(), float_rand(), 
						float_rand(), float_rand() 
					) );
					TestEntityWriter_TestWriteValue<dmat2>( ws, ew, key_names, dmat2( 
						double_rand(), double_rand(), 
						double_rand(), double_rand() 
					) );
					TestEntityWriter_TestWriteValue<fmat3>( ws, ew, key_names, fmat3( 
						float_rand(), float_rand(), float_rand(), 
						float_rand(), float_rand(), float_rand(), 
						float_rand(), float_rand(), float_rand() 
					) );
					TestEntityWriter_TestWriteValue<dmat3>( ws, ew, key_names, dmat3( 
						double_rand(), double_rand(), double_rand(), 
						double_rand(), double_rand(), double_rand(), 
						double_rand(), double_rand(), double_rand() 
					) );
					TestEntityWriter_TestWriteValue<fmat4>( ws, ew, key_names, fmat4( 
						float_rand(), float_rand(), float_rand(), float_rand(), 
						float_rand(), float_rand(), float_rand(), float_rand(), 
						float_rand(), float_rand(), float_rand(), float_rand(), 
						float_rand(), float_rand(), float_rand(), float_rand() 
					) );
					TestEntityWriter_TestWriteValue<dmat4>( ws, ew, key_names, dmat4( 
						double_rand(), double_rand(), double_rand(), double_rand(), 
						double_rand(), double_rand(), double_rand(), double_rand(), 
						double_rand(), double_rand(), double_rand(), double_rand(), 
						double_rand(), double_rand(), double_rand(), double_rand() 
					) );

					TestEntityWriter_TestWriteValue<UUID>( ws, ew, key_names, uuid_rand() );
					}
				}

		};
	}














