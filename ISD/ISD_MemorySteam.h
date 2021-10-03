#pragma once

#include "ISD_Types.h"

#include <map>
#include <mutex>

namespace ISD
	{
	class MemoryStream
		{
		private:
			const uint8 *Data = nullptr;
			uint64 DataSize = 0;
			uint64 StreamPointer = 0;

		public:
			MemoryStream( const uint8 *_Data, uint64 _DataSize ) : Data( _Data ), DataSize( _DataSize ) {};

			void ReadRawBytes( void *dest, uint32 count );

			void ReadInt8s();
			void ReadInt16s();
			void ReadInt32s();
			void ReadInt64s();

			void ReadUInt8s();
			void ReadUInt16s();
			void ReadUInt32s();
			void ReadUInt64s();

			UUID ReadUUIDs();

			void ReadFloats();
			void ReadDoubles();

		};





	};