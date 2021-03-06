// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "ISD_PacketSerializer.h"
#include "ISD_MemoryReadStream.h"
#include "ISD_MemoryWriteStream.h"

using namespace ISD;

#ifdef ISD_BIG_ENDIAN_SYSTEM
static const bool this_system_is_big_endian = true;
#else//ISD_BIG_ENDIAN_SYSTEM
static const bool this_system_is_big_endian = false;
#endif//ISD_BIG_ENDIAN_SYSTEM


// Blob is the serialized data (and header) of each entity in ISD
enum BlobFlags
	{
	BigEndian = 0x1, // if set, the data is stored in big endian format, if not set, stored in little endian
	};

// header for each file blob
struct BlobHeader
	{
	u8 Magic[3]; // magic values of the ISD blob ( 0x49, 0x53, 0x44 )
	u8 Version; // version of the blob header. Initial version is 0.
	u8 Flags; // flags for the blob header
	u8 _padding[3]; // padding reserved for possible future use
	u64 DataSize; // size of the data, after the header
	};

//bool BlobHeader::WriteToStream( MemoryWriteStream &ostream )
//	{
//	u64 p = ostream.GetPosition();
//	ostream.Write( this->Magic, 3 );
//	ostream.Write( this->Version );
//	ostream.Write( this->Flags );
//	ostream.Write( this->_padding, 3 );
//	ostream.Write( this->DataSize );
//	return ostream.GetPosition() == (p + 16); // true if we have written 16 bytes
//	}

std::pair<Packet *, Status> ISD::PacketSerializer::FromMemoryStream( MemoryReadStream &input_stream )
	{
	Packet *entity = {};
	u64 expected_pos = {};

	// read in the first 8 bytes of the header
	BlobHeader header;
	input_stream.Read( header.Magic, 3 );
	header.Version = input_stream.Read<u8>();
	header.Flags = input_stream.Read<u8>();
	input_stream.Read( header._padding, 3 );

	// make sure it is an ISD file, and check flags

	// read in the file header, make sure it is an ISD file
	if( memcmp(header.Magic , "ISD" , 3) != 0 )
		{
		return std::pair<Packet *, Status>( nullptr, Status::EInvalid );
		}

	// for now, the version does not matter, as we are only on first iteration, and future versions need to be backwards compatible 

	// check known flags

	// big/little endian, flip byte order if it does not match
	const bool this_file_is_big_endian = (header.Flags & BlobFlags::BigEndian);
	if( this_file_is_big_endian != this_system_is_big_endian )
		{
		input_stream.SetFlipByteOrder( true );
		}

	// make sure the stream has all the data
	header.DataSize = input_stream.Read<u64>();
	if( (input_stream.GetPosition() + header.DataSize) > input_stream.GetSize() )
		{
		return std::pair<Packet *, Status>( nullptr, Status::EInvalid );
		}
	
	// read in the Id of the Packet and the Packet type, which matches the class of the entity
	UUID EntityId = input_stream.Read<UUID>();
	UUID EntityTypeId = input_stream.Read<UUID>();



	return std::pair<Packet *, Status>(entity,Status::Ok);
	}

Status ISD::PacketSerializer::ToMemoryStream( const Packet *entity, MemoryWriteStream &output_stream )
	{
	return Status::Ok;
	}
