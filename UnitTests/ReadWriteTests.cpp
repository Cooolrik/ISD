// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\ISD\ISD_MemoryReadStream.h"
#include "..\ISD\ISD_MemoryWriteStream.h"


namespace UnitTests
	{
	TEST_CLASS( ReadWriteTests )
		{
		template<class T> void AsserteReadValueIs( MemoryReadStream *rs, T ref_value )
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
						}
					}

				// check the expected size
				Assert::IsTrue( expected_size == ws->GetSize() );

				// get the data, and set up a read stream
				std::vector<uint8> memdata;
				memdata.resize( expected_size );
				memcpy( memdata.data(), ws->GetData(), expected_size );
				delete ws;
				ws = nullptr;

				// read back everything in the same order
				MemoryReadStream *rs = new MemoryReadStream( memdata.data(), expected_size );
				rs->SetFlipByteOrder( (bool)byteorder );
				for( int i = 0; i < num_values; ++i )
					{
					int item_type = order[i];

					switch( item_type )
						{
						case 0:
							Assert::IsTrue( rs->Peek() == u8 ); // also test Peek functionality
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
						}
					}

				Assert::IsTrue( rs->GetPosition() == expected_size );
				delete rs;
				rs = nullptr;
				}
			}

		};
	}