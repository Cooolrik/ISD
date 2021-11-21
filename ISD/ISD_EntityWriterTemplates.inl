// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "ISD_Log.h"

namespace ISD
	{
	// template method that writes a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<ValueType VT, class I, size_t IC> bool write_small_block( 
		MemoryWriteStream &dstream, 
		const char *key, 
		const uint8 key_length, 
		const I *data)
		{
		const uint8 value_type = (uint8)VT;
		const size_t data_size_in_bytes = (data != nullptr) ? (sizeof( I ) * IC) : 0; // if data == nullptr, the block is empty
		const size_t block_size = data_size_in_bytes + key_length;
		ISDSanityCheckDebugMacro( key_length <= EntityMaxKeyLength ); // max key length
		ISDSanityCheckCoreDebugMacro( block_size < 256 ); // must fit in a byte
		const uint8 u8_block_size = (uint8)(block_size);
		const uint64 start_pos = dstream.GetPosition();
		const uint64 expected_end_pos = start_pos + 2 + block_size;

		// write data block 
		dstream.Write( value_type );
		dstream.Write( u8_block_size );
		if( data_size_in_bytes > 0 )
			{
			dstream.Write( data, IC );
			}
		dstream.Write( (int8 *)key, key_length );

		const uint64 end_pos = dstream.GetPosition();
		ISDSanityCheckCoreDebugMacro( end_pos == expected_end_pos );
		return (dstream.GetPosition() == expected_end_pos);
		};


	// returns the stream position of the start of the block, to be used when writing the size when ending the block
	std::pair<bool, uint64> begin_large_block( MemoryWriteStream &dstream, ValueType VT, const char *key, const uint8 key_size_in_bytes )
		{
		const uint8 value_type = (uint8)VT;
		const uint64 start_pos = dstream.GetPosition();
		ISDSanityCheckDebugMacro( key_size_in_bytes <= EntityMaxKeyLength ); // max key length
		const uint64 expected_end_pos = start_pos + key_size_in_bytes + 10; // sizeof(value_type)=1 + sizeof(block_size)=8 + sizeof(key_size_in_bytes)=1 + key_size_in_bytes;

																			// write block header 
																			// write empty stand in value for now (MAXINT64 on purpose), which is definitely 
																			// wrong, as to trigger any test if the value is not overwritten with the correct value
		dstream.Write( value_type );
		dstream.Write( (uint64)MAXINT64 );
		dstream.Write( key_size_in_bytes );
		dstream.Write( (int8 *)key, key_size_in_bytes );

		const uint64 end_pos = dstream.GetPosition();
		ISDSanityCheckCoreDebugMacro( end_pos == expected_end_pos );
		return std::pair<bool, uint64>( (end_pos == expected_end_pos), start_pos );
		}

	// ends the block, write the size of the block
	bool end_large_block( MemoryWriteStream &dstream, uint64 start_pos )
		{
		const uint64 end_pos = dstream.GetPosition();
		const uint64 block_size = end_pos - start_pos - 9; // total block size - ( sizeof( valuetype )=1 + sizeof( block_size_variable )=8 )
		dstream.SetPosition( start_pos + 1 ); // skip over the valuetype
		dstream.Write( block_size );
		dstream.SetPosition( end_pos ); // move back the where we were
		return (end_pos > start_pos); // only thing we really can check
		}

	// write indexed array to stream
	template<ValueType VT, class T, class I, size_t I_per_T > bool write_large_block_array( 
		MemoryWriteStream &dstream, 
		const char *key, 
		const uint8 key_size_in_bytes, 
		const bool is_empty,
		const I *data_items, 
		const size_t data_items_count,
		const size_t *data_index,
		const size_t data_index_count )
		{
		// assuming size_t is same size as uint64
		// if this fails, the code needs to be updated to support other sizes of size_t
		static_assert(sizeof( uint64 ) == sizeof( size_t ), "Unsupported size_t, current code requires it to be 8 bytes in size, equal to uint64");

		// this code only supports values that are exactly the multiple I_per_T of the items it contins
		// eg basic types are ok, since I_per_T is 1. vec3 etc are also ok, as vec3 is I_per_T = 3
		static_assert(sizeof( I ) * I_per_T == sizeof( T ), "Size of T must be exactly size of I times I_per_T");

		// begin a large block, and store the start position returned from it
		auto begin_status = begin_large_block( dstream, VT, key, key_size_in_bytes );
		if( !begin_status.first )
			{
			ISDErrorLog << "begin_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		const uint64 start_pos = dstream.GetPosition();

		// if empty data (not same as a zero-length array, which is not empty, only zero length)
		uint64 expected_end_pos = 0;
		if( is_empty )
			{
			// empty data, mark flags field empty
			dstream.Write( (uint16)0 );

			// calc the end position 
			expected_end_pos = start_pos + sizeof( uint16 );
			}
		else
			{
			// indexed array flags: size of each item (if need to decode array outside regular decoding) and bit set if index is used 
			const uint16 per_item_size = (uint16)sizeof( I ); // size of each item I in T (max 255 bytes per item)
			ISDSanityCheckCoreDebugMacro( per_item_size <= 0xff );
			const uint16 has_index = (data_index) ? 0x100 : 0x0;
			const uint16 index_array_flags = has_index | per_item_size;
			dstream.Write( index_array_flags );

			// write the item count and items
			dstream.Write( data_items_count );
			dstream.Write( data_items, data_items_count );

			// if we have an index, write it 
			if( data_index != nullptr )
				{
				// cast to uint64* and write to stream 
				// (the code assumes size_t and uint64 are the same size, and can be cast, look at static_assert above)
				dstream.Write( data_index, data_index_count );
				}

			// make sure all data was written
			expected_end_pos = start_pos
				+ sizeof( uint16 ) // the flags
				+ sizeof( uint64 ) // the item count
				+ (data_items_count * sizeof( I )) // the items
				+ ((data_index_count * sizeof( uint64 ))); // the list of indices, if any
			}

		// end the block by going back to the start and writing the start position offset
		if( !end_large_block( dstream, begin_status.second ) )
			{
			ISDErrorLog << "end_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// make sure we are at the expected end pos
		const uint64 end_pos = dstream.GetPosition();
		if( end_pos != expected_end_pos )
			{
			ISDErrorLog << "End position of data " << end_pos << " do not equal the expected end position which is " << expected_end_pos << ISDErrorLogEnd;
			return false;
			}

		return true;
		}

	// specific write_large_block_array implementation for VT_Bool
	bool write_bool_array(
		MemoryWriteStream &dstream,
		const char *key,
		const uint8 key_size_in_bytes,
		const bool is_empty,
		const std::vector<bool> *bool_data,
		const std::vector<size_t> *index_data )
		{
		// copy the bool vector to an int8 vector
		std::vector<int8> i8vec;
		if( bool_data )
			{
			for( size_t i = 0; i < bool_data->size(); ++i )
				{
				i8vec[i] = (int8)(*bool_data)[i];
				}
			}

		// call the template implementation with int8 instead of bool
		return write_large_block_array<ValueType::VT_Bool, int8, int8, 1>( 
			dstream, 
			key, 
			key_size_in_bytes, 
			is_empty,
			(bool_data) ? i8vec.data() : nullptr,
			(bool_data) ? i8vec.size() : 0,
			(index_data) ? index_data->data() : nullptr,
			(index_data) ? index_data->size() : 0
			);
		}

	// VT_Bool: bool
	bool EntityWriter::Write( const char *key, const uint8 key_length, const bool &src_variable )
		{
		int8 i8val = (int8)src_variable;
		return write_small_block<ValueType::VT_Bool, int8, 1>( dstream, key, key_length, &i8val );
		}

	// VT_Bool: optional_value<bool>
	bool EntityWriter::Write( const char *key, const uint8 key_length, const optional_value<bool> &src_variable )
		{
		int8 i8val = (int8)src_variable.value().second;
		return write_small_block<ValueType::VT_Bool, int8, 1>( dstream, key, key_length, (src_variable.has_value()) ? &i8val : nullptr );
		}

	// VT_Bool: std::vector<bool>
	bool EntityWriter::Write( const char *key, const uint8 key_length, const std::vector<bool> &src_variable )
		{
		return write_bool_array(
			this->dstream, 
			key, 
			key_length, 
			true, // not an optional value
			&src_variable, 
			nullptr
			);
		}

	// VT_Bool: optional_value<std::vector<bool>>
	bool EntityWriter::Write( const char *key, const uint8 key_length, const optional_value<std::vector<bool>> &opt_src_variable )
		{
		const bool has_value = opt_src_variable.has_value();
		const std::vector<bool> &src_variable = opt_src_variable.value().second;
		
		return write_bool_array(
			this->dstream, 
			key, 
			key_length, 
			true, // not an optional value
			(has_value) ? &src_variable : nullptr, // nullptr if size is zero
			nullptr
			);
		}

	// VT_Bool: indexed_array<bool>
	bool EntityWriter::Write( const char *key, const uint8 key_length, const indexed_array<bool> &src_variable )
		{
		return write_bool_array(
			this->dstream, 
			key, 
			key_length, 
			true, // not an optional value
			&src_variable.values(), 
			&src_variable.index()
			);
		}

	// VT_Bool: optional_value<indexed_array<bool>>
	bool EntityWriter::Write( const char *key, const uint8 key_length, const optional_value<indexed_array<bool>> &opt_src_variable )
		{
		const bool has_value = opt_src_variable.has_value();
		const indexed_array<bool> &src_variable = opt_src_variable.value().second;

		return write_bool_array(
			this->dstream, 
			key, 
			key_length, 
			true, // not an optional value
			(has_value) ? &src_variable.values() : nullptr, 
			(has_value) ? &src_variable.index() : nullptr
			);
		}

	};
