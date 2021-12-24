# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

import CodeGeneratorHelpers as hlp

def ISD_EntityReader_h():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#include "ISD_Types.h"')
	lines.append('#include "ISD_Entity.h"')
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')
	lines.append('    class MemoryReadStream;')
	lines.append('')
	lines.append('    class EntityReader')
	lines.append('        {')
	lines.append('        private:')
	lines.append('            MemoryReadStream &sstream;')
	lines.append('')
	lines.append('        public:')
	lines.append('            EntityReader( MemoryReadStream &_sstream ) : sstream( _sstream ) {}')
	lines.append('')
	lines.append('            // The Read function template, specifically implemented below for all supported value types.')
	lines.append('            template <class T> bool Read( const char *key, const u8 key_length, T &value );')
	lines.append('')

	# print the base types
	for basetype in hlp.base_types:
		type_name = 'VT_' + basetype.name
		lines.append('            // ' + type_name )
		for type_impl in basetype.variants:
			type_impl_name = type_impl.implementing_type
			lines.append('            template <> bool Read<' + type_impl_name + '>( const char *key, const u8 key_length, ' + type_impl_name + ' &value );')
			lines.append('            template <> bool Read<optional_value<' + type_impl_name + '>>( const char *key, const u8 key_length, optional_value<' + type_impl_name + '> &value );')
		lines.append('')

	# print the array types
	for basetype in hlp.base_types:
		type_name = 'VT_Array_' + basetype.name
		lines.append('            // ' + type_name )
		for type_impl in basetype.variants:
			type_impl_name = type_impl.implementing_type
			lines.append('            template <> bool Read<std::vector<' + type_impl_name + '>>( const char *key, const u8 key_length, std::vector<' + type_impl_name + '> &value );')
			lines.append('            template <> bool Read<optional_vector<' + type_impl_name + '>>( const char *key, const u8 key_length, optional_vector<' + type_impl_name + '> &value );')
			lines.append('            template <> bool Read<idx_vector<' + type_impl_name + '>>( const char *key, const u8 key_length, idx_vector<' + type_impl_name + '> &value );')
			lines.append('            template <> bool Read<optional_idx_vector<' + type_impl_name + '>>( const char *key, const u8 key_length, optional_idx_vector<' + type_impl_name + '> &value );')
		lines.append('')

	lines.append('		};')
	lines.append('')
	lines.append('	// Read method. Specialized for all supported value types.')
	lines.append('	template <class T> bool EntityReader::Read( const char *key, const u8 key_length, T &value )')
	lines.append('		{')
	lines.append('		static_assert(false, "Error: EntityReader::Read template: The value type T cannot be serialized.");')
	lines.append('		}')
	lines.append('	};')
	hlp.write_lines_to_file("../ISD/ISD_EntityReader.h",lines)

