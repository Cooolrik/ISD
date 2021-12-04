// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "ISD_EntityReader.h"
#include "ISD_MemoryReadStream.h"
#include "ISD_Log.h"

#include <glm/gtc/type_ptr.hpp>

// value_type: the ValueType enum to read the block as
// object_type: the C++ object that stores the data (can be a basic type), such as u32, or glm::vec3
// item_type: the actual basic type that stores the data, int the case of glm::vec3, it is a float
// item_count: the count of the the basic type, again in the case of glm::vec3, the count is 3

namespace ISD
	{
	enum class reader_status
		{
		fail, // general fail, (or value was empty, but empty is not allowed for this value)
		success_empty, // success, value was empty
		success // success, has value
		};

	// template method that Reads a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<ValueType VT, class I, size_t IC, bool empty_value_is_allowed> reader_status read_small_block( MemoryReadStream &sstream, const char *key, const u8 key_size_in_bytes, I *dest_data )
		{
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
		const u64 dest_data_size_in_bytes = sizeof( I ) * IC;
		const u64 expected_block_size_if_empty = key_size_in_bytes;
		const u64 expected_block_size = dest_data_size_in_bytes + expected_block_size_if_empty;

		ISDSanityCheckCoreDebugMacro( key_size_in_bytes <= EntityMaxKeyLength ); 
		ISDSanityCheckCoreDebugMacro( expected_block_size < 256 ); // must fit in a byte

		// read in size of the small block, if the size does not match the expected block size, check if empty value is ok (is_optional_value == true), and if not raise error
		// any size other than expected_block_size is regarded as empty, and we will check that size if empty is actually allowed
		const u64 block_size = sstream.Read<u8>();
		const bool is_empty_value = (block_size != expected_block_size) ? true : false;
		if( is_empty_value  )
			{
			if( empty_value_is_allowed )
				{
				// if empty is allowed, make sure that we have that block size.
				if( block_size != expected_block_size_if_empty )
					{
					ISDErrorLog << "The size of the block in the input stream:" << block_size << " does not match expected possible sizes: " << expected_block_size_if_empty << " (if empty value) or " << expected_block_size << " (if non-empty) " << ISDErrorLogEnd;
					return reader_status::fail;
					}
				}
			else
				{
				// empty is not allowed, so regardless of the size, it is wrong, error out
				ISDErrorLog << "The size of the block in the input stream:" << block_size << " does not match expected possible size (empty is not allowed): " << expected_block_size << ISDErrorLogEnd;
				return reader_status::fail;
				}
			}

		// read in the value(s), IC is the number of values
		if( !is_empty_value )
			{
			const u64 read_count = sstream.Read( dest_data , IC );
			if( read_count != IC )
				{
				ISDErrorLog << "Could not read all expected values from the input stream. Expected count: " << IC << " read count: " << read_count << ISDErrorLogEnd;
				return reader_status::fail;
				}
			}

		// read in the key data
		char read_key[EntityMaxKeyLength];
		const u64 read_key_length = sstream.Read( (i8*)read_key, (u64)key_size_in_bytes );
		if( read_key_length != (u64)key_size_in_bytes 
			|| memcmp(key,read_key,(u64)key_size_in_bytes)!=0 )
			{
			std::string expected_key_name( key, key_size_in_bytes );
			std::string read_key_name( read_key, key_size_in_bytes );
			ISDErrorLog << "Unexpected key name in the stream. Expected name: " << expected_key_name << " read name: " << read_key_name << ISDErrorLogEnd;
			return reader_status::fail;;
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

	// returns the stream position of the start of the block, to be used when writing the size when ending the block
	std::pair<bool,u64> begin_read_large_block( MemoryReadStream &sstream, ValueType VT, const char *key, const u8 key_size_in_bytes )
		{
		const u64 start_pos = sstream.GetPosition();
		ISDSanityCheckDebugMacro( key_size_in_bytes <= EntityMaxKeyLength ); // max key length

		// read and make sure we have the correct value type
		const u8 value_type = sstream.Read<u8>();
		if( value_type != (u8)VT )
			{
			// not the expected type
			ISDErrorLog << "The type in the input stream:" << value_type << " does not match expected type: " << (u8)VT << ISDErrorLogEnd;
			return std::pair<bool,u64>(false,0);
			}

		// check the size, and calculate expected end position
		const u64 block_size = sstream.Read<u64>();
		const u64 expected_end_pos = sstream.GetPosition() + block_size;

		// read in the key length
		const u8 read_key_size_in_bytes = sstream.Read<u8>();
		if( read_key_size_in_bytes != key_size_in_bytes )
			{
			// not the expected type
			std::string expected_key_name( key, key_size_in_bytes );
			ISDErrorLog << "The size of the input key:" << read_key_size_in_bytes << " does not match expected size: " << key_size_in_bytes << " for key: \"" << expected_key_name << "\"" << ISDErrorLogEnd;
			return std::pair<bool,u64>(false,0);
			}

		// read in the key data
		char read_key[EntityMaxKeyLength];
		sstream.Read( (i8*)read_key, (u64)key_size_in_bytes );
		if( memcmp(key,read_key,(u64)key_size_in_bytes) != 0 )
			{
			std::string expected_key_name( key, key_size_in_bytes );
			std::string read_key_name( read_key, key_size_in_bytes );
			ISDErrorLog << "Unexpected key name in the stream. Expected name: " << expected_key_name << " read name: " << read_key_name << ISDErrorLogEnd;
			return std::pair<bool,u64>(false,0);
			}

		return std::pair<bool,u64>(true,expected_end_pos);
		}

	// ends the block, write the size of the block
	bool end_read_large_block( MemoryReadStream &sstream, u64 expected_end_pos )
		{
		const u64 end_pos = sstream.GetPosition();
		return (end_pos == expected_end_pos); // make sure we have read in the full block
		}

	// class for reading arrays	
	template<class I, bool empty_value_is_allowed> class read_array
		{
		private:
			MemoryReadStream &sstream;

			u64 start_pos = 0;
			u64 expected_end_pos = 0;
			u16 index_array_flags = 0;

		public:
			read_array( MemoryReadStream &_sstream ) : sstream( _sstream ) {}

			reader_status read_header();

			bool read_data(
				I *data_items,
				size_t data_items_count,
				size_t *data_index,
				size_t data_index_count
			);
		};
			
	template<class I, bool empty_value_is_allowed> reader_status read_large_block_array<I,empty_value_is_allowed>::read_header()
		{
		// assuming size_t is same size as u64, so no need for conversions
		// (when this assert fails, the code needs to be updated to support other sizes of size_t)
		static_assert(sizeof( u64 ) == sizeof( size_t ), "Unsupported size_t, current code requires it to be 8 bytes in size, equal to u64");
		static_assert(sizeof( I ) <= 0xff, "Invalid size of I");

		// record start of array data so we can check that it was written
		this->start_pos = sstream.GetPosition();

		// read in flags
		this->index_array_flags = sstream.Read<u16>();
		if( this->index_array_flags == 0 )
			{
			if( empty_value_is_allowed )
				{
				// empty value is allowed, return success if we have the right position
				this->expected_end_pos = this->start_pos + sizeof( u16 );
				if( sstream.GetPosition() == expected_end_pos )
					return reader_status::success_empty;
				else
					return reader_status::fail;
				}
			else
				{
				// empty is not allowed
				ISDErrorLog << "The read stream value is empty, which is not allowed for this value" << ISDErrorLogEnd;
				return reader_status::fail;
				}
			}

		// non-empty, decode the data
		const u8 per_item_size = (u8)(index_array_flags & 0xff);
		const bool has_index = (index_array_flags & 0x100) != 0;

		// make sure we have the right item size
		if( (u8)sizeof( I ) != per_item_size )
			{
			ISDErrorLog << "The size of the items in the stream does not match the expected size" << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// read in the item count

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


	// VT_Bool 
	template <> bool EntityReader::Read<bool>( const char *key, const u8 key_length, bool &value )
		{
		u8 u8val;
		if( read_small_block<ValueType::VT_Bool, u8, 1, false>( this->sstream, key, key_length, &u8val ) == reader_status::fail )
			return false;
		value = (bool)u8val;
		return true;
		}
	
	template <> bool EntityReader::Read<optional_value<bool>>( const char *key, const u8 key_length, optional_value<bool> &value )
		{
		u8 u8val;
		reader_status status = read_small_block<ValueType::VT_Bool,u8,1,true>( this->sstream, key, key_length, &u8val );

		if( status == reader_status::fail ) 
			return false;
		else if( status == reader_status::success )
			value.set( (bool)u8val );
		else
			value.clear();

		return true;
		}

	};
