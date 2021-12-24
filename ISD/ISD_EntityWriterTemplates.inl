// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"
#include "ISD_DataValuePointers.h"
#include "ISD_Log.h"

namespace ISD
	{
	// called to begin a large block
	// returns the stream position of the start of the block, to be used when writing the size when ending the block
	bool begin_write_large_block( MemoryWriteStream &dstream, ValueType VT, const char *key, const u8 key_size_in_bytes )
		{
		const u8 value_type = (u8)VT;
		const u64 start_pos = dstream.GetPosition();
		ISDSanityCheckDebugMacro( key_size_in_bytes <= EntityMaxKeyLength ); 

		// sizeof(value_type)=1 + sizeof(block_size)=8 + sizeof(key_size_in_bytes)=1 + key_size_in_bytes;
		const u64 expected_end_pos = start_pos + key_size_in_bytes + 10; 

		// write block header 
		// write empty stand in value for now (MAXi64 on purpose), which is definitely 
		// wrong, as to trigger any test if the value is not overwritten with the correct value
		dstream.Write( value_type );
		dstream.Write( (u64)MAXINT64 );
		dstream.Write( key_size_in_bytes );
		dstream.Write( (i8*)key, key_size_in_bytes );

		const u64 end_pos = dstream.GetPosition();
		ISDSanityCheckCoreDebugMacro( end_pos == expected_end_pos );
		return (end_pos == expected_end_pos);
		}

	// ends the block
	// writes the size of the block in the header of the block
	bool end_write_large_block( MemoryWriteStream &dstream, u64 start_pos )
		{
		const u64 end_pos = dstream.GetPosition();
		const u64 block_size = end_pos - start_pos - 9; // total block size - ( sizeof( valuetype )=1 + sizeof( block_size_variable )=8 )
		dstream.SetPosition( start_pos + 1 ); // skip over the valuetype
		dstream.Write( block_size );
		dstream.SetPosition( end_pos ); // move back the where we were
		return (end_pos > start_pos); // only thing we really can check
		}

	// template method that writes a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<ValueType VT, class T> bool write_single_value( MemoryWriteStream &dstream, const char *key, const u8 key_length, const T *data )
		{
		static_assert((VT >= ValueType::VT_Bool) && (VT <= ValueType::VT_UUID), "Invalid type for general write_single_value template");

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

	// specialization of write_single_value for bool values, which need conversion to u8
	template<> bool write_single_value<ValueType::VT_Bool, bool>( MemoryWriteStream &dstream, const char *key, const u8 key_length, const bool *data )
		{
		// if data is set, convert to an u8 value, and point at it
		const u8 u8val = ( data ) ? ((u8)(*data)) : 0;
		const u8 *p_data = ( data ) ? (&u8val) : nullptr;
		return write_single_value<ValueType::VT_Bool, u8>( dstream, key, key_length, p_data );
		}

	// specialization of write_single_value for strings
	template<> bool write_single_value<ValueType::VT_String, std::string>( MemoryWriteStream &dstream, const char *key, const u8 key_size_in_bytes, const std::string *string_value )
		{
		// record start position, we need this in the end block
		const u64 start_pos = dstream.GetPosition();

		// begin a large block
		if( !begin_write_large_block( dstream, ValueType::VT_String, key, key_size_in_bytes ) )
			{
			ISDErrorLog << "begin_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
			return false;
			}

		// record start of the array data, for error check
		const u64 string_data_start_pos = dstream.GetPosition();

		// if empty value (not same as empty string, which just has size 0)
		if( !string_value )
			{
			// empty string, early out 
			if( !end_write_large_block( dstream, start_pos ) )
				{
				ISDErrorLog << "end_write_large_block() failed unexpectedly" << ISDErrorLogEnd;
				return false;
				}
			return true;
			}

		// write the size of the string, and the actual string values
		const u64 character_count = u64( string_value->size() );
		const u64 values_size = character_count + sizeof( u64 );
		dstream.Write( character_count );
		if( character_count > 0 )
			{
			const i8 *data = (const i8*)string_value->data();
			dstream.Write( data, character_count );
			}

		// make sure all data was written
		const u64 expected_end_pos = string_data_start_pos + values_size;

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
			ISDErrorLog << "End position of data " << end_pos << " does not equal the expected end position which is " << expected_end_pos << ISDErrorLogEnd;
			return false;
			}

		// succeeded
		return true;
		}

	// write indexed array to stream
	template<ValueType VT, class T> bool write_array( MemoryWriteStream &dstream, const char *key, const u8 key_size_in_bytes, const std::vector<T> *items, const std::vector<i32> *index )
		{
		static_assert((VT >= ValueType::VT_Array_Bool) && (VT <= ValueType::VT_Array_UUID), "Invalid type for write_array");
		static_assert(sizeof( type_information<T>::value_type ) <= 0xff, "Invalid value size, cannot exceed 255 bytes");
		static_assert(sizeof( u64 ) >= sizeof( size_t ), "Unsupported size_t, current code requires it to be at most 8 bytes in size, equal to an u64"); // assuming sizeof(u64) >= sizeof(size_t)
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
				const u64 index_count = index->size();
				dstream.Write( index_count );
				dstream.Write( index->data(), index_count );

				index_size = (index_count * sizeof( i32 )) + sizeof( u64 ); // the index values and the value count
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
			ISDErrorLog << "End position of data " << end_pos << " does not equal the expected end position which is " << expected_end_pos << ISDErrorLogEnd;
			return false;
			}

		// succeeded
		return true;
		}


	// specialization of write_array for bool arrays
	template<> bool write_array<ValueType::VT_Array_Bool, bool>( MemoryWriteStream &dstream, const char *key, const u8 key_size_in_bytes, const std::vector<bool> *items, const std::vector<i32> *index )
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
				const u64 index_count = index->size();
				dstream.Write( index_count );
				dstream.Write( index->data(), index_count );

				index_size = (index_count * sizeof( i32 )) + sizeof( u64 ); // the index values and the value count
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
			ISDErrorLog << "End position of data " << end_pos << " does not equal the expected end position which is " << expected_end_pos << ISDErrorLogEnd;
			return false;
			}

		// succeeded
		return true;
		}

	// specialization of write_array for string arrays
	template<> bool write_array<ValueType::VT_Array_String, std::string>( MemoryWriteStream &dstream, const char *key, const u8 key_size_in_bytes, const std::vector<std::string> *items, const std::vector<i32> *index )
		{
		// record start position, we need this in the end block
		const u64 start_pos = dstream.GetPosition();

		// begin a large block
		if( !begin_write_large_block( dstream, ValueType::VT_Array_String, key, key_size_in_bytes ) )
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

			// write the item count and string items
			const u64 items_count = u64( items->size() );
			u64 values_size = sizeof( u64 ) + (sizeof( u64 ) * items_count);
			dstream.Write( items_count );
			if( items_count > 0 )
				{
				for( size_t string_index = 0; string_index < items->size(); ++string_index )
					{
					u64 string_length = (*items)[string_index].size();
					dstream.Write( string_length );
					if( string_length > 0 )
						{
						i8 *p_data = (i8 *)((*items)[string_index].data());
						dstream.Write( p_data, string_length );

						// update size of values
						values_size += string_length;
						}
					}
				}

			// if we have an index, write it 
			u64 index_size = 0;
			if( index != nullptr )
				{
				const u64 index_count = index->size();
				dstream.Write( index_count );
				dstream.Write( index->data(), index_count );

				index_size = (index_count * sizeof( i32 )) + sizeof( u64 ); // the index values and the value count
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
			ISDErrorLog << "End position of data " << end_pos << " does not equal the expected end position which is " << expected_end_pos << ISDErrorLogEnd;
			return false;
			}

		// succeeded
		return true;
		}

	EntityWriter::~EntityWriter()
		{
		if( this->active_subsection )
			{
			ISDErrorLog << "EntityWriter still has an active subsection. You need to close any active subsection before deleting the object" << ISDErrorLogEnd;
			}
		}

	// Build a section. 
	EntityWriter *EntityWriter::BeginSection( const char *key, const u8 key_length )
		{
		if( this->active_subsection )
			{
			ISDErrorLog << "There is already an active subsection." << ISDErrorLogEnd;
			return nullptr;
			}

		this->active_subsection = new EntityWriter( this->dstream );

		if( !begin_write_large_block( this->dstream, ValueType::VT_Subsection, key, key_length ) )
			{
			ISDErrorLog << "begin_write_large_block failed to write header." << ISDErrorLogEnd;
			return nullptr;
			}

		return this->active_subsection;
		}

	bool EntityWriter::EndSection( const EntityWriter *section_writer )
		{
		if( section_writer != this->active_subsection )
			{
			ISDErrorLog << "Invalid parameter section_writer, it does not match the internal value." << ISDErrorLogEnd;
			return false;
			}

		if( !end_write_large_block( this->dstream, this->active_subsection->start_position ) )
			{
			ISDErrorLog << "end_write_large_block failed unexpectedly." << ISDErrorLogEnd;
			return false;
			}

		delete this->active_subsection;
		active_subsection = nullptr;
		return true;
		}

	bool EntityWriter::WriteNullSection( const char *key, const u8 key_length )
		{
		EntityWriter *subsection = this->BeginSection( key, key_length );
		if( !subsection )
			{
			return false;
			}
		return this->EndSection( subsection );
		}

	EntityWriter *EntityWriter::BeginSectionsArray( const char *key, const u8 key_length, const size_t array_size , const std::vector<i32> *index )
		{
		if( this->active_subsection )
			{
			ISDErrorLog << "There is already an active subsection" << ISDErrorLogEnd;
			return nullptr;
			}

		// create a writer for the array
		this->active_subsection = new EntityWriter( this->dstream );

		if( !begin_write_large_block( this->dstream, ValueType::VT_Array_Subsection, key, key_length ) )
			{
			ISDErrorLog << "begin_write_large_block failed to write header." << ISDErrorLogEnd;
			return nullptr;
			}

		const u64 start_pos = dstream.GetPosition();

		// reset the size and write index in the array
		this->active_subsection_index = ~0;
		this->active_subsection_start_pos = 0;

		// special case: nullptr array marked with ~0
		if( array_size == ~0 )
			{
			this->active_subsection_array_size = 0;

			// array is null array, write empty flags item and exit
			const u16 array_flags = 0;
			dstream.Write( array_flags );

			// make sure we are at the correct position
			const u64 expected_end_pos = start_pos + sizeof( u16 );
			if( dstream.GetPosition() != expected_end_pos )
				{
				ISDErrorLog << "unexpectedly could not write to the stream." << ISDErrorLogEnd;
				return nullptr;
				}

			// ok, return
			return this->active_subsection;
			}
		else
			{
			this->active_subsection_array_size = array_size;
			}

		// flags section
		const u16 has_index = (index) ? (0x100) : (0);
		const u16 array_flags = has_index | 0x1; // lowest bit marks that the array is not empty
		dstream.Write( array_flags );

		// write index 
		u64 index_size = 0;
		if( index != nullptr )
			{
			const u64 index_count = index->size();
			dstream.Write( index_count );
			dstream.Write( index->data(), index_count );

			index_size = (index_count * sizeof( i32 )) + sizeof( u64 ); // the index values and the value count
			}

		// write out the size of the array
		this->dstream.Write( u64( this->active_subsection_array_size ) );

		// check the position
		const u64 expected_end_pos = start_pos + sizeof( u16 ) + sizeof( u64 ) + index_size;
		if( dstream.GetPosition() != expected_end_pos )
			{
			ISDErrorLog << "unexpectedly could not write to the stream." << ISDErrorLogEnd;
			return nullptr;
			}
				
		return this->active_subsection;
		}

	bool EntityWriter::BeginSectionInArray( const EntityWriter *sections_array_writer, const size_t section_index )
		{
		if( this->active_subsection != sections_array_writer )
			{
			ISDErrorLog << "Synch error, currently not writing a subsection array" << ISDErrorLogEnd;
			return false;
			}
		if( (this->active_subsection_index+1) != section_index )
			{
			ISDErrorLog << "Synch error, incorrect subsection index" << ISDErrorLogEnd;
			return false;
			}
		if( section_index >= this->active_subsection_array_size )
			{
			ISDErrorLog << "Incorrect subsection index, out of array bounds" << ISDErrorLogEnd;
			return false;
			}

		this->active_subsection_index = section_index;
		this->active_subsection_start_pos = this->dstream.GetPosition();

		// write a temporary subsection size
		dstream.Write( (u64)MAXINT64 );

		return dstream.GetPosition() == (this->active_subsection_start_pos + sizeof( u64 ));
		}

	bool EntityWriter::EndSectionInArray( const EntityWriter *sections_array_writer, const size_t section_index )
		{
		if( this->active_subsection != sections_array_writer || this->active_subsection_index != section_index )
			{
			ISDErrorLog << "Synch error, currently not writing a subsection array, or incorrect section index" << ISDErrorLogEnd;
			return false;
			}

		const u64 end_pos = dstream.GetPosition();
		const u64 block_size = end_pos - this->active_subsection_start_pos - 8; // total block size - ( sizeof( section_size_value )=8 )
		dstream.SetPosition( this->active_subsection_start_pos ); 
		dstream.Write( block_size );
		dstream.SetPosition( end_pos ); // move back the where we were
		return (end_pos > this->active_subsection_start_pos); // only thing we really can check
		}

	bool EntityWriter::EndSectionsArray( const EntityWriter *sections_array_writer )
		{
		if( this->active_subsection != sections_array_writer )
			{
			ISDErrorLog << "Synch error, currently not writing a subsection array" << ISDErrorLogEnd;
			return false;
			}
		if( (this->active_subsection_index+1) != this->active_subsection_array_size )
			{
			ISDErrorLog << "Synch error, the subsection index does not equal the end of the array" << ISDErrorLogEnd;
			return false;
			}

		if( !end_write_large_block( this->dstream, this->active_subsection->start_position ) )
			{
			ISDErrorLog << "end_write_large_block failed unexpectedly." << ISDErrorLogEnd;
			return false;
			}

		// release active subsection writer
		delete this->active_subsection;
		this->active_subsection = nullptr;
		this->active_subsection_index = ~0;
		this->active_subsection_start_pos = 0;
		this->active_subsection_array_size = 0;
		return true;
		}

	bool EntityWriter::WriteNullSectionsArray( const char *key, const u8 key_length )
		{
		EntityWriter *subsection = this->BeginSectionsArray( key, key_length, ~0, nullptr );
		if( !subsection )
			{
			return false;
			}
		return this->EndSectionsArray( subsection );
		}

	};
