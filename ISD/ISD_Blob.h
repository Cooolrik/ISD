// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#pragma once

#include "ISD_Types.h"
#include "ISD_MemoryReadStream.h"
#include "ISD_MemoryWriteStream.h"

namespace ISD
	{
	// Blob is the serialized data (and header) of each entity in ISD

	// header for each file blob
	struct BlobHeader
		{
		uint8 Magic[3]; // magic values of the ISD blob ( 0x49, 0x53, 0x44 )
		uint8 VersionMajor; // major version of the blob 
		uint8 VersionMinor; // minor version of the blob 
		uint8 _padding[3]; // padding reserved for possible future use
		uint64 DataSize; // size of the data, after the header

		// read a header from file. returns true on success
		bool ReadFromStream( MemoryReadStream &istream );

		// write a header to file. returns true on success
		bool WriteToStream( MemoryWriteStream &ostream );
		};

	bool BlobHeader::ReadFromStream( MemoryReadStream &istream )
		{
		uint64 p = istream.GetPosition();
		istream.Read( this->Magic, 3 );
		this->VersionMajor = istream.Read<uint8>();
		this->VersionMinor = istream.Read<uint8>();
		istream.Read( this->_padding, 3 );
		this->DataSize = istream.Read<uint64>();
		return istream.GetPosition() == (p + 16); // true if we have read 16 bytes
		}

	bool BlobHeader::WriteToStream( MemoryWriteStream &ostream )
		{
		uint64 p = ostream.GetPosition();
		ostream.Write( this->Magic, 3 );
		ostream.Write( this->VersionMajor );
		ostream.Write( this->VersionMinor );
		ostream.Write( this->_padding, 3 );
		ostream.Write( this->DataSize );
		return ostream.GetPosition() == (p + 16); // true if we have written 16 bytes
		}

	};