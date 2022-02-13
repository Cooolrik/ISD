# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

import CodeGeneratorHelpers as hlp

int_bit_range = [8,16,32,64]
float_type_range = ['float','double']
vector_dimension_range = [2,3,4] 
nonconst_const_range = ['','const ']

def print_UUID_header():
	lines = []
	lines.append('// include GUID stuff from windows')
	lines.append('#ifdef _WIN32')
	lines.append('#define __INLINE_ISEQUAL_GUID')
	lines.append('#include <guiddef.h>')
	lines.append('#endif//_WIN32')
	lines.append('')
	lines.append('// define UUID')
	lines.append('#ifndef UUID_DEFINED')
	lines.append('#define UUID_DEFINED')
	lines.append('typedef GUID UUID;')
	lines.append('#ifdef _WIN32')
	lines.append('#ifndef uuid_t')
	lines.append('#define uuid_t UUID')
	lines.append('#endif//uuid_t')
	lines.append('#endif//_WIN32')
	lines.append('inline bool operator<( const UUID &Left, const UUID &Right ) ')
	lines.append('\t{')
	lines.append('\treturn memcmp( &Left, &Right, sizeof( UUID ) ) < 0;')
	lines.append('\t};')
	lines.append('std::ostream &operator<<( std::ostream &os, const UUID &_uuid );')
	lines.append('#endif//UUID_DEFINED')
	return lines

def print_hash_header():
	lines = []
	lines.append('// define hash for sha256 message digests')
	lines.append('#ifndef HASH_DEFINED')
	lines.append('#define HASH_DEFINED')
	lines.append('struct hash')
	lines.append('\t{')
	lines.append('\tunion')
	lines.append('\t\t{')
	lines.append('\t\tstd::uint64_t _digest_q[4];')
	lines.append('\t\tstd::uint8_t digest[32];')
	lines.append('\t\t};')
	lines.append('\t};')
	lines.append('inline bool operator<( const hash &Left, const hash &Right ) ')
	lines.append('\t{')
	lines.append('\treturn memcmp( &Left, &Right, sizeof( hash ) ) < 0;')
	lines.append('\t};')
	lines.append('inline bool operator==( const hash &Left, const hash &Right ) ')
	lines.append('\t{')
	lines.append('\treturn (Left._digest_q[0] == Right._digest_q[0])')
	lines.append('\t    && (Left._digest_q[1] == Right._digest_q[1])')
	lines.append('\t    && (Left._digest_q[2] == Right._digest_q[2])')
	lines.append('\t    && (Left._digest_q[3] == Right._digest_q[3]);')
	lines.append('\t};')
	lines.append('inline bool operator!=( const hash &Left, const hash &Right ) ')
	lines.append('\t{')
	lines.append('\treturn (Left._digest_q[0] != Right._digest_q[0])')
	lines.append('\t    || (Left._digest_q[1] != Right._digest_q[1])')
	lines.append('\t    || (Left._digest_q[2] != Right._digest_q[2])')
	lines.append('\t    || (Left._digest_q[3] != Right._digest_q[3]);')
	lines.append('\t};')
	lines.append('std::ostream &operator<<( std::ostream &os, const hash &_hash );')
	lines.append('#endif//HASH_DEFINED')
	return lines

def print_type_information_header( type , value , value_count ):
	lines = []
	lines.append(f'\ttemplate<> struct type_information<{type}>')
	lines.append( '\t	{')
	lines.append(f'\t	using value_type = {value}; // the value type of {type} ( {value} )')
	lines.append(f'\t	static constexpr size_t value_count = {value_count}; // the number of values in {type} ( {value_count} )')
	lines.append(f'\t	static constexpr const char * value_name = "{value}"; // name of the value in {type} ( "{value}" ) ')
	lines.append(f'\t	static constexpr const char * type_name = "{type}"; // name of the type ( "{type}" ) ')
	lines.append(f'\t	static const {type} zero; // zero value of {type}')
	lines.append(f'\t	static const {type} inf; // limit inferior (minimum possible value) of {type}')
	if type != 'string':
		lines.append(f'\t	static const {type} sup; // limit superior (maximum possible value) of {type}')
	lines.append( '\t	};')
	lines.append('')
	return lines

