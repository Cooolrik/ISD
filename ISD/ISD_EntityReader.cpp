#include "ISD_EntityReader.h"
#include "ISD_MemoryReadStream.h"
#include "ISD_Log.h"

#include <glm/gtc/type_ptr.hpp>

// value_type: the ValueType enum to read the block as
// object_type: the C++ object that stores the data (can be a basic type), such as uint32, or glm::vec3
// item_type: the actual basic type that stores the data, int the case of glm::vec3, it is a float
// item_count: the count of the the basic type, again in the case of glm::vec3, the count is 3
#define ImplementSmallBlockSingleItemReader( value_type , object_type )\
	bool EntityReader::Read( const char *key, const uint8 key_size_in_bytes, object_type &dest_variable )\
		{\
		return read_small_block<value_type,object_type,1,false>( this->sstream, key, key_size_in_bytes, &dest_variable ) != reader_status::fail;\
		}\
	bool EntityReader::Read( const char *key, const uint8 key_size_in_bytes, optional_value<object_type> &dest_variable )\
		{\
		object_type temp_dest = {};\
		reader_status status = read_small_block<value_type,object_type,1,true>( this->sstream, key, key_size_in_bytes, &temp_dest );\
		if( status == reader_status::fail ) { return false; }\
		else if( status == reader_status::success_empty ) { dest_variable.Clear(); return true; }\
		else { dest_variable.Set(temp_dest); return true;}\
		}
#define ImplementSmallBlockVectorReader( value_type , object_type , item_type , item_count )\
	bool EntityReader::Read( const char *key, const uint8 key_size_in_bytes, object_type &dest_variable )\
		{\
		return read_small_block<value_type,item_type,item_count,false>( this->sstream, key, key_size_in_bytes, value_ptr(dest_variable) ) != reader_status::fail;\
		}\
	bool EntityReader::Read( const char *key, const uint8 key_size_in_bytes, optional_value<object_type> &dest_variable )\
		{\
		object_type temp_dest = {};\
		reader_status status = read_small_block<value_type,item_type,item_count,true>( this->sstream, key, key_size_in_bytes, value_ptr(temp_dest) );\
		if( status == reader_status::fail ) { return false; }\
		else if( status == reader_status::success_empty ) { dest_variable.Clear(); return true; }\
		else { dest_variable.Set(temp_dest); return true; }\
		}

