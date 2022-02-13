// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

namespace ISD
	{
	class MemoryReadStream;
	class MemoryWriteStream;

	class Packet
		{
		public:
		};

	class PacketSerializer
		{
		public:
			// deserializes an Entity from a memory stream. The returned Entity is 
			// handed over, and must be deleted by the caller.
			std::pair<Packet *,Status> FromMemoryStream( MemoryReadStream &input_stream );

			// serializes an Entity to a memory stream.
			Status ToMemoryStream( const Packet *packet , MemoryWriteStream &output_stream );
		};
	};