def ISD_DataTypes_h():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#ifndef ISD_SKIP_UUID_AND_HASH')
	lines.append('')
	lines.extend(print_UUID_header())
	lines.append('')
	lines.extend(print_hash_header())
	lines.append('')
	lines.append('#endif//ISD_SKIP_UUID_AND_HASH')
	lines.append('')
	lines.append('#include <glm/fwd.hpp>')
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')

	# typedef base integer types
	lines.append(f"\t// scalar types")
	for bit_size in int_bit_range:
		lines.append(f"\ttypedef std::int{bit_size}_t i{bit_size};")
	for bit_size in int_bit_range:
		lines.append(f"\ttypedef std::uint{bit_size}_t u{bit_size};")
	lines.append('')
	lines.append(f"\ttypedef std::string string;")
	lines.append(f"\ttypedef UUID uuid;")
	lines.append(f"\ttypedef ::hash hash;")
	lines.append('')

	# const min/max values of the standard types
	lines.append('\t// scalar types, zero value, minimum possible value ("inf", limit inferior) and maximum possible value ("sup", limit superior)')
	lines.append('\tconstexpr bool bool_zero = false;')
	lines.append('\tconstexpr bool bool_inf = false;')
	lines.append('\tconstexpr bool bool_sup = true;')
	for bit_size in int_bit_range:
		lines.append(f"\tconstexpr i{bit_size} i{bit_size}_zero = 0;")
		lines.append(f"\tconstexpr i{bit_size} i{bit_size}_inf = INT{bit_size}_MIN;")
		lines.append(f"\tconstexpr i{bit_size} i{bit_size}_sup = INT{bit_size}_MAX;")
	for bit_size in int_bit_range:
		lines.append(f"\tconstexpr u{bit_size} u{bit_size}_zero = 0;")
		lines.append(f"\tconstexpr u{bit_size} u{bit_size}_inf = 0;")
		lines.append(f"\tconstexpr u{bit_size} u{bit_size}_sup = UINT{bit_size}_MAX;")
	lines.append('')
	lines.append('\tconstexpr float float_zero = 0.0f;')
	lines.append('\tconstexpr float float_inf = -FLT_MAX;')
	lines.append('\tconstexpr float float_sup = FLT_MAX;')
	lines.append('\tconstexpr float double_zero = 0.0;')
	lines.append('\tconstexpr double double_inf = -DBL_MAX;')
	lines.append('\tconstexpr double double_sup = DBL_MAX;')
	lines.append('')
	lines.append('\tconst string string_zero;')
	lines.append('\tconst string string_inf;')
	lines.append('')
	lines.append('\tconstexpr uuid uuid_zero = {0,0,0,{0,0,0,0,0,0,0,0}};')
	lines.append('\tconstexpr uuid uuid_inf = {0,0,0,{0,0,0,0,0,0,0,0}};')
	lines.append('\tconstexpr uuid uuid_sup = {~0Ui32,~0Ui16,~0Ui16,{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}};')
	lines.append('')
	lines.append('\tconstexpr hash hash_zero = {0,0,0,0};')
	lines.append('\tconstexpr hash hash_inf = {0,0,0,0};')
	lines.append('\tconstexpr hash hash_sup = {~0Ui64,~0Ui64,~0Ui64,~0Ui64};')
	lines.append('')

	# typedef vector types
	lines.append(f"\t// vector types")
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			lines.append(f"\ttypedef glm::vec<{vec_dim},glm::i{bit_size},glm::packed_highp> i{bit_size}vec{vec_dim};")
	lines.append('')
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			lines.append(f"\ttypedef glm::vec<{vec_dim},glm::u{bit_size},glm::packed_highp> u{bit_size}vec{vec_dim};")
	lines.append('')
	for vec_dim in vector_dimension_range:
		lines.append(f"\ttypedef glm::vec<{vec_dim},glm::f32,glm::packed_highp> fvec{vec_dim};")
	for vec_dim in vector_dimension_range:
		lines.append(f"\ttypedef glm::vec<{vec_dim},glm::f64,glm::packed_highp> dvec{vec_dim};")
	lines.append('')
	
	# typedef matrix types
	lines.append(f"\t// matrix types")
	for vec_dim in vector_dimension_range:
		lines.append(f"\ttypedef glm::mat<{vec_dim},{vec_dim},glm::f32,glm::packed_highp> fmat{vec_dim};")
	for vec_dim in vector_dimension_range:
		lines.append(f"\ttypedef glm::mat<{vec_dim},{vec_dim},glm::f64,glm::packed_highp> dmat{vec_dim};")
	lines.append('')

	# typedef quaternions
	lines.append(f"\t// quaternion types")
	lines.append(f"\ttypedef glm::qua<glm::f32,glm::packed_highp> fquat;")
	lines.append(f"\ttypedef glm::qua<glm::f64,glm::packed_highp> dquat;")
	lines.append('')

	# typedef standard types
	for vec_dim in vector_dimension_range:
		lines.append(f"\ttypedef glm::ivec{vec_dim} ivec{vec_dim};")
	for vec_dim in vector_dimension_range:
		lines.append(f"\ttypedef glm::uvec{vec_dim} uvec{vec_dim};")
	for vec_dim in vector_dimension_range:
		lines.append(f"\ttypedef glm::vec{vec_dim} vec{vec_dim};")
	for vec_dim in vector_dimension_range:
		lines.append(f"\ttypedef glm::mat{vec_dim} mat{vec_dim};")
	lines.append('')

	# type information on all types
	lines.append('\t// type_information stores information on the standard types in ISD')
	lines.append('\ttemplate <class T> struct type_information;')
	lines.append('')

	# scalar type info
	lines.extend(print_type_information_header("bool","bool",1))
	for bit_size in int_bit_range:
		lines.extend(print_type_information_header(f"i{bit_size}",f"i{bit_size}",1))
	for bit_size in int_bit_range:
		lines.extend(print_type_information_header(f"u{bit_size}",f"u{bit_size}",1))
	lines.extend(print_type_information_header("float","float",1))
	lines.extend(print_type_information_header("double","double",1))

	# vector type info
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			lines.extend(print_type_information_header(f"i{bit_size}vec{vec_dim}",f"i{bit_size}",vec_dim))
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			lines.extend(print_type_information_header(f"u{bit_size}vec{vec_dim}",f"u{bit_size}",vec_dim))	
	for vec_dim in vector_dimension_range:
		lines.extend(print_type_information_header(f"fvec{vec_dim}",'float',vec_dim))	
	for vec_dim in vector_dimension_range:
		lines.extend(print_type_information_header(f"dvec{vec_dim}",'double',vec_dim))	

	# matrix type info
	for vec_dim in vector_dimension_range:
		lines.extend(print_type_information_header(f"fmat{vec_dim}",'float',vec_dim*vec_dim))	
	for vec_dim in vector_dimension_range:
		lines.extend(print_type_information_header(f"dmat{vec_dim}",'double',vec_dim*vec_dim))	

	# quaternions info
	lines.extend(print_type_information_header('fquat','float',4))
	lines.extend(print_type_information_header('dquat','double',4))

	# uuid info
	lines.extend(print_type_information_header('uuid','uuid',1))

	# hash info
	lines.extend(print_type_information_header('hash','hash',1))

	# string info
	lines.extend(print_type_information_header('string','string',1))

	# { static_assert(false, "Error: type_information template: The value type T does not have type information"); }
	# end of file
	lines.append('    };')
	hlp.write_lines_to_file("../ISD/ISD_DataTypes.h",lines)

