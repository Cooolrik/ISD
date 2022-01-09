// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\ISD\ISD_MemoryReadStream.h"
#include "..\ISD\ISD_MemoryWriteStream.h"
#include "..\ISD\ISD_EntityWriter.h"
#include "..\ISD\ISD_EntityReader.h"
#include "..\ISD\ISD_Dictionary.h"
#include "..\ISD\ISD_EntityValidator.h"

namespace UnitTests
	{
	class TestEntity
		{
		std::string Name;

		public:
			class MF;
			friend MF;

			void SetName( std::string &name ) { this->Name = name; }
			std::string GetName() { return this->Name; }
		};

	class TestEntity::MF
		{
		using _MgmCl = TestEntity;

		public:
			static bool Write( _MgmCl &obj , EntityWriter &writer )
				{
				if( !writer.Write( ISDKeyMacro( "Name" ), obj.Name ) )
					return false;
				return true;
				}

			static bool Read( _MgmCl &obj , EntityReader &reader )
				{
				if( !reader.Read( ISDKeyMacro( "Name" ), obj.Name ) )
					return false;
				return true;
				}

			static bool Validate( _MgmCl &obj )
				{
				return true;
				}
		};

	TEST_CLASS( DictionaryTests )
		{
		template<class T> void DictionaryReadWriteTests_TestKeyType( const MemoryWriteStream &ws, EntityWriter &ew )
			{
			typedef Dictionary<T, TestEntity> Dict;

			Dict random_dict;

			size_t dict_size = capped_rand( 0, 100 );

			// create random dictionary with random entries (half of them null)
			for( size_t i = 0; i < dict_size; ++i )
				{
				T key = random_value<T>();

				if( random_value<bool>() )
					{
					random_dict.GetEntities()[key] = std::make_unique<TestEntity>();
					random_dict.GetEntities()[key]->SetName( random_value<std::string>() );
					}
				else
					{
					random_dict.GetEntities().emplace( key, nullptr );
					}
				}

			EntityValidator validator;

			// validate
			Assert::IsTrue( Dict::MF::Validate( random_dict , validator ) );

			// write dictionary to stream
			u64 start_pos = ws.GetPosition();
			Assert::IsTrue( Dict::MF::Write( random_dict , ew ) );

			// set up a temporary entity reader 
			MemoryReadStream rs( ws.GetData(), ws.GetSize(), ws.GetFlipByteOrder() );
			EntityReader er( rs );
			rs.SetPosition( start_pos );
			
			// read back the dictionary 
			Dict readback_dict;
			Assert::IsTrue( Dict::MF::Read( readback_dict , er ) );
			
			// validate
			Assert::IsTrue( Dict::MF::Validate( readback_dict , validator ) );

			// compare the values in the dictionaries
			Assert::IsTrue( random_dict.GetEntities().size() == readback_dict.GetEntities().size() );
			Dict::iterator it1 = random_dict.GetEntities().begin();
			Dict::iterator it2 = readback_dict.GetEntities().begin();
			while( it1 != random_dict.GetEntities().end() )
				{
				bool has_1 = it1->second != nullptr;
				bool has_2 = it2->second != nullptr;
			
				Assert::IsTrue( has_1 == has_2 );
				if( has_1 )
					{
					Assert::IsTrue( it1->second->GetName() == it2->second->GetName() );
					}
			
				++it1;
				++it2;
				}
			}

		TEST_METHOD( DictionaryReadWriteTests )
			{
			setup_random_seed();

			for( uint pass_index=0; pass_index<(2*global_number_of_passes); ++pass_index )
				{
				MemoryWriteStream ws;
				EntityWriter ew( ws );

				ws.SetFlipByteOrder( (pass_index & 0x1) != 0 );

				// log the pass
				std::stringstream ss;
				ss << "Pass #" << (pass_index / 2)+1 << " ";
				if( ws.GetFlipByteOrder() )
					ss << "Testing native byte order\n";
				else
					ss << "Testing flipped byte order\n";
				Logger::WriteMessage(ss.str().c_str());

				DictionaryReadWriteTests_TestKeyType<i8>( ws, ew );
				DictionaryReadWriteTests_TestKeyType<i16>( ws, ew );
				DictionaryReadWriteTests_TestKeyType<i32>( ws, ew );
				DictionaryReadWriteTests_TestKeyType<i64>( ws, ew );
				
				DictionaryReadWriteTests_TestKeyType<u8>( ws, ew );
				DictionaryReadWriteTests_TestKeyType<u16>( ws, ew );
				DictionaryReadWriteTests_TestKeyType<u32>( ws, ew );
				DictionaryReadWriteTests_TestKeyType<u64>( ws, ew );
				
				DictionaryReadWriteTests_TestKeyType<float>( ws, ew );
				DictionaryReadWriteTests_TestKeyType<double>( ws, ew );
								
				DictionaryReadWriteTests_TestKeyType<UUID>( ws, ew );
				DictionaryReadWriteTests_TestKeyType<string>( ws, ew );
				}
			}

		};
	}
