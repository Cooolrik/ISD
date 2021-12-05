// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"
#include "ISD_DataValuePointers.h"
#include "ISD_Log.h"

namespace ISD
	{
	// template method that writes a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<ValueType VT, class T> bool write_small_block( 
		MemoryWriteStream &dstream, 
		const char *key, 
		const u8 key_length, 
		const T *data)
		{
		static_assert((VT >= ValueType::VT_Bool) && (VT <= ValueType::VT_UUID), "Invalid type for write_small_block");

		const u8 value_type = (u8)VT;
		const size_t value_size = sizeof( type_information<T>::value_type );
		const size_t value_count = type_information<T>::value_count;
		const size_t data_size_in_bytes = (data != nullptr) ? (value_size * value_count) : 0; // if data == nullptr, the block is empty
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
			const type_information<T>::value_type *pvalue = value_ptr( (*data) );
			dstream.Write( pvalue, value_count );
			}
		dstream.Write( (i8*)key, key_length );

		const u64 end_pos = dstream.GetPosition();
		ISDSanityCheckCoreDebugMacro( end_pos == expected_end_pos );
		return (dstream.GetPosition() == expected_end_pos);
		};

	// specialization of write_small_block for bool values, which need conversion to u8
	template<> bool write_small_block<ValueType::VT_Bool, bool>( MemoryWriteStream &dstream, const char *key, const u8 key_length, const bool *data )
		{
		// if data is set, convert to an u8 value, and point at it
		const u8 u8val = ( data ) ? ((u8)(*data)) : 0;
		const u8 *p_data = ( data ) ? (&u8val) : nullptr;
		return write_small_block<ValueType::VT_Bool, u8>( dstream, key, key_length, p_data );
		}

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
		dstream.Write( (u64)MAXINT );
		dstream.Write( key_size_in_bytes );
		dstream.Write( (i8*)key, key_size_in_bytes );

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

	// write indexed array to stream
	template<ValueType VT, class T> bool write_array( 
		MemoryWriteStream &dstream, 
		const char *key, 
		const u8 key_size_in_bytes, 
		const std::vector<T> *items, 
		const std::vector<size_t> *index)
		{
		static_assert((VT >= ValueType::VT_Array_Bool) && (VT <= ValueType::VT_Array_UUID), "Invalid type for write_array");
		static_assert(sizeof( type_information<T>::value_type ) <= 0xff, "Invalid value size, cannot exceed 255 bytes");
		static_assert(sizeof( u64 ) == sizeof( size_t ), "Unsupported size_t, current code requires it to be 8 bytes in size, equal to an u64"); // assuming sizeof(u64) == sizeof(size_t)
		const size_t value_size = sizeof( type_information<T>::value_type );
		const size_t value_count = type_information<T>::value_count;

		// record start position, we need this in the end block
		const u64 start_pos = dstream.GetPosition();

		// begin a large block
		if( !begin_write_large_block( dstream, VT, key, key_size_in_bytes ) )
			{
			ISDErrorLog << "begin_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}
		
		// record start of the array data, for error check
		const u64 array_data_start_pos = dstream.GetPosition();

		// if empty items vector
		u64 expected_end_pos = 0;
		if( !items )
			{
			// empty data, mark flags field as empty
			dstream.Write( (u16)0 );

			// calc the end position, which is only the flags field 
			expected_end_pos = array_data_start_pos + sizeof( u16 );
			}
		else
			{
			// indexed array flags: size of each item (if need to decode array outside regular decoding) and bit set if index is used 
			const u16 has_index = (index) ? (0x100) : (0);
			const u16 array_flags = has_index | u16(value_size);
			dstream.Write( array_flags );

			// write the value count and values
			const u64 values_count = items->size() * value_count;
			const u64 values_size = (values_count * value_size) + sizeof( u64 );
			dstream.Write( values_count );
			if( values_count > 0 )
				{
				const type_information<T>::value_type *p_values = value_ptr( *(items->data()) );
				dstream.Write( p_values , values_count );
				}

			// if we have an index, write it 
			u64 index_size = 0;
			if( index != nullptr )
				{
				// cast to u64* and write to stream 
				// (the code assumes size_t and u64 are the same size, and can be cast, look at static_assert above)
				const u64 index_count = index->size();
				dstream.Write( index_count );
				dstream.Write( index->data(), index_count );

				index_size = (index_count * sizeof( u64 )) + sizeof( u64 ); // the index values and the value count
				}

			// make sure all data was written
			expected_end_pos = 
				array_data_start_pos
				+ sizeof( u16 ) // the flags
				+ values_size // the values 
				+ index_size; // the (optional) index
			}

		// end the block by going back to the start and writing the start position offset
		if( !end_write_large_block( dstream, start_pos ) )
			{
			ISDErrorLog << "end_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// make sure we are at the expected end pos
		const u64 end_pos = dstream.GetPosition();
		if( end_pos != expected_end_pos )
			{
			ISDErrorLog << "End position of data " << end_pos << " do not equal the expected end position which is " << expected_end_pos << ISDErrorLogEnd;
			return false;
			}

		// succeeded
		return true;
		}


	// specialization of write_array for bool arrays
	template<> bool write_array<ValueType::VT_Array_Bool, bool>( 
		MemoryWriteStream &dstream, 
		const char *key, 
		const u8 key_size_in_bytes, 
		const std::vector<bool> *items, 
		const std::vector<size_t> *index
		)
		{
		// record start position, we need this in the end block
		const u64 start_pos = dstream.GetPosition();

		// begin a large block
		if( !begin_write_large_block( dstream, ValueType::VT_Array_Bool, key, key_size_in_bytes ) )
			{
			ISDErrorLog << "begin_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// record start of the array data, for error check
		const u64 array_data_start_pos = dstream.GetPosition();

		// if empty items vector
		u64 expected_end_pos = 0;
		if( !items )
			{
			// empty data, mark flags field as empty
			dstream.Write( (u16)0 );

			// calc the end position, which is only the flags field 
			expected_end_pos = array_data_start_pos + sizeof( u16 );
			}
		else
			{
			const u16 has_index = (index) ? (0x100) : (0);
			const u16 array_flags = has_index | 0x1; // lowest bit marks that the array is not empty
			dstream.Write( array_flags );

			// write the item count and items
			const u64 items_count = u64( items->size() );
			const u64 number_of_packed_u8s = (items_count+7) / 8; 
			const u64 values_size = number_of_packed_u8s + sizeof( u64 );
			dstream.Write( items_count );
			if( items_count > 0 )
				{
				// pack the bool vector to a temporary u8 vector
				// round up, should the last u8 be not fully filled
				std::vector<u8> packed_vec( number_of_packed_u8s );
				for( size_t bool_index = 0; bool_index < items->size(); ++bool_index )
					{
					if( (*items)[bool_index] )
						{
						const size_t packed_index = bool_index >> 3; // bool_index / 8
						const size_t packed_subindex = bool_index & 0x7; // bool_index % 8
						packed_vec[packed_index] |= 1 << packed_subindex;
						}
					}

				// write u8 vector to stream
				dstream.Write( packed_vec.data(), number_of_packed_u8s );
				}

			// if we have an index, write it 
			u64 index_size = 0;
			if( index != nullptr )
				{
				// cast to u64* and write to stream 
				// (the code assumes size_t and u64 are the same size, and can be cast, look at static_assert above)
				const u64 index_count = index->size();
				dstream.Write( index_count );
				dstream.Write( index->data(), index_count );

				index_size = (index_count * sizeof( u64 )) + sizeof( u64 ); // the index values and the value count
				}

			// make sure all data was written
			expected_end_pos = 
				array_data_start_pos
				+ sizeof( u16 ) // the flags
				+ values_size // the values 
				+ index_size; // the (optional) index
			}

		// end the block by going back to the start and writing the start position offset
		if( !end_write_large_block( dstream, start_pos ) )
			{
			ISDErrorLog << "end_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// make sure we are at the expected end pos
		const u64 end_pos = dstream.GetPosition();
		if( end_pos != expected_end_pos )
			{
			ISDErrorLog << "End position of data " << end_pos << " do not equal the expected end position which is " << expected_end_pos << ISDErrorLogEnd;
			return false;
			}

		// succeeded
		return true;
		}

	};
