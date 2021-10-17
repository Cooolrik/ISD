// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"
#include "ISD_Entity.h"

namespace ISD
	{
	class MemoryReadStream;
	class MemoryWriteStream;

	class EntitySerializer
		{
		public:
			// deserializes an Entity from a memory stream. The returned Entity is 
			// handed over, and must be deleted by the caller.
			std::pair<Entity *,Status> FromMemoryStream( MemoryReadStream &input_stream );

			// serializes an Entity to a memory stream.
			Status ToMemoryStream( const Entity *entity , MemoryWriteStream &output_stream );
		};
	};