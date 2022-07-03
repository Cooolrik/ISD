# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

import CodeGeneratorHelpers as hlp


def ISD_DynamicTypes_h():
	lines = []
	lines.append('// ISD Copyright (c) 2021,2022 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#include "ISD_Types.h"')
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')
	lines.append('    class MemoryReadStream;')
	lines.append('    class MemoryWriteStream;')
	lines.append('    class EntityWriter;')
	lines.append('    class EntityReader;')
	lines.append('    ')
	lines.append('    namespace dynamic_types')
	lines.append('        {')
	lines.append('        // dynamically allocate a data of data type and container combination')
	lines.append('        std::tuple<void*, bool> new_type( data_type_index dataType , container_type_index containerType );')
	lines.append('    ')
	lines.append('        // delete a previously allocated data object')
	lines.append('        // caveat: no type checking is done, so make sure to supply the correct ')
	lines.append('        // type combo to the function.')
	lines.append('        bool delete_type( data_type_index dataType , container_type_index containerType , void *data );')
	lines.append('    ')
	lines.append('        // clear the contents for data object.')
	lines.append('        // caveat: no type checking is done, so make sure to supply the correct ')
	lines.append('        // type combo to the function.')
	lines.append('        bool clear( data_type_index dataType , container_type_index containerType , void *data );')
	lines.append('    ')
	lines.append('        // write the data to an entity writer stream.')
	lines.append('        // caveat: no type checking is done, so make sure to supply the correct ')
	lines.append('        // type combo to the function.')
	lines.append('        bool write( data_type_index dataType , container_type_index containerType , const char *key, const u8 key_length , EntityWriter &writer , const void *data );')
	lines.append('    ')
	lines.append('        // read the data to from an entity reader stream.')
	lines.append('        // caveat: no type checking is done, so make sure to supply the correct ')
	lines.append('        // type combo to the function.')
	lines.append('        bool read( data_type_index dataType , container_type_index containerType , const char *key, const u8 key_length , EntityReader &reader , void *data );')
	lines.append('    ')
	lines.append('        // copy data from src to dest. both data object must be the same type and be allocated.')
	lines.append('        // caveat: no type checking is done, so make sure to supply the correct ')
	lines.append('        // type combo to the function.')
	lines.append('        bool copy( data_type_index dataType , container_type_index containerType , void *dest , const void *src );')
	lines.append('    ')
	lines.append('        // check if two data objects have the same internal data')
	lines.append('        // caveat: no type checking is done, so make sure to supply the correct ')
	lines.append('        // type combo to the function.')
	lines.append('        bool equals( data_type_index dataType , container_type_index containerType , const void *dataA , const void *dataB );')
	lines.append('    ')

	# end of namespaces
	lines.append('        };')
	lines.append('    };')
	hlp.write_lines_to_file("../ISD/ISD_DynamicTypes.h",lines)


# hash table for allocation lookup, must be larger than the number of items to add
class AllocatorHashTable:
	def hash_function( self , data_type , container_type ):
		return ((data_type * self.hash_data_type_mult) + (container_type * self.hash_container_type_mult)) % self.hash_table_size

	def insert_into_table( self , data_type_combo_string , data_type_id , container_type_id ):
		# use hash function to generate a good starting point
		hash_val = self.hash_function( data_type_id , container_type_id )
		# find first empty slot
		while( self.hash_table[hash_val] != None ):
			hash_val = hash_val+1
			if( hash_val >= self.hash_table_size ):
				hash_val = 0
		# fill it
		self.hash_table[hash_val] = data_type_combo_string

	def __init__(self):
		self.hash_table_size = 577 
		self.hash_data_type_mult = 109
		self.hash_container_type_mult = 991
		self.hash_table = [None] * self.hash_table_size

		# fill up hash table 
		for basetype_inx in range(len(hlp.base_types)):
			basetype = hlp.base_types[basetype_inx]
			for variant_inx in range(len(basetype.variants)):
				variant_name = basetype.variants[variant_inx].implementing_type
				variant_id = ( (basetype_inx+1) << 4) + (variant_inx + 1)
				for cont in hlp.container_types:
					self.insert_into_table( f'&_dt_{variant_name}_ct_{cont.implementing_type}_DynamicTypeObject' , variant_id , cont.container_id )


