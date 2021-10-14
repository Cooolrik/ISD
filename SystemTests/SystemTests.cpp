// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "../ISD/ISD.h"
#include "../ISD/ISD_MemoryReadSteam.h"
#include "../ISD/ISD_MemoryWriteSteam.h"

#include <Rpc.h>

extern void safe_thread_map_test();

#define RUN_TEST( name )\
	printf("Running test: " #name "\n");\
	name();\
	printf(" - Test: " #name " done\n");

int main()
	{
	RUN_TEST( safe_thread_map_test );

	//EntityLoader load;

	//uint8 val[8] = {};
	//float val32 = 10.f;
	//bigendian_from_value<uint32>( val, (uint32)val32 );

	//load.Initialize( "../../ISDDir" );

	//UUID id;

	//::UuidCreate( &id );

	//MemoryWriteStream *ws = new MemoryWriteStream(4);
	//ws->Write( id );

	//std::wstring name = value_to_hex_wstring( id );

	//std::wstring top_byte = value_to_hex_wstring( (uint8)((id.Data1 >> 24) & 0xff) );

	//// read the header
	//uint8 arr[256];
	//for( size_t i = 0; i < 256; ++i )
	//	{
	//	arr[i] = (uint8)i;
	//	}

	//MemoryReadStream *pstream = new MemoryReadStream( arr , sizeof(arr) , false );

	//std::vector<float> vec;
	//*((uint64 *)arr) = 12;
	//bool succ = pstream->Read( &vec );


	//q t;
	//t.t = pstream->Read<time_t>();
	//t.t2 = pstream->Read<time_t>();

	return 0;
	}


