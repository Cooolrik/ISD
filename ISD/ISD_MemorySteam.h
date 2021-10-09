#pragma once

#include "ISD_Types.h"

#include <vector>

namespace ISD
	{
	// memory read stream is a wrapper around a read-only memory area.
	// The stream is used to read structured values, and automatically 
	// does byte order swapping and reads in more complex types than
	// just plain old data (POD) types, such as std::string, UUIDs and
	// std::vector of the above types.
	class MemoryReadStream
		{
		private:
			const uint8 *Data = nullptr;
			uint64 DataSize = 0;
			uint64 DataPosition = 0;
			bool FlipByteOrder = false; // true if we should flip BE to LE or LE to BE

			uint64 ReadRawData( void *dest, uint64 count );

			template <class T> uint64 ReadValues( T *dest, uint64 count );
			template <> uint64 ReadValues<uint8>( uint8 *dest, uint64 count );

		public:
			MemoryReadStream( const uint8 *_Data, uint64 _DataSize, bool _FlipByteOrder = false ) : Data( _Data ), DataSize( _DataSize ), FlipByteOrder(_FlipByteOrder) {};

			// get the Size of the stream in bytes
			uint64 GetSize() const;

			// Position is the current data position. the beginning of the stream is position 0. the position will not move past the end of the stream.
			uint64 GetPosition() const;
			bool SetPosition( uint64 new_pos );

			// if at EOF return true, which means that Position == Size
			bool IsEOF() const;

			// FlipByteOrder is set if the stream flips byte order of multibyte values 
			bool GetFlipByteOrder() const;
			void SetFlipByteOrder( bool value );

			// read one item from the memory stream. makes sure to convert endianness
			template <class T> T Read();
			template <> int8 Read<int8>();
			template <> int16 Read<int16>();
			template <> int32 Read<int32>();
			template <> int64 Read<int64>();
			template <> uint8 Read<uint8>();
			template <> uint16 Read<uint16>();
			template <> uint32 Read<uint32>();
			template <> uint64 Read<uint64>();
			template <> UUID Read<UUID>();
			template <> float Read<float>();
			template <> double Read<double>();
			template <> std::string Read<std::string>();

			// read multiple items from the memory stream. makes sure to convert endianness
			uint64 Read( int8 *dest , uint64 count );
			uint64 Read( int16 *dest , uint64 count );
			uint64 Read( int32 *dest , uint64 count );
			uint64 Read( int64 *dest , uint64 count );
			uint64 Read( uint8 *dest , uint64 count );
			uint64 Read( uint16 *dest , uint64 count );
			uint64 Read( uint32 *dest , uint64 count );
			uint64 Read( uint64 *dest , uint64 count );
			uint64 Read( UUID *dest , uint64 count );
			uint64 Read( float *dest , uint64 count );
			uint64 Read( double *dest , uint64 count );
			uint64 Read( std::string *dest , uint64 count );

			// read a std::vector of items from stream. reads in the count, and returns false if not the full vector could be read.
			template<class T> bool Read( std::vector<T> *dest );

		};

	inline uint64 MemoryReadStream::ReadRawData( void *dest, uint64 count )
		{
		// cap the end position
		uint64 end_pos = this->DataPosition + count;
		if( end_pos > this->DataSize )
			end_pos = this->DataSize;

		// copy the data and move the position
		uint64 memcount = end_pos - this->DataPosition;
		memcpy( dest, &this->Data[this->DataPosition], memcount );
		this->DataPosition = end_pos;
		return memcount;
		}

	template <class T> inline uint64 MemoryReadStream::ReadValues( T *dest, uint64 count )
		{
		uint64 readc = this->ReadRawData( dest, count * sizeof(T) ) / sizeof(T);
		if( !this->FlipByteOrder )
			return readc;

		// flip the byte order of the words in the dest
		swap_byte_order<T>( dest, count );
		return readc;
		}

	template <> inline uint64 MemoryReadStream::ReadValues<uint8>( uint8 *dest, uint64 count ) 
		{ 
		return ReadRawData( dest, count ); 
		}

	inline uint64 MemoryReadStream::GetSize() const
		{
		return this->DataSize;
		}

	inline uint64 MemoryReadStream::GetPosition() const 
		{ 
		return this->DataPosition; 
		}

	inline bool MemoryReadStream::SetPosition( uint64 new_pos ) 
		{ 
		if( new_pos > DataSize )
			{
			return false;
			}
		this->DataPosition = new_pos; 
		return true;
		}

	// if at EOF return true
	inline bool MemoryReadStream::IsEOF() const 
		{ 
		return this->DataPosition >= this->DataSize; 
		}

	// true if stream flips byte order
	inline bool MemoryReadStream::GetFlipByteOrder() const 
		{ 
		return this->FlipByteOrder; 
		}

	void MemoryReadStream::SetFlipByteOrder( bool value )
		{
		this->FlipByteOrder = value;
		}

	// read one item of data
	template <> inline int8 MemoryReadStream::Read<int8>() { int8 dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline int16 MemoryReadStream::Read<int16>() { int16 dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline int32 MemoryReadStream::Read<int32>() { int32 dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline int64 MemoryReadStream::Read<int64>() { int64 dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline uint8 MemoryReadStream::Read<uint8>() { uint8 dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline uint16 MemoryReadStream::Read<uint16>() { uint16 dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline uint32 MemoryReadStream::Read<uint32>() { uint32 dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline uint64 MemoryReadStream::Read<uint64>() { uint64 dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline UUID MemoryReadStream::Read<UUID>() { UUID dest = {}; this->Read( &dest, 1 ); return dest; }
	template <> inline float MemoryReadStream::Read<float>() { float dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline double MemoryReadStream::Read<double>() { double dest = 0; this->Read( &dest, 1 ); return dest; }
	template <> inline std::string MemoryReadStream::Read<std::string>() { std::string dest; this->Read( &dest, 1 ); return dest; }

	// 8 bit data
	inline uint64 MemoryReadStream::Read( int8 *dest, uint64 count ) { return this->ReadValues<uint8>( (uint8*)dest, count ); }
	inline uint64 MemoryReadStream::Read( uint8 *dest, uint64 count ) { return this->ReadValues<uint8>( dest, count ); }

	// 16 bit data
	inline uint64 MemoryReadStream::Read( int16 *dest, uint64 count ) { return this->ReadValues<uint16>( (uint16*)dest, count ); }
	inline uint64 MemoryReadStream::Read( uint16 *dest, uint64 count ) { return this->ReadValues<uint16>( dest, count ); }

	// 32 bit data
	inline uint64 MemoryReadStream::Read( int32 *dest, uint64 count ) { return this->ReadValues<uint32>( (uint32*)dest, count ); }
	inline uint64 MemoryReadStream::Read( uint32 *dest, uint64 count ) { return this->ReadValues<uint32>( dest, count ); }
	inline uint64 MemoryReadStream::Read( float *dest, uint64 count ) { return this->ReadValues<uint32>( (uint32*)dest, count ); }

	// 64 bit data
	inline uint64 MemoryReadStream::Read( int64 *dest, uint64 count ) { return this->ReadValues<uint64>( (uint64*)dest, count ); }
	inline uint64 MemoryReadStream::Read( uint64 *dest, uint64 count ) { return this->ReadValues<uint64>( dest, count ); }
	inline uint64 MemoryReadStream::Read( double *dest, uint64 count ) { return this->ReadValues<uint64>( (uint64*)dest, count ); }

	// UUIDs
	inline uint64 MemoryReadStream::Read( UUID *dest, uint64 count ) 
		{ 
		for( uint64 i = 0; i < count; ++i )
			{
			// we always store uuids big endian (the order which the hex values are printed when printing a UUID), regardless of machine, so read in the 16 bytes and assign
			uint8 rawbytes[16];
			if( this->Read( rawbytes, 16 ) != 16 )
				return i; // could not read further, return number of succesful reads

			// assign to the values
			dest[i].Data1 = word_from_big_endian<uint32>( &rawbytes[0] );
			dest[i].Data2 = word_from_big_endian<uint16>( &rawbytes[4] );
			dest[i].Data3 = word_from_big_endian<uint16>( &rawbytes[6] );
			memcpy( dest[i].Data4, &rawbytes[8], 8 );
			}

		return count;
		}

	// std::strings
	inline uint64 MemoryReadStream::Read( std::string *dest, uint64 count ) 
		{ 
		for( uint i = 0; i < count; ++i )
			{
			// read string length
			uint strl = 0;
			if( this->Read( &strl, 1 ) != 1 )
				return i; // failed reading, at end of stream, so return number of successful reads

			// make sure there are enough bytes left in the stream to assign from the data
			uint64 data_left = this->DataSize - this->DataPosition;
			if( data_left < strl )
				return i; // failed reading, at end of stream, so return number of successful reads

			// assign to string directly from the data.
			// NOTE ON CODE: this is OK since we have already checked the bounds, and this is a string of bytes, so no endianness issues arise
			dest[i].assign( (const char*)this->Data, (size_t)strl );
			}

		return count; // all OK
		}

	// std::vector
	template<class T> bool MemoryReadStream::Read( std::vector<T> *dest )
		{
		uint64 count = 0;
		if( this->Read( &count, 1 ) != 1 )
			return false; // could not read array

		// resize array 
		dest->resize( count );

		// read in values, return true if all values could be read
		return (this->Read( dest->data(), count ) == count);
		}
	};