def ISD_DynamicTypes_cpp():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#include "ISD_Types.h"')
	lines.append('#include "ISD_EntityWriter.h"')
	lines.append('#include "ISD_EntityReader.h"')
	lines.append('#include "ISD_DynamicTypes.h"')
	lines.append('#include "ISD_CombinedTypes.h"')
	lines.append('')
	lines.append('#include <glm/gtc/type_ptr.hpp>')
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')
	lines.append('namespace dynamic_types')
	lines.append('    {')
	lines.append('    struct type_combo')
	lines.append('        {')
	lines.append('        data_type_index data_type = {};')
	lines.append('        container_type_index container_type = {};')
	lines.append('        bool operator==(const type_combo& other) const { return other.data_type == this->data_type && other.container_type == this->container_type; }')
	lines.append('        };')
	lines.append('')
	## print all base type combos for the template specializations
	#method_line_list = [ 
	#	'    template <> type_combo get_type_of<{base_type_combo}>() noexcept {{ return {{ data_type_index::dt_{implementing_type} , container_type_index::ct_{container_type} }}; }}' ,
	#	]
	#lines.append('    // template implementations')
	#lines.extend( hlp.generate_lines_for_all_basetype_combos( method_line_list ) )
	#lines.append('')
	
	lines.append('    // dynamic allocation functors for items')
	lines.append('    class _dynamicTypeClass')
	lines.append('        {')
	lines.append('        public:')
	lines.append('            virtual type_combo Type() const = 0;')
	lines.append('            virtual void *New() const = 0;')
	lines.append('            virtual void Delete( void *data ) const = 0;')
	lines.append('            virtual void Clear( void *data ) const = 0;')
	lines.append('            virtual bool Write( const char *key, const u8 key_length , EntityWriter &writer , const void *data ) const = 0;')
	lines.append('            virtual bool Read( const char *key, const u8 key_length , EntityReader &reader , void *data ) const = 0;')
	lines.append('            virtual void Copy( void *dest , const void *src ) const = 0;')
	lines.append('            virtual bool Equals( const void *dataA , const void *dataB ) const = 0;')
	lines.append('        };')

	lines.append('')

	def generate_dynamic_object_function( base_type_name , implementing_type , container_type , item_type , num_items_per_object , base_type_combo ):
		lines = []
		lines.append(f'    // {base_type_combo}' )
		lines.append(f'    static const class _dt_{implementing_type}_ct_{container_type}_DynamicTypeClass : public _dynamicTypeClass' )
		lines.append(f'        {{' )
		lines.append(f'        public:' )
		lines.append(f'            virtual type_combo Type() const {{ return {{ combined_type_information<{base_type_combo}>::type_index , combined_type_information<{base_type_combo}>::container_index }}; }}' )
		lines.append(f'            virtual void *New() const {{ return new {base_type_combo}(); }}' )
		lines.append(f'            virtual void Delete( void *data ) const {{ delete (({base_type_combo}*)(data)); }}' )
		lines.append(f'            virtual void Clear( void *data ) const {{ clear_combined_type(*(({base_type_combo}*)data)); }}' )
		lines.append(f'            virtual bool Write( const char *key, const u8 key_length , EntityWriter &writer , const void *data ) const {{ return writer.Write<{base_type_combo}>( key , key_length , *((const {base_type_combo}*)data) ); }}' )
		lines.append(f'            virtual bool Read( const char *key, const u8 key_length , EntityReader &reader , void *data ) const {{ return reader.Read<{base_type_combo}>( key , key_length , *(({base_type_combo}*)data) ); }}' )
		lines.append(f'            virtual void Copy( void *dest , const void *src ) const {{ *(({base_type_combo}*)dest) = *((const {base_type_combo}*)src); }}' )
		lines.append(f'            virtual bool Equals( const void *dataA , const void *dataB ) const {{ return *((const {base_type_combo}*)dataA) == *((const {base_type_combo}*)dataB); }}' )
		lines.append(f'        }} _dt_{implementing_type}_ct_{container_type}_DynamicTypeObject;' )
		lines.append(f'')
		return lines
	lines.extend( hlp.function_for_all_basetype_combos( generate_dynamic_object_function ))

	# allocate and print hash table
	hash_table = AllocatorHashTable()

	# print it 
	lines.append('    // Hash table with the type allocator objects')
	lines.append(f'    static const _dynamicTypeClass *_dynamicTypeClassHashTable[{hash_table.hash_table_size}] = ')
	lines.append('        {')
	for idx in range(hash_table.hash_table_size):
		if hash_table.hash_table[idx] == None:
			lines.append('        nullptr,')
		else:
			lines.append(f'        {hash_table.hash_table[idx]},')
	lines.append('        };')
	lines.append('')
	lines.append('    // hash table lookup of typeCombo')
	lines.append('    static const _dynamicTypeClass *_findTypeClass( type_combo typeCombo )')
	lines.append('        {')
	lines.append(f'        size_t hashValue = ((((size_t)typeCombo.data_type) * {hash_table.hash_data_type_mult}) + (((size_t)typeCombo.container_type) * {hash_table.hash_container_type_mult})) % {hash_table.hash_table_size};')
	lines.append('        while( _dynamicTypeClassHashTable[hashValue] != nullptr )')
	lines.append('            {')
	lines.append('            type_combo type = _dynamicTypeClassHashTable[hashValue]->Type();')
	lines.append(f'            if( type == typeCombo )')
	lines.append('                return _dynamicTypeClassHashTable[hashValue];')
	lines.append('            ++hashValue;')
	lines.append(f'            if(hashValue >= {hash_table.hash_table_size})')
	lines.append('                hashValue = 0;')
	lines.append('            }')
	lines.append('        ISDErrorLog << "Invalid typeCombo parameter { " << (int)typeCombo.data_type << " , " << (int)typeCombo.container_type << " } " << ISDErrorLogEnd;')
	lines.append('        return nullptr;')
	lines.append('        }')
	lines.append('')
	lines.append('    std::tuple<void*, bool> new_type( data_type_index dataType , container_type_index containerType )')
	lines.append('        {')
	lines.append('        void* data = {};')
	lines.append('        const _dynamicTypeClass *ta = _findTypeClass( { dataType, containerType } );')
	lines.append('        if( ta )')
	lines.append('            data = ta->New();')
	lines.append('        return std::tuple<void*, bool>(data, data != nullptr);')
	lines.append('        }')
	lines.append('')
	lines.append('    bool delete_type( data_type_index dataType , container_type_index containerType , void *data )')
	lines.append('        {')
	lines.append('        if( !data )')
	lines.append('            {')
	lines.append('            ISDErrorLog << "Invalid parameter, data must be a pointer to existing type" << ISDErrorLogEnd;')
	lines.append('            return false;')
	lines.append('            }')
	lines.append('        const _dynamicTypeClass *ta = _findTypeClass( { dataType, containerType } );')
	lines.append('        if( !ta )')
	lines.append('            return false;')
	lines.append('        ta->Delete( data );')
	lines.append('        return true;')
	lines.append('        }')
	lines.append('')
	lines.append('    bool clear( data_type_index dataType , container_type_index containerType , void *data )')
	lines.append('        {')
	lines.append('        if( !data )')
	lines.append('            {')
	lines.append('            ISDErrorLog << "Invalid parameter, data must be a pointer to existing type" << ISDErrorLogEnd;')
	lines.append('            return false;')
	lines.append('            }')
	lines.append('        const _dynamicTypeClass *ta = _findTypeClass( { dataType, containerType } );')
	lines.append('        if( !ta )')
	lines.append('            return false;')
	lines.append('        ta->Clear( data );')
	lines.append('        return true;')
	lines.append('        }')
	lines.append('')
	lines.append('    bool write( data_type_index dataType , container_type_index containerType , const char *key, const u8 key_length , EntityWriter &writer , const void *data )')
	lines.append('        {')
	lines.append('        if( !data )')
	lines.append('            {')
	lines.append('            ISDErrorLog << "Invalid parameter, data must be a pointer to existing type" << ISDErrorLogEnd;')
	lines.append('            return false;')
	lines.append('            }')
	lines.append('        const _dynamicTypeClass *ta = _findTypeClass( { dataType, containerType } );')
	lines.append('        if( !ta )')
	lines.append('            return false;')
	lines.append('        return ta->Write( key , key_length , writer , data );')
	lines.append('        }')
	lines.append('')
	lines.append('    bool read( data_type_index dataType , container_type_index containerType , const char *key, const u8 key_length , EntityReader &reader , void *data )')
	lines.append('        {')
	lines.append('        if( !data )')
	lines.append('            {')
	lines.append('            ISDErrorLog << "Invalid parameter, data must be a pointer to existing type" << ISDErrorLogEnd;')
	lines.append('            return false;')
	lines.append('            }')
	lines.append('        const _dynamicTypeClass *ta = _findTypeClass( { dataType, containerType } );')
	lines.append('        if( !ta )')
	lines.append('            return false;')
	lines.append('        return ta->Read( key , key_length , reader , data );')
	lines.append('        }')
	lines.append('')
	lines.append('    bool copy( data_type_index dataType , container_type_index containerType , void *dest , const void *src )')
	lines.append('        {')
	lines.append('        if( !dest || !src )')
	lines.append('            {')
	lines.append('            ISDErrorLog << "Invalid parameter, dest and src must be pointers to existing types" << ISDErrorLogEnd;')
	lines.append('            return false;')
	lines.append('            }')
	lines.append('        const _dynamicTypeClass *ta = _findTypeClass( { dataType, containerType } );')
	lines.append('        if( !ta )')
	lines.append('            return false;')
	lines.append('        ta->Copy( dest , src );')
	lines.append('        return true;')
	lines.append('        }')
	lines.append('')
	lines.append('    bool equals( data_type_index dataType , container_type_index containerType , const void *dataA , const void *dataB )')
	lines.append('        {')
	lines.append('        if( !dataA || !dataB )')
	lines.append('            {')
	lines.append('            ISDErrorLog << "Invalid parameter, dataA and dataB must be pointers to existing types" << ISDErrorLogEnd;')
	lines.append('            return false;')
	lines.append('            }')
	lines.append('        const _dynamicTypeClass *ta = _findTypeClass( { dataType, containerType } );')
	lines.append('        if( !ta )')
	lines.append('            return false;')
	lines.append('        return ta->Equals( dataA , dataB );')
	lines.append('        }')
	lines.append('')

	# end of namespace
	lines.append('    };')
	lines.append('    };')
	hlp.write_lines_to_file("../ISD/ISD_DynamicTypes.cpp",lines)
	
