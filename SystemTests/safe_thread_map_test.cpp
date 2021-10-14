
#include "SystemTests.h"


class uint_string_map_test_class
	{
	thread_safe_map<uint, std::string> *object = nullptr;
	uint index = 0;
	static const uint num_iters = 10000;
	bool done = false;

	public:
		static void uint_string_map_test_thread( void * ptr )
			{
			uint list[num_iters];
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
				TEST_ASSERT( val.second ); // make sure we found it
				TEST_ASSERT( str == val.first );
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

void safe_thread_map_test()
	{
	thread_safe_map<uint, std::string> uint_string_map;
	std::vector<uint_string_map_test_class> test_objects( MAXIMUM_WAIT_OBJECTS );
	std::vector<HANDLE> thread_handles( test_objects.size() );
	for( size_t i = 0; i < test_objects.size(); ++i )
		{
		thread_handles[i] = test_objects[i].run( (uint)i, &uint_string_map );
		}
	DWORD val = WaitForMultipleObjects( (uint)thread_handles.size(), thread_handles.data(), TRUE, INFINITE );
	TEST_ASSERT( val == 0 );
	for( size_t i = 0; i < test_objects.size(); ++i )
		{
		TEST_ASSERT( test_objects[i].is_done() );
		}
	}
