// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "../ISD/ISD.h"
#include "../ISD/ISD_MemoryReadStream.h"
#include "../ISD/ISD_MemoryWriteStream.h"
#include "../ISD/ISD_TestNode.h"
#include "../ISD/ISD_Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Rpc.h>
extern void safe_thread_map_test();

#define RUN_TEST( name )\
	printf("Running test: " #name "\n");\
	name();\
	printf(" - Test: " #name " done\n");

namespace ISD
	{
	typedef ver1::TestNode TestNode;



	}




int main()
	{
	glm::vec2 v;


	//ISD::Log::Error() << "In read_small_block, the type in the input stream:" << 1 << " does not match expected type: " << 3 << std::endl;

	//BoolWriter(  )

	auto val = glm::value_ptr( v );

	std::vector<double> dat1 = {1,2,3,4,5,6,7,8,9,10};
	std::vector<double> dat2 = {1,2,3,4,5,6,7,8,9,10};

	ISD::indexed_array<double> arr1( dat1 );
	ISD::indexed_array<double> arr2( dat2 );

	const auto val1 = arr1.Index();
	const auto val2 = arr2.Index();

	const void *p1 = &(val1.second);
	const void *p2 = &(val2.second);

	ISD::TestNode tn;

	tn.GetName();

	const auto &t = tn.GetTransformation();

	const auto &tr = t.GetTranslation();

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


