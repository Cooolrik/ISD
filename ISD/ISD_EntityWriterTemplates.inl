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
		const u8 key_length, 
		const I *data)
		{
		const u8 value_type = (u8)VT;
		const size_t data_size_in_bytes = (data != nullptr) ? (sizeof( I ) * IC) : 0; // if data == nullptr, the block is empty
		const size_t block_size = data_size_in_bytes + key_length;
		ISDSanityCheckDebugMacro( key_length <= EntityMaxKeyLength ); // max key length
		ISDSanityCheckCoreDebugMacro( block_size < 256 ); // must fit in a byte
		const u8 u8_block_size = (u8)(block_size);
		const u64 start_pos = dstream.GetPosition();
		const u64 expected_end_pos = start_pos + 2 + block_size;

		// write data block 
		dstream.Write( value_type );
		dstream.Write( u8_block_size );
		if( data_size_in_bytes > 0 )
			{
			dstream.Write( data, IC );
			}
		dstream.Write( (i8 *)key, key_length );

		const u64 end_pos = dstream.GetPosition();
		ISDSanityCheckCoreDebugMacro( end_pos == expected_end_pos );
		return (dstream.GetPosition() == expected_end_pos);
		};


	// returns the stream position of the start of the block, to be used when writing the size when ending the block
	bool begin_write_large_block( MemoryWriteStream &dstream, ValueType VT, const char *key, const u8 key_size_in_bytes )
		{
		const u8 value_type = (u8)VT;
		const u64 start_pos = dstream.GetPosition();
		ISDSanityCheckDebugMacro( key_size_in_bytes <= EntityMaxKeyLength ); // max key length
		const u64 expected_end_pos = start_pos + key_size_in_bytes + 10; // sizeof(value_type)=1 + sizeof(block_size)=8 + sizeof(key_size_in_bytes)=1 + key_size_in_bytes;
		
		// write block header 
		// write empty stand in value for now (MAXi64 on purpose), which is definitely 
		// wrong, as to trigger any test if the value is not overwritten with the correct value
		dstream.Write( value_type );
		dstream.Write( (u64)MAXi64 );
		dstream.Write( key_size_in_bytes );
		dstream.Write( (i8 *)key, key_size_in_bytes );

		const u64 end_pos = dstream.GetPosition();
		ISDSanityCheckCoreDebugMacro( end_pos == expected_end_pos );
		return (end_pos == expected_end_pos);
		}

	// ends the block, write the size of the block
	bool end_write_large_block( MemoryWriteStream &dstream, u64 start_pos )
		{
		const u64 end_pos = dstream.GetPosition();
		const u64 block_size = end_pos - start_pos - 9; // total block size - ( sizeof( valuetype )=1 + sizeof( block_size_variable )=8 )
		dstream.SetPosition( start_pos + 1 ); // skip over the valuetype
		dstream.Write( block_size );
		dstream.SetPosition( end_pos ); // move back the where we were
		return (end_pos > start_pos); // only thing we really can check
		}

	// template to write array data to large block
	template<class I> bool write_large_block_array_data( 
		MemoryWriteStream &dstream, 
		bool has_values, 
		const I *data_items, 
		const size_t data_items_count,
		const size_t *data_index,
		const size_t data_index_count )
		{
		// assuming size_t is same size as u64, so no need for conversions
		// (when this assert fails, the code needs to be updated to support other sizes of size_t)
		static_assert(sizeof( u64 ) == sizeof( size_t ), "Unsupported size_t, current code requires it to be 8 bytes in size, equal to u64");
		static_assert(sizeof( I ) <= 0xff, "Invalid size of I");

		// record start of array data so we can check that it was written
		const u64 start_pos = dstream.GetPosition();

		// if empty data (not same as a zero-length array, which is not empty, only zero length)
		// note that zero-length data will have nullptr pointers, so we will not use that to deduce if there are values
		u64 expected_end_pos = 0;
		if( !has_values )
			{
			// empty data, mark flags field as empty
			dstream.Write( (u16)0 );

			// calc the end position, which is only the flags field 
			expected_end_pos = start_pos + sizeof( u16 );
			}
		else
			{
			// indexed array flags: size of each item (if need to decode array outside regular decoding) and bit set if index is used 
			const u16 per_item_size = (u16)sizeof( I ); // size of each item I (max 255 bytes per item)
			const u16 has_index = (data_index) ? 0x100 : 0x0;
			const u16 index_array_flags = has_index | per_item_size;
			dstream.Write( index_array_flags );

			// write the item count and items
			dstream.Write( u64(data_items_count) );
			dstream.Write( data_items, data_items_count );

			// if we have an index, write it 
			if( data_index != nullptr )
				{
				// cast to u64* and write to stream 
				// (the code assumes size_t and u64 are the same size, and can be cast, look at static_assert above)
				dstream.Write( data_index, data_index_count );
				}

			// make sure all data was written
			expected_end_pos = start_pos
				+ sizeof( u16 ) // the flags
				+ sizeof( u64 ) // the item count
				+ (data_items_count * sizeof( I )) // the items
				+ ((data_index_count * sizeof( u64 ))); // the list of indices, if any
			}

		// make sure we are at the expected end pos
		const u64 end_pos = dstream.GetPosition();
		if( end_pos != expected_end_pos )
			{
			ISDErrorLog << "End position of data " << end_pos << " do not equal the expected end position which is " << expected_end_pos << ISDErrorLogEnd;
			return false;
			}

		return true;
		}

	// write indexed array to stream
	template<ValueType VT, class T, class I, size_t I_per_T > bool write_array( 
		MemoryWriteStream &dstream, 
		const char *key, 
		const u8 key_size_in_bytes, 
		bool has_values, 
		const I *data_items, 
		const size_t data_items_count,
		const size_t *data_index,
		const size_t data_index_count )
		{
		// this code only supports values that are exactly the multiple I_per_T of the items it contins
		// eg basic types are ok, since I_per_T is 1. vec3 etc are also ok, as vec3 is I_per_T = 3
		static_assert(sizeof( I ) * I_per_T == sizeof( T ), "Size of T must be exactly size of I times I_per_T");

		// record start position, we need this in the end block
		const u64 start_pos = dstream.GetPosition();

		// begin a large block
		if( !begin_write_large_block( dstream, VT, key, key_size_in_bytes ) )
			{
			ISDErrorLog << "begin_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// write the array data
		if( !write_large_block_array_data<I>( dstream, has_values, data_items, data_items_count, data_index, data_index_count ) )
			{
			ISDErrorLog << "write_large_block_array_data() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// end the block by going back to the start and writing the start position offset
		if( !end_write_large_block( dstream, start_pos ) )
			{
			ISDErrorLog << "end_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// all blocks succeeded
		return true;
		}

	// specific write_array implementation for VT_Array_Bool, since we cant retrieve the data directly in bool vectors
	bool write_bool_array(
		MemoryWriteStream &dstream,
		const char *key,
		const u8 key_size_in_bytes,
		const std::vector<bool> *bool_vector,
		const std::vector<size_t> *index_vector )
		{
		u8 *data_items = nullptr;
		size_t data_items_count = 0;
		std::vector<u8> u8vec;

		const bool has_values = bool_vector != nullptr;
		if( has_values )
			{
			// pack the bool vector to a temporary u8 vector
			size_t number_of_packed_u8s = (bool_vector->size()+7) / 8; // round up, should the last u8 be not fully filled
			u8vec.resize( number_of_packed_u8s );
			for( size_t bool_index = 0; bool_index < bool_vector->size(); ++bool_index )
				{
				if( (*bool_vector)[bool_index] )
					{
					const size_t packed_index = bool_index / 8;
					const size_t packed_subindex = bool_index % 8;
					u8vec[packed_index] |= 1 << packed_subindex;
					}
				}

			// point at the temporary vector
			data_items = u8vec.data();
			data_items_count = u8vec.size();
			}

		const size_t *data_index = (index_vector) ? index_vector->data() : nullptr;
		const size_t data_index_count = (index_vector) ? index_vector->size() : 0;
		
		// record start position of the stream, we need this in the end block
		const u64 start_pos = dstream.GetPosition();

		// begin a large block
		if( !begin_write_large_block( dstream, ValueType::VT_Array_Bool, key, key_size_in_bytes ) )
			{
			ISDErrorLog << "begin_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// write the u8 array data items
		if( !write_large_block_array_data<u8>( dstream, has_values, data_items, data_items_count, data_index, data_index_count ) )
			{
			ISDErrorLog << "write_large_block_array_data() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// write the actual size of the bool array to the end of the data, so we can reconstruct the bool array when reading
		if( bool_vector )
			{
			dstream.Write( u64(bool_vector->size()) );
			}

		// end the block by going back to the start and writing the start position offset
		if( !end_write_large_block( dstream, start_pos ) )
			{
			ISDErrorLog << "end_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		return true;
		}

	// VT_Bool: bool
	template <> bool EntityWriter::Write<bool>( const char *key, const u8 key_length, const bool &src_variable )
		{
		u8 u8val = (u8)src_variable;
		return write_small_block<ValueType::VT_Bool, u8, 1>( dstream, key, key_length, &u8val );
		}

	// VT_Bool: optional_value<bool>
	template <> bool EntityWriter::Write<optional_value<bool>>( const char *key, const u8 key_length, const optional_value<bool> &src_variable )
		{
		u8 u8val = (src_variable.has_value()) ? (u8)src_variable.value() : false;
		return write_small_block<ValueType::VT_Bool, u8, 1>( dstream, key, key_length, (src_variable.has_value()) ? &u8val : nullptr );
		}

	// VT_Bool: std::vector<bool>
	template <> bool EntityWriter::Write<std::vector<bool>>( const char *key, const u8 key_length, const std::vector<bool> &src_variable )
		{
		return write_bool_array(
			this->dstream, 
			key, 
			key_length, 
			&src_variable, 
			nullptr
			);
		}

	// VT_Bool: optional_value<std::vector<bool>>
	template <> bool EntityWriter::Write<optional_value<std::vector<bool>>>( const char *key, const u8 key_length, const optional_value<std::vector<bool>> &src_variable )
		{
		return write_bool_array(
			this->dstream, 
			key, 
			key_length, 
			(src_variable.has_value()) ? &(src_variable.value()) : nullptr, 
			nullptr
			);
		}

	// VT_Bool: indexed_array<bool>
	template <> bool EntityWriter::Write<indexed_array<bool>>( const char *key, const u8 key_length, const indexed_array<bool> &src_variable )
		{
		return write_bool_array(
			this->dstream, 
			key, 
			key_length, 
			&src_variable.values(), 
			&src_variable.index()
			);
		}

	// VT_Bool: optional_value<indexed_array<bool>>
	template <> bool EntityWriter::Write<optional_value<indexed_array<bool>>>( const char *key, const u8 key_length, const optional_value<indexed_array<bool>> &src_variable )
		{
		return write_bool_array(
			this->dstream, 
			key, 
			key_length, 
			(src_variable.has_value()) ? &(src_variable.value().values()) : nullptr, 
			(src_variable.has_value()) ? &(src_variable.value().index()) : nullptr
			);
		}

	};
