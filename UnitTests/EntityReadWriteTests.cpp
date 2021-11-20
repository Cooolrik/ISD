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

		template<class T> void TestEntityWriter_TestWriteValue( const MemoryWriteStream &ws, EntityWriter &ew, const std::vector<std::string> &key_names)
			{
			const T value = random_value<T>();

			// write 
			const std::string key = key_names[rand() % key_names.size()];
			uint64 start_pos = ws.GetPosition();
			uint64 expected_pos = start_pos + 2 + sizeof( value ) + key.size();
			bool write_successfully = ew.Write( key.c_str(), (uint8)key.size(), value );
			Assert::IsTrue( ws.GetPosition() == expected_pos );
			Assert::IsTrue( write_successfully );

			// write an optional version of the value
			optional_value<T> opt_value;
			if( rand() % 2 == 0 )
				opt_value.set( value );
			else
				opt_value.clear();
			write_successfully = ew.Write( key.c_str(), (uint8)key.size(), opt_value );
			Assert::IsTrue( write_successfully );

			// set up a temporary entity reader and read back the values
			MemoryReadStream rs( ws.GetData(), ws.GetSize(), ws.GetFlipByteOrder() );
			EntityReader er( rs );
			rs.SetPosition( start_pos );
			T read_back_value;
			bool read_successfully = er.Read( key.c_str() , (uint8)key.size(), read_back_value );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( memcmp( &value, &read_back_value, sizeof( value ) ) == 0 );

			// read back the optional value as well
			optional_value<T> read_back_opt_value;
			read_successfully = er.Read( key.c_str() , (uint8)key.size(), read_back_opt_value );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( opt_value.has_value() == read_back_opt_value.has_value() );
			if( opt_value.has_value() )
				{
				const T val1 = opt_value.value().second;
				const T val2 = read_back_opt_value.value().second;
				Assert::IsTrue( memcmp( &val1, &val2, sizeof( val1 ) ) == 0 );
				}
			}

		template<class T> void TestEntityWriter_TestWriteVector( const MemoryWriteStream &ws, EntityWriter &ew, const std::vector<std::string> &key_names )
			{
			indexed_array<T> arr = random_array<T>();

			// write 
			const std::string key = key_names[rand() % key_names.size()];
			uint64 start_pos = ws.GetPosition();
			//uint64 expected_pos = start_pos + 2 + sizeof( value ) + key.size();
			bool write_successfully = ew.Write( key.c_str(), (uint8)key.size(), arr );
			//Assert::IsTrue( ws.GetPosition() == expected_pos );
			Assert::IsTrue( write_successfully );




			}
		TEST_METHOD( TestEntityWriterAndReadback )
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

				TestEntityWriter_TestWriteValue<bool>( ws, ew, key_names );

				TestEntityWriter_TestWriteValue<int8>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<int16>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<int32>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<int64>( ws, ew, key_names );

				TestEntityWriter_TestWriteValue<uint8>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<uint16>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<uint32>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<uint64>( ws, ew, key_names );

				TestEntityWriter_TestWriteValue<float>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<double>( ws, ew, key_names );

				TestEntityWriter_TestWriteValue<fvec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<dvec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<fvec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<dvec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<fvec4>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<dvec4>( ws, ew, key_names );

				TestEntityWriter_TestWriteValue<i8vec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i16vec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i32vec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i64vec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i8vec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i16vec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i32vec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i64vec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i8vec4>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i16vec4>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i32vec4>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<i64vec4>( ws, ew, key_names );

				TestEntityWriter_TestWriteValue<u8vec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u16vec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u32vec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u64vec2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u8vec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u16vec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u32vec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u64vec3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u8vec4>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u16vec4>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u32vec4>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<u64vec4>( ws, ew, key_names );

				TestEntityWriter_TestWriteValue<fmat2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<dmat2>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<fmat3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<dmat3>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<fmat4>( ws, ew, key_names );
				TestEntityWriter_TestWriteValue<dmat4>( ws, ew, key_names );

				TestEntityWriter_TestWriteValue<UUID>( ws, ew, key_names );

				//TestEntityWriter_TestWriteVector<int8>( ws, ew, key_names );
				}
			}
		};
	}