
#include "../ISD/ISD.h"

#include <Rpc.h>

using namespace ISD;

int main()
	{
	EntityLoader load;

	load.Initialize( "../../ISDDir" );

	UUID id;

	::UuidCreate( &id );

	std::wstring name = uuid_to_hex_string( id );

	std::wstring top_byte = uint8_to_hex_string( (uint8)((id.Data1 >> 24) & 0xff) );


	return 0;
	}


