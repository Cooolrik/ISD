// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\ISD\ISD_SHA256.h"

namespace TypeTests
	{
	TEST_CLASS(TypeTests)
		{
		STANDARD_TEST_INIT()

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
			Assert::IsTrue(sizeof(hash) == 32);

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
			uuid uuidval = { 0x12345678 , 0xa1a2 , 0xb1b2 , { 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8 } };
			std::string expected_hexuuidval = "12345678-a1a2-b1b2-c1c2-c3c4c5c6c7c8";
			Assert::IsTrue( value_to_hex_string( uuidval ) == expected_hexuuidval );

			const u8 hashdata[32] = {
				0xf6,0x48,0x54,0x2d,0xf8,0xcc,0xf2,0x1f,
				0xd3,0x4e,0x95,0xf6,0x7d,0xf5,0xf2,0xb4,
				0xf2,0x72,0x72,0xaa,0x14,0xf5,0x03,0x09,
				0x0c,0xc4,0x76,0x6f,0xe2,0x78,0xc4,0xb5
				};
			hash hashval;
			memcpy( &hashval, hashdata, 32 );
			std::string expected_hexhashval = "f648542df8ccf21fd34e95f67df5f2b4f27272aa14f503090cc4766fe278c4b5";
			Assert::IsTrue( value_to_hex_string( hashval ) == expected_hexhashval );

			u8 u8val = 0x13;
			std::string expected_hexu8 = "13";
			Assert::IsTrue( value_to_hex_string( u8val ) == expected_hexu8 );

			u16 u16val = 0x1234;
			std::string expected_hexu16 = "1234";
			Assert::IsTrue( value_to_hex_string( u16val ) == expected_hexu16 );

			u32 u32val = 0x0218a782;
			std::string expected_hexu32 = "0218a782";
			Assert::IsTrue( value_to_hex_string( u32val ) == expected_hexu32 );

			u64 u64val = 0x35023218a7828505;
			std::string expected_hexu64 = "35023218a7828505";
			Assert::IsTrue( value_to_hex_string( u64val ) == expected_hexu64 );
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

				u8 expected_hash[32] = {
					0xf6,0x48,0x54,0x2d,0xf8,0xcc,0xf2,0x1f,
					0xd3,0x4e,0x95,0xf6,0x7d,0xf5,0xf2,0xb4,
					0xf2,0x72,0x72,0xaa,0x14,0xf5,0x03,0x09,
					0x0c,0xc4,0x76,0x6f,0xe2,0x78,0xc4,0xb5
					};

				Assert::IsTrue( memcmp( calc_hash, expected_hash, 32 ) == 0 );
				}
			}

		TEST_METHOD( Test_entity_ref )
			{
			entity_ref ref;
			Assert::IsTrue( !ref );
			uuid val = ref;
			Assert::IsTrue( val == uuid_zero );
			Assert::IsTrue( val == entity_ref::null() );
			Assert::IsTrue( ref == entity_ref() );

			entity_ref ref2 = entity_ref::make_ref();
			Assert::IsTrue( ref != ref2 );
			Assert::IsTrue( !(ref == ref2) );
			Assert::IsTrue( ref < ref2 ); // ref is zero, ref2 must be more 
			val = ref2;
			Assert::IsTrue( val != uuid_zero );
			Assert::IsTrue( uuid_zero < val );

			ref = std::move( ref2 );
			Assert::IsTrue( ref != ref2 );
			Assert::IsTrue( !(ref == ref2) );
			Assert::IsTrue( ref2 < ref ); // ref2 is zero, ref must be more 

			ref2 = ref;
			Assert::IsTrue( ref == ref2 );
			Assert::IsTrue( !(ref != ref2) );
			Assert::IsTrue( !(ref2 < ref) ); 
			}

		TEST_METHOD( Test_idx_vector )
			{
			idx_vector<uuid> vec;
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );

			for( size_t i = 0; i < 10; ++i )
				{
				vec.index().emplace_back( (uint)i*2+0 );
				vec.index().emplace_back( (uint)i*2+1 );
				vec.values().emplace_back( random_value<uuid>() );
				}
			Assert::IsTrue( vec.index().size() == 20 );
			Assert::IsTrue( vec.values().size() == 10 );

			idx_vector<uuid> vec2 = std::move(vec);
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec2.index().size() == 20 );
			Assert::IsTrue( vec2.values().size() == 10 );
			Assert::IsTrue( vec.index() != vec2.index() );
			Assert::IsTrue( vec.values() != vec2.values() );
			Assert::IsTrue( vec != vec2 );
			Assert::IsTrue( !(vec == vec2) );

			vec = vec2;
			Assert::IsTrue( vec.index().size() == 20 );
			Assert::IsTrue( vec.values().size() == 10 );
			Assert::IsTrue( vec.index().size() == vec2.index().size() );
			Assert::IsTrue( vec.values().size() == vec2.values().size() );
			Assert::IsTrue( vec.index() == vec2.index() );
			Assert::IsTrue( vec.values() == vec2.values() );
			Assert::IsTrue( vec == vec2 );
			Assert::IsTrue( !(vec != vec2) );

			vec.clear();
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec.index() != vec2.index() );
			Assert::IsTrue( vec.values() != vec2.values() );

			vec2.clear();
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec2.index().size() == 0 );
			Assert::IsTrue( vec2.values().size() == 0 );
			Assert::IsTrue( vec.index() == vec2.index() );
			Assert::IsTrue( vec.values() == vec2.values() );
			Assert::IsTrue( vec == vec2 );
			Assert::IsTrue( !(vec != vec2) );
			}

		TEST_METHOD( Test_optional_idx_vector )
			{
			optional_idx_vector<uuid> vec;
			ISDExpectSanityCheckDebugFailMacro( vec.index(); );
			ISDExpectSanityCheckDebugFailMacro( vec.values(); );
			Assert::IsTrue( vec == optional_idx_vector<uuid>() );

			vec.set();
			for( size_t i = 0; i < 10; ++i )
				{
				vec.index().emplace_back( (uint)i*2+0 );
				vec.index().emplace_back( (uint)i*2+1 );
				vec.values().emplace_back( random_value<uuid>() );
				}
			Assert::IsTrue( vec.index().size() == 20 );
			Assert::IsTrue( vec.values().size() == 10 );

			optional_idx_vector<uuid> vec2 = std::move(vec);
			ISDExpectSanityCheckDebugFailMacro( vec.index(); );
			ISDExpectSanityCheckDebugFailMacro( vec.values(); );
			Assert::IsTrue( vec2.index().size() == 20 );
			Assert::IsTrue( vec2.values().size() == 10 );
			Assert::IsTrue( vec != vec2 );
				
			vec = vec2;
			Assert::IsTrue( vec.index().size() == 20 );
			Assert::IsTrue( vec.values().size() == 10 );
			Assert::IsTrue( vec.index().size() == vec2.index().size() );
			Assert::IsTrue( vec.values().size() == vec2.values().size() );
			Assert::IsTrue( vec.index() == vec2.index() );
			Assert::IsTrue( vec.values() == vec2.values() );
				
			vec.vector().clear();
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec.index() != vec2.index() );
			Assert::IsTrue( vec.values() != vec2.values() );
				
			vec2.vector().clear();
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec2.index().size() == 0 );
			Assert::IsTrue( vec2.values().size() == 0 );
			Assert::IsTrue( vec.index() == vec2.index() );
			Assert::IsTrue( vec.values() == vec2.values() );
			}

		TEST_METHOD( Test_optional_value )
			{
			optional_value<int> opt( 0x1337 );
			Assert::IsTrue( opt.has_value() );
			Assert::IsTrue( opt == 0x1337 );
			Assert::IsFalse( opt != 0x1337 );

			optional_value<int> opt2 = std::move( opt );
			Assert::IsFalse( opt.has_value() );
			Assert::IsTrue( opt != 0x1337 );
			Assert::IsFalse( opt == 0x1337 );
			Assert::IsTrue( opt != 0 );
			Assert::IsTrue( opt2.has_value() );
			Assert::IsTrue( opt2 == 0x1337 );
			Assert::IsTrue( opt != opt2 );
			Assert::IsFalse( opt == opt2 );

			opt.set( opt2 );
			Assert::IsTrue( opt.has_value() );
			Assert::IsTrue( opt2.has_value() );
			Assert::IsTrue( opt == opt2 );

			opt2.reset();
			Assert::IsTrue( opt.has_value() );
			Assert::IsFalse( opt2.has_value() );
			Assert::IsTrue( opt != opt2 );

			opt2.set();
			Assert::IsTrue( opt != opt2 );
			opt2.value() = opt.value();
			Assert::IsTrue( opt == opt2 );
			opt.set( 0x13337 );
			Assert::IsTrue( opt != opt2 );
			Assert::IsFalse( opt == opt2 );
			Assert::IsFalse( opt == opt2.value() );
			Assert::IsTrue( opt != opt2.value() );
			Assert::IsTrue( opt.value() != opt2.value() );
			Assert::IsTrue( opt.has_value() );
			Assert::IsTrue( opt2.has_value() );

			opt.reset();
			opt2.reset();
			Assert::IsTrue( opt == opt2 );
			}

		TEST_METHOD( Test_optional_vector )
			{
			optional_vector<uuid> vec;
			ISDExpectSanityCheckDebugFailMacro( vec.vector(); );
			Assert::IsTrue( vec == optional_vector<uuid>() );

			vec.set();
			for( size_t i = 0; i < 10; ++i )
				{
				vec.values().emplace_back( random_value<uuid>() );
				}
			Assert::IsTrue( vec.values().size() == 10 );

			optional_vector<uuid> vec2 = std::move(vec);
			ISDExpectSanityCheckDebugFailMacro( vec.values(); );
			Assert::IsTrue( vec2.values().size() == 10 );
			Assert::IsTrue( vec != vec2 );

			vec = vec2;
			Assert::IsTrue( vec.values().size() == 10 );
			Assert::IsTrue( vec.values().size() == vec2.values().size() );
			Assert::IsTrue( vec.values() == vec2.values() );

			vec.vector().clear();
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec.values() != vec2.values() );

			vec2.vector().clear();
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec2.values().size() == 0 );
			Assert::IsTrue( vec.values() == vec2.values() );
			}

		TEST_METHOD( Test_package_ref )
			{
			package_ref ref;
			Assert::IsTrue( !ref );
			hash val = ref;
			Assert::IsTrue( val == hash_zero );
			Assert::IsTrue( val == package_ref::null() );
			Assert::IsTrue( ref == package_ref() );

			package_ref ref2 = random_value<hash>();
			Assert::IsTrue( ref != ref2 );
			Assert::IsTrue( !(ref == ref2) );
			Assert::IsTrue( ref < ref2 ); // ref is zero, ref2 must be more 
			val = ref2;
			Assert::IsTrue( val != hash_zero );
			Assert::IsTrue( hash_zero < val );

			ref = std::move( ref2 );
			Assert::IsTrue( ref != ref2 );
			Assert::IsTrue( !(ref == ref2) );
			Assert::IsTrue( ref2 < ref ); // ref2 is zero, ref must be more 

			ref2 = ref;
			Assert::IsTrue( ref == ref2 );
			Assert::IsTrue( !(ref != ref2) );
			Assert::IsTrue( !(ref2 < ref) ); 
			}

		template<typename K, typename V>
		std::map<V, K> inverse_map(std::map<K, V> &map)
			{
			std::map<V, K> inv;
			std::for_each(map.begin(), map.end(),
				[&inv] (const std::pair<K, V> &p) {
				inv.emplace(p.second, p.first);
				});
			return inv;
			}

		// this tests the hash function, the less than operator and the equals operator
		template<class K> void TestSetAndMapWithKey()
			{ 
			const size_t item_count = 100;

			std::map<K, size_t> mmap;
			std::unordered_map<K, size_t> umap;
			std::set<K> mset;
			std::unordered_set<K> uset;
			for( size_t i = 0; i < item_count; ++i )
				{
				K key = random_value<K>();
				mmap.emplace( key, i );
				umap.emplace( key, i );
				mset.emplace( key );
				uset.emplace( key );
				}
			
			Assert::AreEqual( item_count , mmap.size() );
			Assert::AreEqual( item_count , umap.size() );
			Assert::AreEqual( item_count , mset.size() );
			Assert::AreEqual( item_count , uset.size() );

			auto lookup = inverse_map( mmap );

			for( size_t i = 0; i < item_count; ++i )
				{
				// lookup key from value
				bool found = lookup.find(i) != lookup.end();
				Assert::IsTrue( found );
				K key = lookup[i];
			
				// make sure that the key is in map and umap, and returns same value
				found = mmap.find( key ) != mmap.end();
				Assert::IsTrue( found );
				Assert::IsTrue( mmap[key] == i );
				found = umap.find( key ) != umap.end();
				Assert::IsTrue( found );
				Assert::IsTrue( umap[key] == i );

				// make sure the key is in the two sets
				found = mset.find( key ) != mset.end();
				Assert::IsTrue( found );
				found = uset.find( key ) != uset.end();
				Assert::IsTrue( found );
				}
			}

		TEST_METHOD( Test_functors )
			{
			setup_random_seed();

			for( uint pass_index = 0; pass_index < global_number_of_passes; ++pass_index )
				{
				TestSetAndMapWithKey<uuid>();
				TestSetAndMapWithKey<hash>();
				}
			}

		class uint_string_map_test_class
			{
			thread_safe_map<uint, std::string> *object = nullptr;
			uint index = 0;
			static const uint num_iters = 10;
			bool done = false;

			public:
				static void uint_string_map_test_thread( void * ptr )
					{
					std::vector<uint> list(num_iters);
					uint_string_map_test_class *pthis = (uint_string_map_test_class *)ptr;

					// insert values into list
					for( uint i = 0; i < num_iters; ++i )
						{
						list[i] = rand() << 16 | pthis->index;
						char num[20];
						sprintf_s( num, "%d", list[i] );
						std::string str = num;
						pthis->object->insert( std::pair<uint,std::string>(list[i],str));
						}

					// make sure that the values exist, (do random lookup)
					for( uint i = 0; i < num_iters; ++i )
						{
						uint look_for = rand() % num_iters;
						char num[20];
						sprintf_s( num, "%d", list[look_for] );
						std::string str = num;
						auto val = pthis->object->find( list[look_for] );
						Assert::IsTrue( val.second ); // make sure we found it
						Assert::IsTrue( str == val.first );
						}

					// erase all values
					for( uint i = 0; i < num_iters; ++i )
						{
						pthis->object->erase( list[i] );
						}

					pthis->done = true;
					}

				HANDLE run( uint _index , thread_safe_map<uint, std::string> *_object )
					{
					this->done = false;
					this->index = _index;
					this->object = _object;
					return (HANDLE)_beginthread( &uint_string_map_test_thread, 0, (void *)this );
					}

				bool is_done() const
					{
					return this->done;
					}
			};

		TEST_METHOD( Test_thread_safe_map )
			{
			thread_safe_map<uint, std::string> uint_string_map;
			std::vector<uint_string_map_test_class> test_objects( 4 );
			std::vector<HANDLE> thread_handles( test_objects.size() );
			for( size_t i = 0; i < test_objects.size(); ++i )
				{
				thread_handles[i] = test_objects[i].run( (uint)i, &uint_string_map );
				}
			DWORD val = WaitForMultipleObjects( (uint)thread_handles.size(), thread_handles.data(), TRUE, INFINITE );
			Assert::IsTrue( val == 0 );
			for( size_t i = 0; i < test_objects.size(); ++i )
				{
				Assert::IsTrue( test_objects[i].is_done() );
				}
			}
		};
	}