def print_UUID_source():
	lines = []
	lines.append('std::ostream &operator<<( std::ostream &os, const UUID &guid )')
	lines.append('    {')
	lines.append('    return os;')
	lines.append('    }')
	lines.append('')
	return lines

def print_type_information_source( type , value , value_count ):
	lines = []
	
	zero = inf = sup = ''
	for i in range(value_count):
		zero += f'{value}_zero'
		inf += f'{value}_inf'
		sup += f'{value}_sup'
		if i < value_count-1:
			zero += ','
			inf += ','
			sup += ','

	lines.append(f'\tconst {type} type_information<{type}>::zero = {type}({zero}); // zero value of {type}')
	lines.append(f'\tconst {type} type_information<{type}>::inf = {type}({inf}); // limit inferior (minimum bound) of {type}')
	if type != 'string':
		lines.append(f'\tconst {type} type_information<{type}>::sup = {type}({sup}); // limit superior (maximum bound) of {type}')
	lines.append('')
	return lines

def ISD_DataTypes_cpp():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#include <glm/glm.hpp>')
	lines.append('#include <glm/gtc/type_ptr.hpp>')
	lines.append('')
	lines.append('#include "ISD_Types.h"')
	lines.append('')
	lines.extend(print_UUID_source())
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')

	# scalar type info
	for bit_size in int_bit_range:
		lines.extend(print_type_information_source(f"i{bit_size}",f"i{bit_size}",1))
	for bit_size in int_bit_range:
		lines.extend(print_type_information_source(f"u{bit_size}",f"u{bit_size}",1))
	lines.extend(print_type_information_source("float","float",1))
	lines.extend(print_type_information_source("double","double",1))

	# vector type info
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			lines.extend(print_type_information_source(f"i{bit_size}vec{vec_dim}",f"i{bit_size}",vec_dim))
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			lines.extend(print_type_information_source(f"u{bit_size}vec{vec_dim}",f"u{bit_size}",vec_dim))	
	for vec_dim in vector_dimension_range:
		lines.extend(print_type_information_source(f"fvec{vec_dim}",'float',vec_dim))	
	for vec_dim in vector_dimension_range:
		lines.extend(print_type_information_source(f"dvec{vec_dim}",'double',vec_dim))	

	# matrix type info
	for vec_dim in vector_dimension_range:
		lines.extend(print_type_information_source(f"fmat{vec_dim}",'float',vec_dim*vec_dim))	
	for vec_dim in vector_dimension_range:
		lines.extend(print_type_information_source(f"dmat{vec_dim}",'double',vec_dim*vec_dim))	

	# quaternions info
	lines.extend(print_type_information_source('fquat','float',4))
	lines.extend(print_type_information_source('dquat','double',4))

	# other types that are atomic
	same_type_range = ['uuid','hash','string']
	for type in same_type_range:
		lines.extend(print_type_information_source(type,type,1))
	lines.append('    };')

	hlp.write_lines_to_file("../ISD/ISD_DataTypes.cpp",lines)

