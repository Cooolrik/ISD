// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\ISD\ISD_MemoryReadStream.h"
#include "..\ISD\ISD_MemoryWriteStream.h"
#include "..\ISD\ISD_EntityWriter.h"
#include "..\ISD\ISD_EntityReader.h"

namespace UnitTests
	{
	TEST_CLASS( EntityReadWriteTests )
		{

		template<class T> void TestEntityWriter_TestWriteValue( const MemoryWriteStream &ws, EntityWriter &ew, const std::vector<std::string> &key_names, const T value )
			{
			// write 
			const std::string key = key_names[rand() % key_names.size()];
			uint64 start_pos = ws.GetPosition();
			uint64 expected_pos = start_pos + 2 + sizeof( value ) + key.size();
			bool write_successfully = ew.Write( key.c_str(), (uint8)key.size(), value );
			Assert::IsTrue( ws.GetPosition() == expected_pos );
			Assert::IsTrue( write_successfully );
			
			// set up a temporary entity reader and read back the value
			MemoryReadStream rs( ws.GetData(), ws.GetSize(), ws.GetFlipByteOrder() );
			EntityReader er( rs );
			rs.SetPosition( start_pos );
			T read_back_value;
			bool read_successfully = er.Read( key.c_str() , (uint8)key.size(), read_back_value );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( memcmp( &value, &read_back_value, sizeof( value ) ) == 0 );
			}

		void TestEntityWriter_TestWriteNull( const MemoryWriteStream &ws, EntityWriter &ew, const std::vector<std::string> &key_names )
			{
			// write 
			const std::string key = key_names[rand() % key_names.size()];
			uint64 start_pos = ws.GetPosition();
			uint64 expected_pos = start_pos + 2 + key.size();
			bool write_successfully = ew.WriteNull( key.c_str(), (uint8)key.size() );
			Assert::IsTrue( ws.GetPosition() == expected_pos );
			Assert::IsTrue( write_successfully );

			// set up a temporary entity reader and read back the value
			MemoryReadStream rs( ws.GetData(), ws.GetSize(), ws.GetFlipByteOrder() );
			EntityReader er( rs );
			rs.SetPosition( start_pos );
			bool read_successfully = er.ReadNull( key.c_str() , (uint8)key.size() );
			Assert::IsTrue( read_successfully );
			}

		TEST_METHOD( TestEntityWriter )
			{
			for( uint swap_byte_order_flag=0; swap_byte_order_flag<2; ++swap_byte_order_flag )
				{
				MemoryWriteStream ws;
				EntityWriter ew( ws );

				ws.SetFlipByteOrder( swap_byte_order_flag != 0 );

				std::vector<std::string> key_names =
					{
					std::string( "PLbYYDnVEpoPO2Yz" ),
					std::string( "h3HHExIVS4eCngO1UZr4" ),
					std::string( "At1w2H4jZe" ),
					std::string( "Hi2I" ),
					std::string( "uGp3TU67GSkitXB" ),
					std::string( "c" ),
					std::string( "PQkmX7Og" ),
					std::string( "hellofoobar" ),
					std::string( "ARJVMxS6yF6lasdjllg8jE292A7" ),
					std::string( "z8ERgfAM8" ),
					};

				TestEntityWriter_TestWriteNull( ws, ew, key_names );

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

				TestEntityWriter_TestWriteValue<fvec2>( ws, ew, key_names, fvec2( float_rand(), float_rand() ) );
				TestEntityWriter_TestWriteValue<dvec2>( ws, ew, key_names, dvec2( double_rand(), double_rand() ) );
				TestEntityWriter_TestWriteValue<fvec3>( ws, ew, key_names, fvec3( float_rand(), float_rand(), float_rand() ) );
				TestEntityWriter_TestWriteValue<dvec3>( ws, ew, key_names, dvec3( double_rand(), double_rand(), double_rand() ) );
				TestEntityWriter_TestWriteValue<fvec4>( ws, ew, key_names, fvec4( float_rand(), float_rand(), float_rand(), float_rand() ) );
				TestEntityWriter_TestWriteValue<dvec4>( ws, ew, key_names, dvec4( double_rand(), double_rand(), double_rand(), double_rand() ) );

				TestEntityWriter_TestWriteValue<i8vec2>( ws, ew, key_names, i8vec2( uint8_rand(), uint8_rand() ) );
				TestEntityWriter_TestWriteValue<i16vec2>( ws, ew, key_names, i16vec2( uint16_rand(), uint16_rand() ) );
				TestEntityWriter_TestWriteValue<i32vec2>( ws, ew, key_names, i32vec2( uint32_rand(), uint32_rand() ) );
				TestEntityWriter_TestWriteValue<i64vec2>( ws, ew, key_names, i64vec2( uint64_rand(), uint64_rand() ) );
				TestEntityWriter_TestWriteValue<i8vec3>( ws, ew, key_names, i8vec3( uint8_rand(), uint8_rand(), uint8_rand() ) );
				TestEntityWriter_TestWriteValue<i16vec3>( ws, ew, key_names, i16vec3( uint16_rand(), uint16_rand(), uint16_rand() ) );
				TestEntityWriter_TestWriteValue<i32vec3>( ws, ew, key_names, i32vec3( uint32_rand(), uint32_rand(), uint32_rand() ) );
				TestEntityWriter_TestWriteValue<i64vec3>( ws, ew, key_names, i64vec3( uint64_rand(), uint64_rand(), uint64_rand() ) );
				TestEntityWriter_TestWriteValue<i8vec4>( ws, ew, key_names, i8vec4( uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand() ) );
				TestEntityWriter_TestWriteValue<i16vec4>( ws, ew, key_names, i16vec4( uint16_rand(), uint16_rand(), uint16_rand(), uint16_rand() ) );
				TestEntityWriter_TestWriteValue<i32vec4>( ws, ew, key_names, i32vec4( uint32_rand(), uint32_rand(), uint32_rand(), uint32_rand() ) );
				TestEntityWriter_TestWriteValue<i64vec4>( ws, ew, key_names, i64vec4( uint64_rand(), uint64_rand(), uint64_rand(), uint64_rand() ) );

				TestEntityWriter_TestWriteValue<u8vec2>( ws, ew, key_names, u8vec2( uint8_rand(), uint8_rand() ) );
				TestEntityWriter_TestWriteValue<u16vec2>( ws, ew, key_names, u16vec2( uint16_rand(), uint16_rand() ) );
				TestEntityWriter_TestWriteValue<u32vec2>( ws, ew, key_names, u32vec2( uint32_rand(), uint32_rand() ) );
				TestEntityWriter_TestWriteValue<u64vec2>( ws, ew, key_names, u64vec2( uint64_rand(), uint64_rand() ) );
				TestEntityWriter_TestWriteValue<u8vec3>( ws, ew, key_names, u8vec3( uint8_rand(), uint8_rand(), uint8_rand() ) );
				TestEntityWriter_TestWriteValue<u16vec3>( ws, ew, key_names, u16vec3( uint16_rand(), uint16_rand(), uint16_rand() ) );
				TestEntityWriter_TestWriteValue<u32vec3>( ws, ew, key_names, u32vec3( uint32_rand(), uint32_rand(), uint32_rand() ) );
				TestEntityWriter_TestWriteValue<u64vec3>( ws, ew, key_names, u64vec3( uint64_rand(), uint64_rand(), uint64_rand() ) );
				TestEntityWriter_TestWriteValue<u8vec4>( ws, ew, key_names, u8vec4( uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand() ) );
				TestEntityWriter_TestWriteValue<u16vec4>( ws, ew, key_names, u16vec4( uint16_rand(), uint16_rand(), uint16_rand(), uint16_rand() ) );
				TestEntityWriter_TestWriteValue<u32vec4>( ws, ew, key_names, u32vec4( uint32_rand(), uint32_rand(), uint32_rand(), uint32_rand() ) );
				TestEntityWriter_TestWriteValue<u64vec4>( ws, ew, key_names, u64vec4( uint64_rand(), uint64_rand(), uint64_rand(), uint64_rand() ) );

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