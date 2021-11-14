// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>

namespace ISD
	{
	// Memory write stream is a write-only memory area.
	// The stream is used to write structured values. 
	// It can write out more complex types than just plain old data (POD) 
	// types, and also supports std::string, UUIDs and std::vectors of 
	// the above types.
	// Caveat: The stream is NOT thread safe, and should be accessed by 
	// only one thread at a time.
	class MemoryWriteStream
		{
		private:
			static const uint64 InitialAllocationSize = 1024*1024*64; // 64MB initial size

			uint8 *Data = nullptr; // the allocated data
			uint64 DataSize = 0; // the size of the memory stream (not the reserved allocation)
			uint64 Position = 0; // the write position in the memory stream
			
			uint64 DataReservedSize = 0; // the reserved size of the allocation
			uint32 PageSize = 0; // size of each page of allocation
			
			bool FlipByteOrder = false; // true if we should flip BE to LE or LE to BE

			// reserve data for at least reserveSize.
			void ReserveForSize( uint64 reserveSize );
			void FreeAllocation();

			// resize (grow) the data stream. if the new size is larger than the reserved size, the allocation will be resized to fit the new size
			void Resize( uint64 newSize );

			// write raw bytes to the memory stream. this will increase reserved allocation of data as needed
			void WriteRawData( const void *src, uint64 count );

			// write 1,2,4 or 8 byte values and make sure they are in the correct byte order
			template <class T> void WriteValues( const T *src, uint64 count );
			template <> void WriteValues<uint8>( const uint8 *src, uint64 count );

		public:
			MemoryWriteStream( uint64 _InitialAllocationSize = InitialAllocationSize ) { this->ReserveForSize( _InitialAllocationSize ); };
			~MemoryWriteStream() { this->FreeAllocation(); };

			// get a read-only pointer to the data
			const void *GetData() const { return this->Data; }

			// get the Size of the stream in bytes
			uint64 GetSize() const;

			// Position is the current data position. the beginning of the stream is position 0. the stream grows whenever the position moves past the current end of the stream.
			uint64 GetPosition() const;
			void SetPosition( uint64 new_pos );

			// FlipByteOrder is set if the stream flips byte order of multibyte values 
			bool GetFlipByteOrder() const;
			void SetFlipByteOrder( bool value );

			// write one item to the memory stream. makes sure to convert endianness
			void Write( const int8 &src );
			void Write( const int16 &src );
			void Write( const int32 &src );
			void Write( const int64 &src );
			void Write( const uint8 &src );
			void Write( const uint16 &src );
			void Write( const uint32 &src );
			void Write( const uint64 &src );
			void Write( const UUID &src );
			void Write( const float &src );
			void Write( const double &src );
			//void Write( const std::string &src );
			
			// write an array of items to the memory stream. makes sure to convert endianness
			void Write( const int8 *src , uint64 count );
			void Write( const int16 *src , uint64 count );
			void Write( const int32 *src , uint64 count );
			void Write( const int64 *src , uint64 count );
			void Write( const uint8 *src , uint64 count );
			void Write( const uint16 *src , uint64 count );
			void Write( const uint32 *src , uint64 count );
			void Write( const uint64 *src , uint64 count );
			void Write( const UUID *src , uint64 count );
			void Write( const float *src , uint64 count );
			void Write( const double *src , uint64 count );
			//void Write( const std::string *src , uint64 count );
			
			//// read a std::vector of items from stream. reads in the count, and returns false if not the full vector could be read.
			//template<class T> void Write( const std::vector<T> *src );
		};

	inline void MemoryWriteStream::ReserveForSize( uint64 reserveSize )
		{
		// we need to resize the reserved data area, try doubling size
		// and if that is not enough, set to the reserveSize
		this->DataReservedSize *= 2;
		if( this->DataReservedSize < reserveSize )
			{
			this->DataReservedSize = reserveSize;
			}

		// allocate a new area
		uint8 *pNewData = (uint8*)::VirtualAlloc( nullptr, this->DataReservedSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
		if( pNewData == nullptr )
			{
			throw std::bad_alloc();
			}

		// if there is an old area, copy the data and free the old area
		// the area can never shrink, so we don't need to worry about capping
		if( this->Data )
			{
			memcpy( pNewData, this->Data, this->DataSize );
			::VirtualFree( this->Data, 0, MEM_RELEASE );
			}

		// set the new pointer
		this->Data = pNewData;
		}

	inline void MemoryWriteStream::FreeAllocation()
		{
		if( this->Data ) 
			{ 
			::VirtualFree( this->Data, 0, MEM_RELEASE ); 
			}
		}


	inline void MemoryWriteStream::Resize( uint64 newSize )
		{
		if( newSize > this->DataReservedSize )
			{
			this->ReserveForSize( newSize );
			}

		this->DataSize = newSize;
		}

	inline void MemoryWriteStream::WriteRawData( const void *src, uint64 count )
		{
		// cap the end position
		uint64 end_pos = this->Position + count;
		if( end_pos > this->DataSize )
			{
			this->Resize( end_pos );
			}

		// copy the data and move the position
		memcpy( &this->Data[this->Position] , src , count );
		this->Position = end_pos;
		}

	template <class T> inline void MemoryWriteStream::WriteValues( const T *src, uint64 count )
		{
		if( this->FlipByteOrder )
			{
			// flip the byte order of the words in the dest 
			uint64 pos = this->Position;
			this->WriteRawData( src, count * sizeof(T) );
			swap_byte_order<T>( (T*)(&this->Data[pos]), count );
			}
		else
			{
			// no flipping, just write as is
			this->WriteRawData( src, count * sizeof(T) );
			}
		}

	template <> inline void MemoryWriteStream::WriteValues<uint8>( const uint8 *src, uint64 count ) 
		{ 
		this->WriteRawData( src, count ); 
		}

	inline uint64 MemoryWriteStream::GetSize() const
		{
		return this->DataSize;
		}

	inline uint64 MemoryWriteStream::GetPosition() const 
		{ 
		return this->Position; 
		}

	inline void MemoryWriteStream::SetPosition( uint64 new_pos ) 
		{ 
		if( new_pos > DataSize )
			{
			this->Resize( new_pos );
			}
		this->Position = new_pos; 
		}

	inline bool MemoryWriteStream::GetFlipByteOrder() const 
		{ 
		return this->FlipByteOrder; 
		}

	inline void MemoryWriteStream::SetFlipByteOrder( bool value )
		{
		this->FlipByteOrder = value;
		}

	//// write one item of data
	inline void MemoryWriteStream::Write( const int8 &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const int16 &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const int32 &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const int64 &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const uint8 &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const uint16 &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const uint32 &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const uint64 &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const UUID &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const float &src ) { this->Write( &src, 1 ); }
	inline void MemoryWriteStream::Write( const double &src ) { this->Write( &src, 1 ); }
	//inline void MemoryWriteStream::Write( const std::string &src ) { this->Write( &src, 1 ); }

	// 8 bit data
	inline void MemoryWriteStream::Write( const int8 *src, uint64 count ) { return this->WriteValues<uint8>( (const uint8*)src, count ); }
	inline void MemoryWriteStream::Write( const uint8 *src, uint64 count ) { return this->WriteValues<uint8>( src, count ); }
								   
	// 16 bit data				   
	inline void MemoryWriteStream::Write( const int16 *src, uint64 count ) { return this->WriteValues<uint16>( (const uint16*)src, count ); }
	inline void MemoryWriteStream::Write( const uint16 *src, uint64 count ) { return this->WriteValues<uint16>( src, count ); }
								   
	// 32 bit data				   
	inline void MemoryWriteStream::Write( const int32 *src, uint64 count ) { return this->WriteValues<uint32>( (const uint32*)src, count ); }
	inline void MemoryWriteStream::Write( const uint32 *src, uint64 count ) { return this->WriteValues<uint32>( src, count ); }
	inline void MemoryWriteStream::Write( const float *src, uint64 count ) { return this->WriteValues<uint32>( (const uint32*)src, count ); }
								   
	// 64 bit data				   
	inline void MemoryWriteStream::Write( const int64 *src, uint64 count ) { return this->WriteValues<uint64>( (const uint64*)src, count ); }
	inline void MemoryWriteStream::Write( const uint64 *src, uint64 count ) { return this->WriteValues<uint64>( src, count ); }
	inline void MemoryWriteStream::Write( const double *src, uint64 count ) { return this->WriteValues<uint64>( (const uint64*)src, count ); }

	// UUIDs
	inline void MemoryWriteStream::Write( const UUID *src, uint64 count ) 
		{ 
		for( uint64 i = 0; i < count; ++i )
			{
			uint8 rawbytes[16];

			// we always store uuids big endian (the order which the hex values are printed when printing a UUID), regardless of machine, so write the 16 bytes
			// assign the values to the raw big endian byte array 
			bigendian_from_value<uint32>( &rawbytes[0] , src[i].Data1 );
			bigendian_from_value<uint16>( &rawbytes[4] , src[i].Data2 );
			bigendian_from_value<uint16>( &rawbytes[6] , src[i].Data3 );
			memcpy( &rawbytes[8] , src[i].Data4, 8 );

			// write raw bytes
			this->Write( rawbytes, 16 );
			}
		}

	//// std::strings
	//inline void MemoryWriteStream::Write( const std::string *src, uint64 count ) 
	//	{ 
	//	for( uint i = 0; i < count; ++i )
	//		{
	//		// write string length
	//		uint64 strl = src[i].length();
	//		this->Write( &strl, 1 );

	//		// write string data
	//		this->Write( src[i].data(), strl );
	//		}
	//	}

	//// std::vector
	//template<class T> void MemoryWriteStream::Write( const std::vector<T> *dest )
	//	{
	//	uint64 count = 0;
	//	this->Write( &count, 1 );

	//	// read in values, return true if all values could be read
	//	this->Write( dest->data(), count );
	//	}
	
	};