namespace ISD
	{
	enum reader_status
		{
		fail, // general fail, (or value was empty, but empty is not allowed for this value)
		success_empty, // success, value was empty
		success // success, has value
		};

	// template method that Reads a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<ValueType VT, class I, size_t IC, bool empty_value_is_allowed> reader_status read_small_block( MemoryReadStream &sstream, const char *key, const uint8 key_size_in_bytes, I *dest_data )
		{
		// record start position, for validation
		const uint64 start_pos = sstream.GetPosition();

		// read uint8 value_type, check the value
		const uint8 value_type = sstream.Read<uint8>();
		if( value_type != (uint8)VT )
			{
			// not the expected type
			ISDErrorLog << "The type in the input stream:" << value_type << " does not match expected type: " << (uint8)VT << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// calc the expected possible sizes. if empty value, the data size must be 0, else it is the expected size based on the item type (I) and count (IC)
		const uint64 dest_data_size_in_bytes = sizeof( I ) * IC;
		const uint64 expected_block_size_if_empty = key_size_in_bytes;
		const uint64 expected_block_size = dest_data_size_in_bytes + expected_block_size_if_empty;

		ISDSanityCheckCoreDebugMacro( key_size_in_bytes <= EntityMaxKeyLength ); 
		ISDSanityCheckCoreDebugMacro( expected_block_size < 256 ); // must fit in a byte

		// read in size of the small block, if the size does not match the expected block size, check if empty value is ok (is_optional_value == true), and if not raise error
		// any size other than expected_block_size is regarded as empty, and we will check that size if empty is actually allowed
		const uint64 block_size = sstream.Read<uint8>();
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
			const uint64 read_count = sstream.Read( dest_data , IC );
			if( read_count != IC )
				{
				ISDErrorLog << "Could not read all expected values from the input stream. Expected count: " << IC << " read count: " << read_count << ISDErrorLogEnd;
				return reader_status::fail;
				}
			}

		// read in the key data
		char read_key[EntityMaxKeyLength];
		const uint64 read_key_length = sstream.Read( (int8*)read_key, (uint64)key_size_in_bytes );
		if( read_key_length != (uint64)key_size_in_bytes 
			|| memcmp(key,read_key,(uint64)key_size_in_bytes)!=0 )
			{
			std::string expected_key_name( key, key_size_in_bytes );
			std::string read_key_name( read_key, key_size_in_bytes );
			ISDErrorLog << "Unexpected key name in the stream. Expected name: " << expected_key_name << " read name: " << read_key_name << ISDErrorLogEnd;
			return reader_status::fail;;
			}

		// get the position beyond the end of the block, and validate position
		const uint64 expected_end_pos = (is_empty_value) ? (start_pos + 2 + expected_block_size_if_empty) : (start_pos + 2 + expected_block_size);
		const uint64 end_pos = sstream.GetPosition();
		ISDSanityCheckCoreDebugMacro( end_pos == expected_end_pos ); 
		if( end_pos != expected_end_pos )
			{
			ISDErrorLog << "Invaild position in the read stream. Expected position: " << expected_end_pos << " current position: " << end_pos << ISDErrorLogEnd;
			return reader_status::fail;
			}

		// return success, either with value or empty
		if( is_empty_value )
			return reader_status::success_empty;
		else
			return reader_status::success;
		};

	// VT_Bool 
	bool EntityReader::Read( const char *key, const uint8 key_length, bool &dest_variable )
		{
		return read_small_block<ValueType::VT_Bool,int8,1,false>( this->sstream, key, key_length, (int8*)&dest_variable ) != reader_status::fail;
		}
	bool EntityReader::Read( const char *key, const uint8 key_length, optional_value<bool> &dest_variable )
		{
		int8 temp_dest;
		reader_status status = read_small_block<ValueType::VT_Bool,int8,1,true>( this->sstream, key, key_length, &temp_dest );
		if( status == reader_status::fail ) { return false; }
		else if( status == reader_status::success_empty ) { dest_variable.Clear(); return true; }
		else { dest_variable.Set(temp_dest); return true;}
		}

	// VT_Int				 
	ImplementSmallBlockSingleItemReader( ValueType::VT_Int, int8 )
	ImplementSmallBlockSingleItemReader( ValueType::VT_Int, int16 )
	ImplementSmallBlockSingleItemReader( ValueType::VT_Int, int32 )
	ImplementSmallBlockSingleItemReader( ValueType::VT_Int, int64 )

	// VT_UInt				 
	ImplementSmallBlockSingleItemReader( ValueType::VT_UInt, uint8 )
	ImplementSmallBlockSingleItemReader( ValueType::VT_UInt, uint16 )
	ImplementSmallBlockSingleItemReader( ValueType::VT_UInt, uint32 )
	ImplementSmallBlockSingleItemReader( ValueType::VT_UInt, uint64 )

	// VT_Float				
	ImplementSmallBlockSingleItemReader( ValueType::VT_Float, float )
	ImplementSmallBlockSingleItemReader( ValueType::VT_Float, double )

	// VT_Vec2			
	ImplementSmallBlockVectorReader( ValueType::VT_Vec2 , fvec2 , float , 2 )
	ImplementSmallBlockVectorReader( ValueType::VT_Vec2 , dvec2 , double , 2 )

	// VT_Vec3			
	ImplementSmallBlockVectorReader( ValueType::VT_Vec3 , fvec3 , float , 3 )
	ImplementSmallBlockVectorReader( ValueType::VT_Vec3 , dvec3 , double , 3 )

	// VT_Vec4			
	ImplementSmallBlockVectorReader( ValueType::VT_Vec4 , fvec4 , float , 4 )
	ImplementSmallBlockVectorReader( ValueType::VT_Vec4 , dvec4 , double , 4 )

	// VT_IVec2			
	ImplementSmallBlockVectorReader( ValueType::VT_IVec2 , i8vec2 , int8 , 2 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec2 , i16vec2 , int16 , 2 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec2 , i32vec2 , int32 , 2 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec2 , i64vec2 , int64 , 2 )

	// VT_IVec3		
	ImplementSmallBlockVectorReader( ValueType::VT_IVec3 , i8vec3 , int8 , 3 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec3 , i16vec3 , int16 , 3 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec3 , i32vec3 , int32 , 3 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec3 , i64vec3 , int64 , 3 )

	// VT_IVec4
	ImplementSmallBlockVectorReader( ValueType::VT_IVec4 , i8vec4 , int8 , 4 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec4 , i16vec4 , int16 , 4 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec4 , i32vec4 , int32 , 4 )
	ImplementSmallBlockVectorReader( ValueType::VT_IVec4 , i64vec4 , int64 , 4 )

	// VT_UVec2			
	ImplementSmallBlockVectorReader( ValueType::VT_UVec2 , u8vec2 , uint8 , 2 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec2 , u16vec2 , uint16 , 2 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec2 , u32vec2 , uint32 , 2 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec2 , u64vec2 , uint64 , 2 )
												   		   
	// VT_UVec3									   		   
	ImplementSmallBlockVectorReader( ValueType::VT_UVec3 , u8vec3 , uint8 , 3 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec3 , u16vec3 , uint16 , 3 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec3 , u32vec3 , uint32 , 3 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec3 , u64vec3 , uint64 , 3 )
												   		   
	// VT_UVec4									   		   
	ImplementSmallBlockVectorReader( ValueType::VT_UVec4 , u8vec4 , uint8 , 4 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec4 , u16vec4 , uint16 , 4 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec4 , u32vec4 , uint32 , 4 )
	ImplementSmallBlockVectorReader( ValueType::VT_UVec4 , u64vec4 , uint64 , 4 )

	// VT_Mat2				  
	ImplementSmallBlockVectorReader( ValueType::VT_Mat2 , fmat2 , float , 4 )
	ImplementSmallBlockVectorReader( ValueType::VT_Mat2 , dmat2 , double , 4 )

	// VT_Mat3				   
	ImplementSmallBlockVectorReader( ValueType::VT_Mat3 , fmat3 , float , 9 )
	ImplementSmallBlockVectorReader( ValueType::VT_Mat3 , dmat3 , double , 9 )

	// VT_Mat4				   
	ImplementSmallBlockVectorReader( ValueType::VT_Mat4 , fmat4 , float , 16 )
	ImplementSmallBlockVectorReader( ValueType::VT_Mat4 , dmat4 , double , 16 )

	// VT_UUID 
	ImplementSmallBlockSingleItemReader( ValueType::VT_UUID, UUID )

	};
