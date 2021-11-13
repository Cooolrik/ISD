#include "ISD_EntityReader.h"
#include "ISD_MemoryReadStream.h"
#include "ISD_Log.h"

#include <glm/gtc/type_ptr.hpp>

// value_type: the ValueType enum to read the block as
// object_type: the C++ object that stores the data (can be a basic type), such as uint32, or glm::vec3
// item_type: the actual basic type that stores the data, int the case of glm::vec3, it is a float
// item_count: the count of the the basic type, again in the case of glm::vec3, the count is 3
#define ImplementSmallBlockSingleItemReader( value_type , object_type )\
	bool EntityReader::Read( const char *key, const uint8 key_length, object_type &dest_variable )\
		{\
		return read_small_block<value_type,object_type,1>( this->sstream, key, key_length, &dest_variable );\
		}
#define ImplementSmallBlockVectorReader( value_type , object_type , item_type , item_count )\
	bool EntityReader::Read( const char *key, const uint8 key_length, object_type &dest_variable )\
		{\
		return read_small_block<value_type,item_type,item_count>( this->sstream, key, key_length, value_ptr(dest_variable) );\
		}

namespace ISD
	{
	// template method that Reads a small block of a specific ValueType VT to the stream. Since most value types 
	// can have different bit depths, the second parameter I is the actual type of the data stored. The data can have more than one values of type I, the count is stored in IC.
	template<ValueType VT, class I, size_t IC> bool read_small_block( MemoryReadStream &sstream, const char *key, const uint8 key_length, I *dest_data )
		{
		ISDSanityCheckDebugMacro( key_length <= EntityMaxKeyLength ); // max key length
		ISDSanityCheckCoreDebugMacro( block_size < 256 ); // must fit in a byte
		const size_t expected_size = (sizeof( I ) * IC) + key_length;
		const uint64 start_pos = sstream.GetPosition();
		const uint64 expected_end_pos = start_pos + 2 + expected_size;

		// Read data block 
		const uint8 value_type = sstream.Read<uint8>();
		if( value_type != (uint8)VT )
			{
			ISDErrorLog << "The type in the input stream:" << value_type << " does not match expected type: " << (uint8)VT << ISDErrorLogEnd;
			return false;
			}
		const size_t block_size = sstream.Read<uint8>();
		if( block_size != expected_size )
			{
			ISDErrorLog << "The size of the block in the input stream:" << block_size << " does not match expected size: " << expected_size << ISDErrorLogEnd;
			return false;
			}
		const uint64 read_count = sstream.Read( dest_data , IC );
		if( read_count != IC )
			{
			ISDErrorLog << "Could not read all expected values from the input stream. Expected count: " << IC << " read count: " << read_count << ISDErrorLogEnd;
			return false;
			}
		char read_key[EntityMaxKeyLength];
		const uint64 read_key_length = sstream.Read( (int8*)read_key, (uint64)key_length );
		if( read_key_length != (uint64)key_length 
			|| memcmp(key,read_key,(uint64)key_length)!=0 )
			{
			std::string expected_key_name( key, key_length );
			std::string read_key_name( read_key, key_length );
			ISDErrorLog << "Unexpected key name in the stream. Expected name: " << expected_key_name << " read name: " << read_key_name << ISDErrorLogEnd;
			return false;
			}

		ISDSanityCheckCoreDebugMacro( sstream.GetPosition() == expected_end_pos ); // this should always be true
		return (sstream.GetPosition() == expected_end_pos);			
		};

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

	// special cases:

	// VT_Null - Read no data
	bool EntityReader::ReadNull( const char *key, const uint8 key_length )
		{
		ISDSanityCheckDebugMacro( key_length <= EntityMaxKeyLength ); // max key length
		ISDSanityCheckCoreDebugMacro( block_size < 256 ); // must fit in a byte
		const size_t expected_size = key_length;
		const uint64 start_pos = this->sstream.GetPosition();
		const uint64 expected_end_pos = start_pos + 2 + expected_size;

		// Read data block 
		const uint8 value_type = this->sstream.Read<uint8>();
		if( value_type != (uint8)ValueType::VT_Null )
			{
			ISDErrorLog << "The type in the input stream:" << value_type << " does not match expected type: " << (uint8)ValueType::VT_Null << ISDErrorLogEnd;
			return false;
			}
		const size_t block_size = this->sstream.Read<uint8>();
		if( block_size != expected_size )
			{
			ISDErrorLog << "The size of the block in the input stream:" << block_size << " does not match expected size: " << expected_size << ISDErrorLogEnd;
			return false;
			}
		char read_key[EntityMaxKeyLength];
		const uint64 read_key_length = this->sstream.Read( (int8*)read_key, (uint64)key_length );
		if( read_key_length != (uint64)key_length 
			|| memcmp(key,read_key,(uint64)key_length)!=0 )
			{
			std::string expected_key_name( key, key_length );
			std::string read_key_name( read_key, key_length );
			ISDErrorLog << "Unexpected key name in the stream. Expected name: " << expected_key_name << " read name: " << read_key_name << ISDErrorLogEnd;
			return false;
			}

		ISDSanityCheckCoreDebugMacro( this->sstream.GetPosition() == expected_end_pos ); // this should always be true
		return (this->sstream.GetPosition() == expected_end_pos);			
		};

	// VT_Bool - convert to int8
	bool EntityReader::Read( const char *key, const uint8 key_length, bool &dest_variable )
		{
		return read_small_block<ValueType::VT_Bool,int8,1>( this->sstream, key, key_length, (int8*)&dest_variable );
		}

	};
