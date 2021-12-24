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
	class section_array;

	class section_object
		{
		public:
			optional_value<std::string> object_name;
			mat4 object_transform = {};
			std::unique_ptr<section_object> sub;

			std::vector< std::unique_ptr<section_object> > vec;
			optional_idx_vector< std::unique_ptr<section_object> > opt_arr;

			bool Setup( int levels_left );
			uint CountItems() const;
			bool Write( const MemoryWriteStream &ws, EntityWriter &ew );
		};

	bool section_object::Setup( int levels_left )
		{
		if( levels_left < 0 )
			return true;

		if( random_value<bool>() )
			{
			object_name.set( random_value<std::string>() );
			}

		object_transform = random_value<mat4>();

		sub = std::make_unique<UnitTests::section_object>();
		sub->Setup( levels_left - 1 );

		random_vector<std::unique_ptr<section_object>>( this->vec, 0, 10 );
		for( size_t i = 0; i < vec.size(); ++i )
			{
			if( vec[i] ) { vec[i]->Setup( levels_left - 1 ); }
			}

		random_optional_idx_vector<std::unique_ptr<section_object>>( this->opt_arr, 0, 10 );
		if( opt_arr.has_value() )
			{
			for( size_t i = 0; i < opt_arr.values().size(); ++i )
				{
				if( opt_arr.values()[i] ) { opt_arr.values()[i]->Setup( levels_left - 1 ); }
				}
			}

		return true;
		}

	uint section_object::CountItems() const
		{
		uint count = 1; // start with this item

		if( sub ) { count += sub->CountItems(); }

		for( size_t i = 0; i < this->vec.size(); ++i )
			{
			if( this->vec[i] ) { count += this->vec[i]->CountItems(); }
			}

		if( this->opt_arr.has_value() )
			{
			const auto &vals = opt_arr.values();
			for( size_t i = 0; i < vals.size(); ++i )
				{
				if( vals[i] ) { count += vals[i]->CountItems(); }
				}
			}

		return count;
		}

	bool section_object::Write( const MemoryWriteStream &ws, EntityWriter &ew )
		{
		Assert::IsTrue( ew.Write( "object_name", 11, object_name ) );

		Assert::IsTrue( ew.Write( "object_transform", 16, object_transform ) );

		if( sub )
			{
			EntityWriter *section_writer = ew.BeginSection( "sub", 3 );
			Assert::IsTrue( section_writer != nullptr );
			if( section_writer )
				{
				Assert::IsTrue( sub->Write( ws, *section_writer ) );
				Assert::IsTrue( ew.EndSection( section_writer ) );
				}
			}
		else
			{
			ew.WriteNullSection( "sub", 3 );
			}

		EntityWriter *section_array_writer = ew.BeginSectionsArray( "vec", 3, vec.size() );
		Assert::IsTrue( section_array_writer != nullptr );
		if( section_array_writer )
			{
			for( size_t i = 0; i < vec.size(); ++i )
				{
				Assert::IsTrue( ew.BeginSectionInArray( section_array_writer, i ) );
				if( vec[i] )
					{
					Assert::IsTrue( vec[i]->Write( ws, *section_array_writer ) );
					}
				Assert::IsTrue( ew.EndSectionInArray( section_array_writer, i ) );
				}

			Assert::IsTrue( ew.EndSectionsArray( section_array_writer ) );
			}

		if( opt_arr.has_value() )
			{
			EntityWriter *section_array_writer = ew.BeginSectionsArray( "opt_arr", 7, vec.size(), &opt_arr.index() );
			Assert::IsTrue( section_array_writer != nullptr );
			if( section_array_writer )
				{
				for( size_t i = 0; i < vec.size(); ++i )
					{
					Assert::IsTrue( ew.BeginSectionInArray( section_array_writer, i ) );
					if( vec[i] )
						{
						Assert::IsTrue( vec[i]->Write( ws, *section_array_writer ) );
						}
					Assert::IsTrue( ew.EndSectionInArray( section_array_writer, i ) );
					}

				Assert::IsTrue( ew.EndSectionsArray( section_array_writer ) );
				}
			}
		else
			{
			Assert::IsTrue( ew.WriteNullSectionsArray( "opt_arr", 7 ) );
			}

		return true;
		}

	TEST_CLASS( SectionHierarchyReadWriteTests )
		{
		TEST_METHOD( TestEntitySectionWriterAndReadback )
			{
			setup_random_seed();

			for( uint pass_index = 0; pass_index < global_number_of_passes; ++pass_index )
				{
				std::stringstream ss;
				ss << "Pass #" << pass_index << "\n";
				Logger::WriteMessage( ss.str().c_str() );

				section_object my_hierarchy;
				Assert::IsTrue( my_hierarchy.Setup( (int)capped_rand( 1, 3 ) ) );

				ss = std::stringstream();
				ss << "\tCreated hierarchy, size:" << my_hierarchy.CountItems() << "\n";
				Logger::WriteMessage( ss.str().c_str() );

				MemoryWriteStream ws;
				EntityWriter ew( ws );

				Assert::IsTrue( my_hierarchy.Write( ws, ew ) );

				ss = std::stringstream();
				ss << "\tWrote hierarchy, size of stream: " << ws.GetPosition() << "\n";
				Logger::WriteMessage( ss.str().c_str() );

				}
			}
		};
	}

	// implement the random value function for std::unique_ptr<UnitTests::section_object>
	template<> std::unique_ptr<UnitTests::section_object> random_value< std::unique_ptr<UnitTests::section_object> >()
		{
		if( random_value<bool>() )
			{
			return std::make_unique<UnitTests::section_object>();
			}
		return std::unique_ptr<UnitTests::section_object>( nullptr );
		}
