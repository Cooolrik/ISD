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
		template<class T> void TestEntityWriter_TestValueType( const MemoryWriteStream &ws, EntityWriter &ew, const std::vector<std::string> &key_names)
			{
			const T value = random_value<T>();
			const optional_value<T> opt_value = optional_random_value<T>();
			const std::vector<T> value_vec = random_vector<T>();
			const optional_value<std::vector<T>> opt_value_vec = optional_random_vector<T>();
			const indexed_array<T> value_inxarr = random_indexed_array<T>();
			const optional_value<indexed_array<T>> opt_value_inxarr(optional_random_indexed_array<T>());

			const std::string key = key_names[rand() % key_names.size()];

			// write value
			u64 start_pos = ws.GetPosition();
			u64 expected_pos = start_pos + 2 + sizeof( value ) + key.size();
			bool write_successfully = ew.Write<T>( key.c_str(), (u8)key.size(), value );
			Assert::IsTrue( write_successfully );

			// write an optional value
			write_successfully = ew.Write<optional_value<T>>( key.c_str(), (u8)key.size(), opt_value );
			Assert::IsTrue( write_successfully );

			// write a random vector of values
			write_successfully = ew.Write<std::vector<T>>( key.c_str(), (u8)key.size(), value_vec );
			Assert::IsTrue( write_successfully );

			// write an optional random vector of values
			write_successfully = ew.Write<optional_value<std::vector<T>>>( key.c_str(), (u8)key.size(), opt_value_vec );
			Assert::IsTrue( write_successfully );

			// write a random indexed array of values
			write_successfully = ew.Write<indexed_array<T>>( key.c_str(), (u8)key.size(), value_inxarr );
			Assert::IsTrue( write_successfully );

			// write an optional random indexed array of values
			write_successfully = ew.Write<optional_value<indexed_array<T>>>( key.c_str(), (u8)key.size(), opt_value_inxarr );
			Assert::IsTrue( write_successfully );

			// set up a temporary entity reader and read back the values
			MemoryReadStream rs( ws.GetData(), ws.GetSize(), ws.GetFlipByteOrder() );
			EntityReader er( rs );
			rs.SetPosition( start_pos );

			// read back value
			T read_back_value;
			bool read_successfully = er.Read( key.c_str() , (u8)key.size(), read_back_value );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( value == read_back_value );

			// read back optional value
			optional_value<T> read_back_opt_value;
			read_successfully = er.Read( key.c_str() , (u8)key.size(), read_back_opt_value );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( opt_value.has_value() == read_back_opt_value.has_value() );
			if( opt_value.has_value() )
				{
				Assert::IsTrue( opt_value.value() == read_back_opt_value.value() );
				}

			// read back vector of values
			std::vector<T> read_back_value_vec;
			read_successfully = er.Read( key.c_str() , (u8)key.size(), read_back_value_vec );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( value_vec == read_back_value_vec );

			// read back optional vector of values
			optional_value<std::vector<T>> read_back_opt_value_vec;
			read_successfully = er.Read( key.c_str() , (u8)key.size(), read_back_opt_value_vec );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( opt_value_vec.has_value() == read_back_opt_value_vec.has_value() );
			if( opt_value_vec.has_value() )
				{
				Assert::IsTrue( opt_value_vec.value() == read_back_opt_value_vec.value() );
				}

			// read back indexed_array of values
			indexed_array<T> read_back_value_inxarr;
			read_successfully = er.Read( key.c_str() , (u8)key.size(), read_back_value_inxarr );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( value_inxarr.values() == read_back_value_inxarr.values() );
			Assert::IsTrue( value_inxarr.index() == read_back_value_inxarr.index() );

			// read back optional indexed_array of values
			optional_value<indexed_array<T>> read_back_opt_value_inxarr;
			read_successfully = er.Read( key.c_str() , (u8)key.size(), read_back_opt_value_inxarr );
			Assert::IsTrue( read_successfully );
			Assert::IsTrue( read_back_opt_value_inxarr.has_value() == read_back_opt_value_inxarr.has_value() );
			if( opt_value_inxarr.has_value() )
				{
				Assert::IsTrue( opt_value_inxarr.value().values() == read_back_opt_value_inxarr.value().values() );
				Assert::IsTrue( opt_value_inxarr.value().index() == read_back_opt_value_inxarr.value().index() );
				}


			}

		TEST_METHOD( TestEntityWriterAndReadback )
			{
			random_seed(0);

			for( uint swap_byte_order_flag=0; swap_byte_order_flag<2; ++swap_byte_order_flag )
				{

				MemoryWriteStream ws;
				EntityWriter ew( ws );

				ws.SetFlipByteOrder( (swap_byte_order_flag & 0x1) != 0 );

				if( ws.GetFlipByteOrder() )
					Logger::WriteMessage("Testing native byte order");
				else
					Logger::WriteMessage("Testing flipped byte order");

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

				TestEntityWriter_TestValueType<bool>( ws, ew, key_names );
				
				TestEntityWriter_TestValueType<i8>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i16>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i32>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i64>( ws, ew, key_names );

				TestEntityWriter_TestValueType<u8>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u16>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u32>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u64>( ws, ew, key_names );

				TestEntityWriter_TestValueType<float>( ws, ew, key_names );
				TestEntityWriter_TestValueType<double>( ws, ew, key_names );

				TestEntityWriter_TestValueType<fvec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<dvec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<fvec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<dvec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<fvec4>( ws, ew, key_names );
				TestEntityWriter_TestValueType<dvec4>( ws, ew, key_names );

				TestEntityWriter_TestValueType<i8vec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i16vec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i32vec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i64vec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i8vec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i16vec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i32vec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i64vec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i8vec4>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i16vec4>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i32vec4>( ws, ew, key_names );
				TestEntityWriter_TestValueType<i64vec4>( ws, ew, key_names );

				TestEntityWriter_TestValueType<u8vec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u16vec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u32vec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u64vec2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u8vec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u16vec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u32vec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u64vec3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u8vec4>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u16vec4>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u32vec4>( ws, ew, key_names );
				TestEntityWriter_TestValueType<u64vec4>( ws, ew, key_names );

				TestEntityWriter_TestValueType<fmat2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<dmat2>( ws, ew, key_names );
				TestEntityWriter_TestValueType<fmat3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<dmat3>( ws, ew, key_names );
				TestEntityWriter_TestValueType<fmat4>( ws, ew, key_names );
				TestEntityWriter_TestValueType<dmat4>( ws, ew, key_names );

				TestEntityWriter_TestValueType<UUID>( ws, ew, key_names );
				TestEntityWriter_TestValueType<string>( ws, ew, key_names );
				}
			}
		};
	}