def ISD_EntityReader_cpp():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#include "ISD_EntityReader.h"')
	lines.append('#include "ISD_MemoryReadStream.h"')
	lines.append('')
	lines.append('#include "ISD_EntityReaderTemplates.inl"')
	lines.append('')
	lines.append('namespace ISD')
	lines.append('	{')
	lines.append('')

	# print the base types
	for basetype in hlp.base_types:
		type_name = 'VT_' + basetype.name
		array_type_name = 'VT_Array_' + basetype.name

		# all other cases
		for type_impl in basetype.variants:
			implementing_type = str(type_impl.implementing_type)
			item_type = str(type_impl.item_type)
			num_items_per_object = str(type_impl.num_items_per_object)
			lines.append(f'	// {type_name}: {implementing_type}')
			lines.append(f'	template <> bool EntityReader::Read<{implementing_type}>( const char *key, const u8 key_length, {implementing_type} &dest_variable )')
			lines.append(f'		{{')
			lines.append(f'		reader_status status = read_single_item<ValueType::{type_name},{implementing_type}>(this->sstream, key, key_length, false, &(dest_variable) );')
			lines.append(f'		return status != reader_status::fail;')
			lines.append(f'		}}')
			lines.append(f'')
			lines.append(f'	// {type_name}: optional_value<{implementing_type}>' )
			lines.append(f'	template <> bool EntityReader::Read<optional_value<{implementing_type}>>( const char *key, const u8 key_length, optional_value<{implementing_type}> &dest_variable )')
			lines.append(f'		{{')
			lines.append(f'		dest_variable.set();')
			lines.append(f'		reader_status status = read_single_item<ValueType::{type_name},{implementing_type}>(this->sstream, key, key_length, true, &(dest_variable.value()) );')
			lines.append(f'		if( status == reader_status::success_empty )')
			lines.append(f'			dest_variable.clear();')
			lines.append(f'		return status != reader_status::fail;')
			lines.append(f'		}}')
			lines.append(f'')
			lines.append(f'	// {type_name}: std::vector<{implementing_type}>' )
			lines.append(f'	template <> bool EntityReader::Read<std::vector<{implementing_type}>>( const char *key, const u8 key_length, std::vector<{implementing_type}> &dest_variable )')
			lines.append(f'		{{')
			lines.append(f'		reader_status status = read_array<ValueType::{array_type_name},{implementing_type}>(this->sstream, key, key_length, false, &(dest_variable), nullptr );')
			lines.append(f'		return status != reader_status::fail;')
			lines.append(f'		}}')
			lines.append(f'')
			lines.append(f'	// {type_name}: optional_vector<{implementing_type}>' )
			lines.append(f'	template <> bool EntityReader::Read<optional_vector<{implementing_type}>>( const char *key, const u8 key_length, optional_vector<{implementing_type}> &dest_variable )')
			lines.append(f'		{{')
			lines.append(f'		dest_variable.set();')
			lines.append(f'		reader_status status = read_array<ValueType::{array_type_name},{implementing_type}>(this->sstream, key, key_length, true, &(dest_variable.values()), nullptr );')
			lines.append(f'		if( status == reader_status::success_empty )')
			lines.append(f'			dest_variable.clear();')
			lines.append(f'		return status != reader_status::fail;')
			lines.append(f'		}}')
			lines.append(f'')
			lines.append(f'	// {type_name}: idx_vector<{implementing_type}>' )
			lines.append(f'	template <> bool EntityReader::Read<idx_vector<{implementing_type}>>( const char *key, const u8 key_length, idx_vector<{implementing_type}> &dest_variable )')
			lines.append(f'		{{')
			lines.append(f'		reader_status status = read_array<ValueType::{array_type_name},{implementing_type}>(this->sstream, key, key_length, false, &(dest_variable.values()), &(dest_variable.index()) );')
			lines.append(f'		return status != reader_status::fail;')
			lines.append(f'		}}')
			lines.append(f'')
			lines.append(f'	// {type_name}: optional_idx_vector<{implementing_type}>' )
			lines.append(f'	template <> bool EntityReader::Read<optional_idx_vector<{implementing_type}>>( const char *key, const u8 key_length, optional_idx_vector<{implementing_type}> &dest_variable )')
			lines.append(f'		{{')
			lines.append(f'		dest_variable.set();')
			lines.append(f'		reader_status status = read_array<ValueType::{array_type_name},{implementing_type}>(this->sstream, key, key_length, true, &(dest_variable.values()), &(dest_variable.index()) );')
			lines.append(f'		if( status == reader_status::success_empty )')
			lines.append(f'			dest_variable.clear();')
			lines.append(f'		return status != reader_status::fail;')
			lines.append(f'		}}')
			lines.append(f'')
				
	lines.append('	};')
	hlp.write_lines_to_file("../ISD/ISD_EntityReader.cpp",lines)

def run():
	ISD_EntityReader_h()
	ISD_EntityReader_cpp()