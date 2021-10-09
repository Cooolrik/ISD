
#include "../ISD/ISD.h"
#include "../ISD/ISD_MemorySteam.h"

#include <Rpc.h>

using namespace ISD;

int main()
	{
	EntityLoader load;

	load.Initialize( "../../ISDDir" );

	UUID id;

	::UuidCreate( &id );

	std::wstring name = uuid_to_hex_wstring( id );

	std::wstring top_byte = uint8_to_hex_wstring( (uint8)((id.Data1 >> 24) & 0xff) );

	// read the header
	uint8 arr[256];
	for( size_t i = 0; i < 256; ++i )
		{
		arr[i] = (uint8)i;
		}

	MemoryReadStream *pstream = new MemoryReadStream( arr , sizeof(arr) , false );

	std::vector<float> vec;
	*((uint64 *)arr) = 12;
	bool succ = pstream->Read( &vec );

	return 0;
	}


