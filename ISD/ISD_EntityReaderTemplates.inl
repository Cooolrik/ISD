// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "ISD_EntityReader.h"
#include "ISD_MemoryReadStream.h"
#include "ISD_Log.h"
#include "ISD_DataValuePointers.h"

// value_type: the ValueType enum to read the block as
// object_type: the C++ object that stores the data (can be a basic type), such as u32, or glm::vec3
// item_type: the actual basic type that stores the data, int the case of glm::vec3, it is a float
// item_count: the count of the the basic type, again in the case of glm::vec3, the count is 3

namespace ISD
	{
	enum class reader_status
		{
		fail, // general fail, (or value is empty, but empty is not allowed for this value)
		success_empty, // success, value is empty
		success // success, has value
		};

	// read the header of a large block
	// returns the stream position of the expected end of the block, to validate the read position
	// a stream position of 0 is not possible, and indicates error
	u64 begin_read_large_block( MemoryReadStream &sstream, ValueType VT, const char *key, const u8 key_size_in_bytes )
		{
		const u64 start_pos = sstream.GetPosition();
		ISDSanityCheckDebugMacro( key_size_in_bytes <= EntityMaxKeyLength ); // max key length

		// read and make sure we have the correct value type
		const u8 value_type = sstream.Read<u8>();
		if( value_type != (u8)VT )
			{
			// not the expected type
			ISDErrorLog << "The type in the input stream:" << value_type << " does not match expected type: " << (u8)VT << ISDErrorLogEnd;
			return 0;
			}

		// check the size, and calculate expected end position
		const u64 block_size = sstream.Read<u64>();
		const u64 expected_end_pos = sstream.GetPosition() + block_size;
		if( expected_end_pos > sstream.GetSize() )
			{
			// not the expected type
			ISDErrorLog << "The block size:" << block_size << " points beyond the end of the stream size: " << (u8)VT << ISDErrorLogEnd;
			return 0;
			}

		// read in the key length
		const u8 read_key_size_in_bytes = sstream.Read<u8>();
		if( read_key_size_in_bytes != key_size_in_bytes )
			{
			// not the expected type
			std::string expected_key_name( key, key_size_in_bytes );
			ISDErrorLog << "The size of the input key:" << read_key_size_in_bytes << " does not match expected size: " << key_size_in_bytes << " for key: \"" << expected_key_name << "\"" << ISDErrorLogEnd;
			return 0;
			}

		// read in the key data
		char read_key[EntityMaxKeyLength];
		sstream.Read( (i8 *)read_key, (u64)key_size_in_bytes );
		if( memcmp( key, read_key, (u64)key_size_in_bytes ) != 0 )
			{
			std::string expected_key_name( key, key_size_in_bytes );
			std::string read_key_name( read_key, key_size_in_bytes );
			ISDErrorLog << "Unexpected key name in the stream. Expected name: " << expected_key_name << " read name: " << read_key_name << ISDErrorLogEnd;
			return 0;
			}

		return expected_end_pos;
		}

	// ends the block, write the size of the block
	bool end_read_large_block( MemoryReadStream &sstream, u64 expected_end_pos )
		{
		const u64 end_pos = sstream.GetPosition();
		return (end_pos == expected_end_pos); // make sure we have read in the full block
		}

	// template method that Reads a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<ValueType VT, class T> reader_status read_single_item( MemoryReadStream &sstream, const char *key, const u8 key_size_in_bytes, const bool empty_value_is_allowed, T *dest_data )
		{
		static_assert((VT >= ValueType::VT_Bool) && (VT <= ValueType::VT_UUID), "Invalid type for generic template of read_single_item");

		const u64 value_size = sizeof( type_information<T>::value_type );
		const u64 value_count = type_information<T>::value_count;

		// record start position, for validation
		const u64 start_pos = sstream.GetPosition();

		// read u8 value_type, check the value
		const u8 value_type = sstream.Read<u8>();
		if( value_type != (u8)VT )
			{
			// not the expected type
			ISDErrorLog << "The type in the input stream:" << value_type << " does not match expected type: " << (u8)VT << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// calc the expected possible sizes. if empty value, the data size must be 0, else it is the expected size based on the item type (I) and count (IC)
		const u64 dest_data_size_in_bytes = value_size * value_count;
		const u64 expected_block_size_if_empty = key_size_in_bytes;
		const u64 expected_block_size = dest_data_size_in_bytes + expected_block_size_if_empty;

		ISDSanityCheckCoreDebugMacro( key_size_in_bytes <= EntityMaxKeyLength );
		ISDSanityCheckCoreDebugMacro( expected_block_size < 256 ); // must fit in a byte

		// read in size of the small block, if the size does not match the expected block size, check if empty value is ok (is_optional_value == true), and if not raise error
		// any size other than expected_block_size is regarded as empty, and we will check that size if empty is actually allowed
		const u64 block_size = sstream.Read<u8>();
		const bool is_empty_value = (block_size != expected_block_size) ? true : false;
		if( is_empty_value )
			{
			if( empty_value_is_allowed )
				{
				// if empty is allowed, make sure that we have the block size of an empty block
				if( block_size != expected_block_size_if_empty )
					{
					ISDErrorLog << "The size of the block in the input stream:" << block_size << " does not match expected possible sizes: " << expected_block_size_if_empty << " (if empty value) or " << expected_block_size << " (if non-empty) " << ISDErrorLogEnd;
					return reader_status::fail;
					}
				}
			else
				{
				// empty is not allowed, so regardless of the size, it is invalid, error out
				ISDErrorLog << "The size of the block in the input stream:" << block_size << " does not match expected size (empty is not allowed): " << expected_block_size << ISDErrorLogEnd;
				return reader_status::fail;
				}
			}

		// read in the value(s)
		if( !is_empty_value )
			{
			const u64 read_count = sstream.Read( value_ptr( *dest_data ), value_count );
			if( read_count != value_count )
				{
				ISDErrorLog << "Could not read all expected values from the input stream. Expected count: " << value_count << " read count: " << read_count << ISDErrorLogEnd;
				return reader_status::fail;
				}
			}

		// read in the key data
		char read_key[EntityMaxKeyLength];
		const u64 read_key_length = sstream.Read( (u8 *)read_key, (u64)key_size_in_bytes );
		if( read_key_length != (u64)key_size_in_bytes
			|| memcmp( key, read_key, (u64)key_size_in_bytes ) != 0 )
			{
			ISDSanityCheckCoreDebugMacro( read_key_length < (u64)key_size_in_bytes );
			std::string expected_key_name( key, key_size_in_bytes );
			std::string read_key_name( read_key, read_key_length );
			ISDErrorLog << "Unexpected key name in the stream. Expected name: " << expected_key_name << " read name: " << read_key_name << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// get the position beyond the end of the block, and validate position
		const u64 expected_end_pos = (is_empty_value) ? (start_pos + 2 + expected_block_size_if_empty) : (start_pos + 2 + expected_block_size);
		const u64 end_pos = sstream.GetPosition();
		ISDSanityCheckCoreDebugMacro( end_pos == expected_end_pos );
		if( end_pos != expected_end_pos )
			{
			ISDErrorLog << "Invaild position in the read stream. Expected position: " << expected_end_pos << " current position: " << end_pos << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// return success, either with value or empty
		if( is_empty_value )
			return reader_status::success_empty;
		return reader_status::success;
		};

	// special implementation of read_small_block for bool values, which reads a u8 and converts to bool
	template<> reader_status read_single_item<ValueType::VT_Bool, bool>( MemoryReadStream &sstream, const char *key, const u8 key_size_in_bytes, const bool empty_value_is_allowed, bool *dest_data )
		{
		u8 u8val;
		reader_status status = read_single_item<ValueType::VT_Bool, u8>( sstream, key, key_size_in_bytes, empty_value_is_allowed, &u8val );
		if( status != reader_status::fail )
			{
			(*dest_data) = (bool)u8val;
			}
		return status;
		};

	// template method that Reads a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<> reader_status read_single_item<ValueType::VT_String, string>( MemoryReadStream &sstream, const char *key, const u8 key_size_in_bytes, const bool empty_value_is_allowed, string *dest_data )
		{
		static_assert(sizeof( u64 ) == sizeof( size_t ), "Unsupported size_t, current code requires it to be 8 bytes in size, equal to u64");

		ISDSanityCheckCoreDebugMacro( dest_data );

		// read block header
		const u64 expected_end_position = begin_read_large_block( sstream, ValueType::VT_String, key, key_size_in_bytes );
		if( expected_end_position == 0 )
			{
			ISDErrorLog << "begin_read_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// if we are at the end of the block, we have an empty string
		if( sstream.GetPosition() == expected_end_position )
			{
			// marked as empty, check that this is allowed
			if( empty_value_is_allowed )
				{
				// empty value is allowed, early out if the block end checks out
				if( !end_read_large_block( sstream, expected_end_position ) )
					{
					ISDErrorLog << "End position of data " << sstream.GetPosition() << " does not equal the expected end position which is " << expected_end_position << ISDErrorLogEnd;
					return reader_status::fail;
					}

				// all good
				return reader_status::success_empty;
				}
			else
				{
				// empty is not allowed
				ISDErrorLog << "The read stream value is empty, which is not allowed for value \"" << key << "\"" << ISDErrorLogEnd;
				return reader_status::fail;
				}
			}

		// non-empty, read in the string size
		const u64 string_size = sstream.Read<u64>();

		// make sure the item count is plausible before allocating the vector
		const u64 expected_string_size = (expected_end_position - sstream.GetPosition());
		if( string_size > expected_string_size )
			{
			ISDErrorLog << "The string size in the stream is invalid, it is beyond the size of the value block" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// resize the destination string
		dest_data->resize( string_size );

		// read in the string data
		i8 *p_data = (i8 *)&(dest_data->front());
		const u64 read_item_count = sstream.Read( p_data, string_size );
		if( read_item_count != string_size )
			{
			ISDErrorLog << "The stream could not read the whole string" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// make sure we are at the expected end pos
		if( !end_read_large_block( sstream, expected_end_position ) )
			{
			ISDErrorLog << "End position of data " << sstream.GetPosition() << " does not equal the expected end position which is " << expected_end_position << ISDErrorLogEnd;
			return reader_status::fail;
			}

		return reader_status::success;
		}

	reader_status end_read_empty_large_block( MemoryReadStream &sstream, const char *key, const bool empty_value_is_allowed, const u64 expected_end_position )
		{
		// check that empty value this is allowed
		if( empty_value_is_allowed )
			{
			// empty value is allowed, early out if the block end checks out
			if( !end_read_large_block( sstream, expected_end_position ) )
				{
				ISDErrorLog << "End position of data " << sstream.GetPosition() << " does not equal the expected end position which is " << expected_end_position << ISDErrorLogEnd;
				return reader_status::fail;
				}

			// all good
			return reader_status::success_empty;
			}
		else
			{
			// empty is not allowed
			ISDErrorLog << "The read stream value is empty, which is not allowed for value \"" << key << "\"" << ISDErrorLogEnd;
			return reader_status::fail;
			}
		}

	bool read_array_index( MemoryReadStream &sstream, const bool has_index , const u64 expected_end_position , std::vector<i32> *dest_index )
		{
		// if we have an index, read it
		if( has_index )
			{
			// make sure we DO expect an index
			if( !dest_index )
				{
				ISDErrorLog << "Invalid array type: The stream type has an index, but the destination object does not." << ISDErrorLogEnd;
				return false;
				}

			// read in the size of the index
			ISDSanityCheckCoreDebugMacro( expected_end_position >= sstream.GetPosition() );
			const u64 index_count = sstream.Read<u64>();
			const u64 maximum_possible_index_count = (expected_end_position - sstream.GetPosition()) / sizeof( u32 );
			if( index_count > maximum_possible_index_count )
				{
				ISDErrorLog << "The index item count in the stream is invalid, it is beyond the size of the block" << ISDErrorLogEnd;
				return false;
				}

			// resize the dest vector
			dest_index->resize( index_count );

			// read in the data
			i32 *p_index_data = dest_index->data();
			sstream.Read( p_index_data, index_count );
			}
		else
			{
			// make sure we do NOT expect an index
			if( dest_index )
				{
				ISDErrorLog << "Invalid array type: The stream type has does not have an index, but the destination object does." << ISDErrorLogEnd;
				return false;
				}
			}
		return true;
		}

	template<ValueType VT, class T> reader_status read_array( MemoryReadStream &sstream, const char *key, const u8 key_size_in_bytes, const bool empty_value_is_allowed, std::vector<T> *dest_items, std::vector<i32> *dest_index )
		{
		static_assert((VT >= ValueType::VT_Array_Bool) && (VT <= ValueType::VT_Array_UUID), "Invalid type for generic read_array template");
		static_assert(sizeof( u64 ) >= sizeof( size_t ), "Unsupported size_t, current code requires it to be at max 8 bytes in size, equal to u64");

		ISDSanityCheckCoreDebugMacro( dest_items );

		// read block header
		const u64 expected_end_position = begin_read_large_block( sstream, VT, key, key_size_in_bytes );
		if( expected_end_position == 0 )
			{
			ISDErrorLog << "begin_read_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// read in flags, if 0 the block is empty
		const u16 array_flags = sstream.Read<u16>();
		if( array_flags == 0 )
			{
			return end_read_empty_large_block( sstream, key, empty_value_is_allowed, expected_end_position );
			}

		// non-empty, decode the data
		const u8 per_item_size = (u8)(array_flags & 0xff);
		const bool has_index = (array_flags & 0x100) != 0;

		// make sure we have the right item size
		const u8 value_size = u8(sizeof( type_information<T>::value_type ));
		if( value_size != per_item_size )
			{
			ISDErrorLog << "The size of the items in the stream does not match the expected size" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// read in the item count
		const u64 item_count = sstream.Read<u64>();

		// make sure the item count is plausible before allocating the vector
		const u64 maximum_possible_item_count = (expected_end_position - sstream.GetPosition()) / value_size;
		if( item_count > maximum_possible_item_count )
			{
			ISDErrorLog << "The array item count in the stream is invalid, it is beyond the size of the block" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// resize the destination vector
		const u64 type_count = item_count / type_information<T>::value_count;
		dest_items->resize( type_count );

		// read in the data
		T *p_data = dest_items->data();
		const u64 read_item_count = sstream.Read( value_ptr( *p_data ), item_count );
		if( read_item_count != item_count )
			{
			ISDErrorLog << "The stream could not read all the items for the array" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// read index if it exists, or make sure we do not expect an index
		if( !read_array_index( sstream, has_index, expected_end_position, dest_index ) )
			{
			return reader_status::fail;
			}

		// make sure we are at the expected end pos
		if( !end_read_large_block( sstream, expected_end_position ) )
			{
			ISDErrorLog << "End position of data " << sstream.GetPosition() << " does not equal the expected end position which is " << expected_end_position << ISDErrorLogEnd;
			return reader_status::fail;
			}

		return reader_status::success;
		}

	// read_array implementation for bool arrays (which need specific packing)
	template <> reader_status read_array<ValueType::VT_Array_Bool, bool>( MemoryReadStream &sstream, const char *key, const u8 key_size_in_bytes, const bool empty_value_is_allowed, std::vector<bool> *dest_items, std::vector<i32> *dest_index )
		{
		ISDSanityCheckCoreDebugMacro( dest_items );

		// read block header
		const u64 expected_end_position = begin_read_large_block( sstream, ValueType::VT_Array_Bool, key, key_size_in_bytes );
		if( expected_end_position == 0 )
			{
			ISDErrorLog << "begin_read_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// read in flags, if 0 the block is empty
		const u16 array_flags = sstream.Read<u16>();
		if( array_flags == 0 )
			{
			return end_read_empty_large_block( sstream, key, empty_value_is_allowed, expected_end_position );
			}

		// non-empty, decode the data
		const bool has_index = (array_flags & 0x100) != 0;

		// read in the item count
		const u64 value_count = sstream.Read<u64>();

		// calculate the number of packed items.
		// round up, should the last u8 be not fully filled
		const u64 number_of_packed_u8s = (value_count+7) / 8; 

		// make sure the item count is plausible before allocating the vector
		const u64 maximum_possible_item_count = (expected_end_position - sstream.GetPosition());
		if( number_of_packed_u8s > maximum_possible_item_count )
			{
			ISDErrorLog << "The array item count in the stream is invalid, it is beyond the size of the block" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// resize the destination vector
		dest_items->resize( value_count );

		// read in a u8 vector with the packed values
		std::vector<u8> packed_vec( number_of_packed_u8s );
		sstream.Read( packed_vec.data(), number_of_packed_u8s );

		// unpack the bool vector from the u8 vector
		for( u64 bool_index = 0; bool_index < value_count; ++bool_index )
			{
			const size_t packed_index = bool_index >> 3; // bool_index / 8
			const size_t packed_subindex = bool_index & 0x7; // bool_index % 8
			(*dest_items)[bool_index] = ((packed_vec[packed_index]) & (1 << packed_subindex)) != 0;
			}

		// read index if it exists, or make sure we do not expect an index
		if( !read_array_index( sstream, has_index, expected_end_position, dest_index ) )
			{
			return reader_status::fail;
			}

		// make sure we are at the expected end pos
		if( !end_read_large_block( sstream, expected_end_position ) )
			{
			ISDErrorLog << "End position of data " << sstream.GetPosition() << " does not equal the expected end position which is " << expected_end_position << ISDErrorLogEnd;
			return reader_status::fail;
			}

		return reader_status::success;
		}

	template<> reader_status read_array<ValueType::VT_Array_String, string>( MemoryReadStream &sstream, const char *key, const u8 key_size_in_bytes, const bool empty_value_is_allowed, std::vector<string> *dest_items, std::vector<i32> *dest_index )
		{
		static_assert(sizeof( u64 ) == sizeof( size_t ), "Unsupported size_t, current code requires it to be 8 bytes in size, equal to u64");

		ISDSanityCheckCoreDebugMacro( dest_items );

		// read block header
		const u64 expected_end_position = begin_read_large_block( sstream, ValueType::VT_Array_String, key, key_size_in_bytes );
		if( expected_end_position == 0 )
			{
			ISDErrorLog << "begin_read_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// read in flags, if 0 the block is empty
		const u16 array_flags = sstream.Read<u16>();
		if( array_flags == 0 )
			{
			return end_read_empty_large_block( sstream, key, empty_value_is_allowed, expected_end_position );
			}

		// non-empty, decode the data
		const bool has_index = (array_flags & 0x100) != 0;

		// read in the number of strings in the array
		const u64 string_count = sstream.Read<u64>();

		// make sure the item count is plausible before allocating the vector
		// (the size is assuming only empty strings, so only the size u64 per string)
		const u64 maximum_possible_item_count = (expected_end_position - sstream.GetPosition()) / sizeof(u64); 
		if( string_count > maximum_possible_item_count )
			{
			ISDErrorLog << "The array string count in the stream is invalid, it is beyond the size of the block" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// resize the destination vector
		dest_items->resize( string_count );

		// read in each string separately
		for( u64 string_index = 0; string_index < string_count; ++string_index )
			{
			string &dest_string = (*dest_items)[string_index];

			const u64 string_size = sstream.Read<u64>();

			// make sure the string is not outsize of possible size
			const u64 maximum_possible_string_size = (expected_end_position - sstream.GetPosition()); 
			if( string_size > maximum_possible_string_size )
				{
				ISDErrorLog << "A string size in a string array in the stream is invalid, it is beyond the size of the block" << ISDErrorLogEnd;
				return reader_status::fail;
				}

			// setup the destination string, and read in the data
			dest_string.resize( string_size );
			if( string_size > 0 )
				{
				i8 *p_data = (i8 *)&(dest_string.front());
				const u64 read_item_count = sstream.Read( p_data, string_size );
				if( read_item_count != string_size )
					{
					ISDErrorLog << "The stream could not read one of the strings" << ISDErrorLogEnd;
					return reader_status::fail;
					}
				}
			}

		// read index if it exists, or make sure we do not expect an index
		if( !read_array_index( sstream, has_index, expected_end_position, dest_index ) )
			{
			return reader_status::fail;
			}

		// make sure we are at the expected end pos
		if( !end_read_large_block( sstream, expected_end_position ) )
			{
			ISDErrorLog << "End position of data " << sstream.GetPosition() << " does not equal the expected end position which is " << expected_end_position << ISDErrorLogEnd;
			return reader_status::fail;
			}

		return reader_status::success;
		}



	};