def ISD_DataValuePointers_h():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#include "ISD_Types.h"')
	lines.append('')
	lines.append('#include <glm/gtc/type_ptr.hpp>')
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')

	# type pointer functions (return pointer to first item in each type)
	lines.append(f"\t// item pointer functions, returns a pointer to the first item of each type")
	lines.append('')
	for bit_size in int_bit_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}i{bit_size} *value_ptr( {const_type}i{bit_size} &value ) {{ return &value; }}")
	lines.append('')
	for bit_size in int_bit_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}u{bit_size} *value_ptr( {const_type}u{bit_size} &value ) {{ return &value; }}")
	lines.append('')
	for float_type in float_type_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}{float_type} *value_ptr( {const_type}{float_type} &value ) {{ return &value; }}")
	lines.append('')
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			for const_type in nonconst_const_range:
				lines.append(f"\tinline {const_type}i{bit_size} *value_ptr( {const_type}i{bit_size}vec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			for const_type in nonconst_const_range:
				lines.append(f"\tinline {const_type}u{bit_size} *value_ptr( {const_type}u{bit_size}vec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')

	# vectors
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}float *value_ptr( {const_type}fvec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}double *value_ptr( {const_type}dvec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')

	# matrices
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}float *value_ptr( {const_type}fmat{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}double *value_ptr( {const_type}dmat{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')

	# quaternions
	for const_type in nonconst_const_range:
		lines.append(f"\tinline {const_type}float *value_ptr( {const_type}fquat &value ) {{ return glm::value_ptr(value); }}")
	for const_type in nonconst_const_range:
		lines.append(f"\tinline {const_type}double *value_ptr( {const_type}dquat &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')

	# other types that have no inner item pointer
	same_type_range = ['uuid','hash','string']
	for type in same_type_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}{type} *value_ptr( {const_type}{type} &value ) {{ return &value; }}")

	# end of namespace
	lines.append('    };')
	hlp.write_lines_to_file("../ISD/ISD_DataValuePointers.h",lines)

def run():
	ISD_DataTypes_h()
	ISD_DataTypes_cpp()
	ISD_DataValuePointers_h()