# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

import CodeGeneratorHelpers as hlp

def ISD_EntityWriter_h():
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
	lines.append('    class MemoryWriteStream;')
	lines.append('')
	lines.append('    class EntityWriter')
	lines.append('        {')
	lines.append('        private:')
	lines.append('            MemoryWriteStream &dstream;')
	lines.append('')
	lines.append('        public:')
	lines.append('            EntityWriter( MemoryWriteStream &_dstream ) : dstream( _dstream ) {}')
	lines.append('')
	lines.append('            // The Write function template, specifically implemented below for all supported value types.')
	lines.append('            template <class T> bool Write( const char *key, const u8 key_length, const T &value );')
	lines.append('')

	# print the base types
	for basetype in hlp.base_types:
		type_name = 'VT_' + basetype.name
		lines.append('            // ' + type_name )
		for type_impl in basetype.variants:
			type_impl_name = type_impl.implementing_type
			lines.append('            template <> bool Write<' + type_impl_name + '>( const char *key, const u8 key_length, const ' + type_impl_name + ' &value );')
			lines.append('            template <> bool Write<optional_value<' + type_impl_name + '>>( const char *key, const u8 key_length, const optional_value<' + type_impl_name + '> &value );')
		lines.append('')

	# print the array types
	for basetype in hlp.base_types:
		type_name = 'VT_Array_' + basetype.name
		lines.append('            // ' + type_name )
		for type_impl in basetype.variants:
			type_impl_name = type_impl.implementing_type
			lines.append('            template <> bool Write<std::vector<' + type_impl_name + '>>( const char *key, const u8 key_length, const std::vector<' + type_impl_name + '> &value );')
			lines.append('            template <> bool Write<optional_value<std::vector<' + type_impl_name + '>>>( const char *key, const u8 key_length, const optional_value<std::vector<' + type_impl_name + '>> &value );')
			lines.append('            template <> bool Write<indexed_array<' + type_impl_name + '>>( const char *key, const u8 key_length, const indexed_array<' + type_impl_name + '> &value );')
			lines.append('            template <> bool Write<optional_value<indexed_array<' + type_impl_name + '>>>( const char *key, const u8 key_length, const optional_value<indexed_array<' + type_impl_name + '>> &value );')
		lines.append('')

	lines.append('		};')
	lines.append('')
	lines.append('	// Write function. Specialized for all supported value types.')
	lines.append('	template <class T> bool EntityWriter::Write( const char *key, const u8 key_length, const T &value )')
	lines.append('		{')
	lines.append('		static_assert(false, "Error: EntityWriter::Write template: The value type T cannot be serialized.");')
	lines.append('		}')
	lines.append('	};')
	hlp.write_lines_to_file("../ISD/ISD_EntityWriter.h",lines)

def ISD_EntityWriter_cpp():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#include "ISD_EntityWriter.h"')
	lines.append('#include "ISD_MemoryWriteStream.h"')
	lines.append('')
	lines.append('#include "ISD_EntityWriterTemplates.inl"')
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
			lines.append(f'	template <> bool EntityWriter::Write<{implementing_type}>( const char *key, const u8 key_length, const {implementing_type} &src_variable )')
			lines.append(f'		{{')
			lines.append(f'		return write_small_block<ValueType::{type_name},{implementing_type}>( this->dstream, key, key_length, &src_variable );')
			lines.append(f'		}}')
			lines.append(f'')
			
			lines.append(f'	// {type_name}: optional_value<{implementing_type}>' )
			lines.append(f'	template <> bool EntityWriter::Write<optional_value<{implementing_type}>>( const char *key, const u8 key_length, const optional_value<{implementing_type}> &src_variable )')
			lines.append(f'		{{')
			lines.append(f'		const {implementing_type} *p_src_variable = (src_variable.has_value()) ? &(src_variable.value()) : nullptr;')
			lines.append(f'		return write_small_block<ValueType::{type_name},{implementing_type}>( this->dstream, key, key_length, p_src_variable );')
			lines.append(f'		}}')
			lines.append(f'')
			
			lines.append(f'	//  {array_type_name}: std::vector<{implementing_type}>' )
			lines.append(f'	template <> bool EntityWriter::Write<std::vector<{implementing_type}>>( const char *key, const u8 key_length, const std::vector<{implementing_type}> &src_variable )')
			lines.append(f'		{{')
			lines.append(f'		return write_array<ValueType::{array_type_name},{implementing_type}>(this->dstream, key, key_length, &src_variable , nullptr );')
			lines.append(f'		}}')
			lines.append(f'')
			
			lines.append(f'	//  {array_type_name}: optional_value<std::vector<{implementing_type}>>' )
			lines.append(f'	template <> bool EntityWriter::Write<optional_value<std::vector<{implementing_type}>>>( const char *key, const u8 key_length, const optional_value<std::vector<{implementing_type}>> &src_variable )')
			lines.append(f'		{{')
			lines.append(f'		const std::vector<{implementing_type}> *p_src_variable = (src_variable.has_value()) ? &(src_variable.value()) : nullptr;')
			lines.append(f'		return write_array<ValueType::{array_type_name},{implementing_type}>(this->dstream, key, key_length, p_src_variable , nullptr );')
			lines.append(f'		}}')
			lines.append(f'')

			lines.append(f'	//  {array_type_name}: indexed_array<{implementing_type}>' )
			lines.append(f'	template <> bool EntityWriter::Write<indexed_array<{implementing_type}>>( const char *key, const u8 key_length, const indexed_array<{implementing_type}> &src_variable )')
			lines.append(f'		{{')
			lines.append(f'		return write_array<ValueType::{array_type_name},{implementing_type}>(this->dstream, key, key_length, &(src_variable.values()) , &(src_variable.index()) );')
			lines.append(f'		}}')
			lines.append(f'')
			
			lines.append(f'	//  {array_type_name}: optional_value<indexed_array<{implementing_type}>>' )
			lines.append(f'	template <> bool EntityWriter::Write<optional_value<indexed_array<{implementing_type}>>>( const char *key, const u8 key_length, const optional_value<indexed_array<{implementing_type}>> &src_variable )')
			lines.append(f'		{{')
			lines.append(f'		const std::vector<{implementing_type}> *p_src_values = (src_variable.has_value()) ? &(src_variable.value().values()) : nullptr;')
			lines.append(f'		const std::vector<size_t> *p_src_index = (src_variable.has_value()) ? &(src_variable.value().index()) : nullptr;')
			lines.append(f'		return write_array<ValueType::{array_type_name},{implementing_type}>(this->dstream, key, key_length, p_src_values , p_src_index );')
			lines.append(f'		}}')
			lines.append(f'')
				
	lines.append('	};')
	hlp.write_lines_to_file("../ISD/ISD_EntityWriter.cpp",lines)

def run():
	ISD_EntityWriter_h()
	ISD_EntityWriter_cpp()