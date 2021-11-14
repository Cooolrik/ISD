#include "ISD_EntityWriter.h"
#include "ISD_MemoryWriteStream.h"
#include "ISD_Log.h"

#include <glm/gtc/type_ptr.hpp>

// value_type: the ValueType enum to store the block as
// object_type: the C++ object that stores the data (can be a basic type), such as uint32, or glm::vec3
// item_type: the actual basic type that stores the data, int the case of glm::vec3, it is a float
// item_count: the count of the the basic type, again in the case of glm::vec3, the count is 3
#define ImplementSmallBlockSingleItemWriter( value_type , object_type )\
	bool EntityWriter::Write( const char *key, const uint8 key_length, const object_type &src_variable )\
		{\
		return write_small_block<value_type,object_type,1>( this->dstream, key, key_length, &src_variable );\
		}\
	bool EntityWriter::Write( const char *key, const uint8 key_length, const optional_value<object_type> &src_variable )\
		{\
		const auto val = src_variable.Value();\
		return write_small_block<value_type,object_type,1>( this->dstream, key, key_length, val.first ? &val.second : nullptr );\
		}
#define ImplementSmallBlockVectorWriter( value_type , object_type , item_type , item_count )\
	bool EntityWriter::Write( const char *key, const uint8 key_length, const object_type &src_variable )\
		{\
		return write_small_block<value_type,item_type,item_count>( this->dstream, key, key_length, value_ptr(src_variable) );\
		}\
	bool EntityWriter::Write( const char *key, const uint8 key_length, const optional_value<object_type> &src_variable )\
		{\
		const auto val = src_variable.Value();\
		return write_small_block<value_type,item_type,item_count>( this->dstream, key, key_length, val.first ? value_ptr(val.second) : nullptr );\
		}

namespace ISD
	{
	// template method that writes a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<ValueType VT, class I, size_t IC> bool write_small_block( MemoryWriteStream &dstream, const char *key, const uint8 key_length, const I *data )
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
			dstream.Write( data , IC );
			}
		dstream.Write( (int8*)key, key_length );

		ISDSanityCheckCoreDebugMacro( dstream.GetPosition() == expected_end_pos ); // this should always be true
		return (dstream.GetPosition() == expected_end_pos);			
		};

	// VT_Bool
	bool EntityWriter::Write( const char *key, const uint8 key_length, const bool &value )
		{
		uint8 u8value = value; // convert it to uint8, since we need a u8 value, not bool
		return write_small_block<ValueType::VT_Bool,uint8,1>( this->dstream, key, key_length, &u8value );
		}
	bool EntityWriter::Write( const char *key, const uint8 key_length, const optional_value<bool> &value )
		{
		const auto val = value.Value(); // get the optional value
		const uint8 u8val = val.second; // convert it to uint8 (even if it does not exits, as it is optional), since we need a u8 value, not bool
		return write_small_block<ValueType::VT_Bool,uint8,1>( this->dstream, key, key_length, val.first ? &u8val : nullptr );
		}

	// VT_Int				 
	ImplementSmallBlockSingleItemWriter( ValueType::VT_Int, int8 )
	ImplementSmallBlockSingleItemWriter( ValueType::VT_Int, int16 )
	ImplementSmallBlockSingleItemWriter( ValueType::VT_Int, int32 )
	ImplementSmallBlockSingleItemWriter( ValueType::VT_Int, int64 )

	// VT_UInt				 
	ImplementSmallBlockSingleItemWriter( ValueType::VT_UInt, uint8 )
	ImplementSmallBlockSingleItemWriter( ValueType::VT_UInt, uint16 )
	ImplementSmallBlockSingleItemWriter( ValueType::VT_UInt, uint32 )
	ImplementSmallBlockSingleItemWriter( ValueType::VT_UInt, uint64 )

	// VT_Float				
	ImplementSmallBlockSingleItemWriter( ValueType::VT_Float, float )
	ImplementSmallBlockSingleItemWriter( ValueType::VT_Float, double )

	// VT_Vec2			
	ImplementSmallBlockVectorWriter( ValueType::VT_Vec2 , fvec2 , float , 2 )
	ImplementSmallBlockVectorWriter( ValueType::VT_Vec2 , dvec2 , double , 2 )

	// VT_Vec3			
	ImplementSmallBlockVectorWriter( ValueType::VT_Vec3 , fvec3 , float , 3 )
	ImplementSmallBlockVectorWriter( ValueType::VT_Vec3 , dvec3 , double , 3 )

	// VT_Vec4			
	ImplementSmallBlockVectorWriter( ValueType::VT_Vec4 , fvec4 , float , 4 )
	ImplementSmallBlockVectorWriter( ValueType::VT_Vec4 , dvec4 , double , 4 )

	// VT_IVec2			
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec2 , i8vec2 , int8 , 2 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec2 , i16vec2 , int16 , 2 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec2 , i32vec2 , int32 , 2 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec2 , i64vec2 , int64 , 2 )

	// VT_IVec3		
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec3 , i8vec3 , int8 , 3 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec3 , i16vec3 , int16 , 3 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec3 , i32vec3 , int32 , 3 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec3 , i64vec3 , int64 , 3 )

	// VT_IVec4
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec4 , i8vec4 , int8 , 4 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec4 , i16vec4 , int16 , 4 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec4 , i32vec4 , int32 , 4 )
	ImplementSmallBlockVectorWriter( ValueType::VT_IVec4 , i64vec4 , int64 , 4 )

	// VT_UVec2			
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec2 , u8vec2 , uint8 , 2 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec2 , u16vec2 , uint16 , 2 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec2 , u32vec2 , uint32 , 2 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec2 , u64vec2 , uint64 , 2 )
												   		   
	// VT_UVec3									   		   
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec3 , u8vec3 , uint8 , 3 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec3 , u16vec3 , uint16 , 3 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec3 , u32vec3 , uint32 , 3 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec3 , u64vec3 , uint64 , 3 )
												   		   
	// VT_UVec4									   		   
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec4 , u8vec4 , uint8 , 4 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec4 , u16vec4 , uint16 , 4 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec4 , u32vec4 , uint32 , 4 )
	ImplementSmallBlockVectorWriter( ValueType::VT_UVec4 , u64vec4 , uint64 , 4 )

	// VT_Mat2				  
	ImplementSmallBlockVectorWriter( ValueType::VT_Mat2 , fmat2 , float , 4 )
	ImplementSmallBlockVectorWriter( ValueType::VT_Mat2 , dmat2 , double , 4 )

	// VT_Mat3				   
	ImplementSmallBlockVectorWriter( ValueType::VT_Mat3 , fmat3 , float , 9 )
	ImplementSmallBlockVectorWriter( ValueType::VT_Mat3 , dmat3 , double , 9 )

	// VT_Mat4				   
	ImplementSmallBlockVectorWriter( ValueType::VT_Mat4 , fmat4 , float , 16 )
	ImplementSmallBlockVectorWriter( ValueType::VT_Mat4 , dmat4 , double , 16 )

	// VT_UUID 
	ImplementSmallBlockSingleItemWriter( ValueType::VT_UUID, UUID )


	};