def DynamicTypesTests_cpp():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('')
	lines.append('#include "pch.h"')
	lines.append('#include "CppUnitTest.h"')
	lines.append('')
	lines.append('using namespace Microsoft::VisualStudio::CppUnitTestFramework;')
	lines.append('')
	lines.append('#include "..\ISD\ISD_DynamicTypes.h"')
	lines.append('#include <glm/glm.hpp>')
	lines.append('#include <glm/ext.hpp>')
	lines.append('')
	lines.append('namespace TypeTests')
	lines.append('    {')
	lines.append('    TEST_CLASS(DynamicTypesTests)')
	lines.append('        {')
	lines.append('        STANDARD_TEST_INIT()')
	lines.append('')

	lines.append('        template<class _Ty> void DynamicValueTester()')
	lines.append('            {')
	lines.append('            constexpr ISD::data_type_index type_index = ISD::data_type_information<_Ty>::type_index;')
	lines.append('            void *dataA = {};')
	lines.append('            void *dataB = {};')
	lines.append('            bool ret = {};')
	lines.append('')

	for cont in hlp.container_types:
		lines.append(f'            // test container type: {cont.implementing_type} ')
		lines.append(f'            constexpr ISD::container_type_index ct_{cont.implementing_type} = container_type_index::ct_{cont.implementing_type};')
		lines.append('')
		lines.append('            // create objects of the type, one dynamically typed in the heap and one statically typed on the stack')
		lines.append(f'            std::tie(dataA,ret) = ISD::dynamic_types::new_type( type_index , ct_{cont.implementing_type} );')
		lines.append('            Assert::IsTrue( ret );')
		if cont.is_template:
			lines.append(f'            ISD::{cont.implementing_type}<_Ty> ct_{cont.implementing_type}_valueB;')
		else:
			lines.append(f'            _Ty ct_{cont.implementing_type}_valueB = ISD::data_type_information<_Ty>::zero;')

		lines.append(f'            dataB = &ct_{cont.implementing_type}_valueB;')
		lines.append('')
		lines.append('            // clear A, make sure they match')
		lines.append(f'            Assert::IsTrue( ISD::dynamic_types::clear( type_index , ct_{cont.implementing_type} , dataA ) );')
		lines.append(f'            Assert::IsTrue( ISD::dynamic_types::equals( type_index , ct_{cont.implementing_type} , dataA, dataB ) );')
		lines.append('')
		lines.append('            // give A a random non-zero value')
		if cont.is_template:
			lines.append(f'            random_nonzero_{cont.implementing_type}<_Ty>( *(({cont.implementing_type}<_Ty>*)dataA) );')
		else:
			lines.append(f'            random_nonzero_value<_Ty>( *((_Ty*)dataA) );')
		lines.append('')
		lines.append('            // try comparing, clearing and copying')
		lines.append(f'            Assert::IsFalse( ISD::dynamic_types::equals( type_index , ct_{cont.implementing_type} , dataA, dataB ) );')
		lines.append(f'            Assert::IsTrue( ISD::dynamic_types::copy( type_index , ct_{cont.implementing_type} , dataB, dataA ) );')
		lines.append(f'            Assert::IsTrue( ISD::dynamic_types::equals( type_index , ct_{cont.implementing_type} , dataA, dataB ) );')
		lines.append(f'            Assert::IsTrue( ISD::dynamic_types::clear( type_index , ct_{cont.implementing_type} , dataA ) );')
		lines.append(f'            Assert::IsFalse( ISD::dynamic_types::equals( type_index , ct_{cont.implementing_type} , dataA, dataB ) );')
		lines.append(f'            Assert::IsTrue( ISD::dynamic_types::copy( type_index , ct_{cont.implementing_type} , dataB, dataA ) );')
		lines.append(f'            Assert::IsTrue( ISD::dynamic_types::equals( type_index , ct_{cont.implementing_type} , dataA, dataB ) );')
		lines.append('')
		lines.append('            // delete the heap allocated data')
		lines.append(f'            Assert::IsTrue( ISD::dynamic_types::delete_type( type_index , ct_{cont.implementing_type} , dataA ) );')
		lines.append('')

	lines.append('            }')

	lines.append('        TEST_METHOD( DynamicTypes )')
	lines.append('            {')

	lines.append('            setup_random_seed();')
	lines.append('            for( uint pass_index=0; pass_index<(2*global_number_of_passes); ++pass_index )')
	lines.append('                {')

	for basetype in hlp.base_types:
		for var in basetype.variants:
			lines.append(f'            DynamicValueTester<{var.implementing_type}>();')

	lines.append('                }')
	lines.append('            }')

	lines.append('        };')
	lines.append('    };')
	hlp.write_lines_to_file("../Tests/DynamicTypesTests.cpp",lines)	
	
def run():
	ISD_DynamicTypes_h()
	ISD_DynamicTypes_cpp()
	DynamicTypesTests_cpp()