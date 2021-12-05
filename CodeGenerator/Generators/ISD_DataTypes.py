# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

import CodeGeneratorHelpers as hlp

int_bit_range = [8,16,32,64]
float_type_range = ['float','double']
vector_dimension_range = [2,3,4] 
nonconst_const_range = ['','const ']

def print_type_information_header( type , value , value_count ):
	lines = []
	lines.append(f'\ttemplate<> struct type_information<{type}>')
	lines.append( '\t	{')
	lines.append(f'\t	typedef {value} value_type; // the value type of {type} ( {value} )')
	lines.append(f'\t	static const size_t value_count = {value_count}; // the number of values in {type} ( {value_count} )')
	lines.append(f'\t	static const char *value_name; // name of the value in {type} ( "{value}" ) ')
	lines.append(f'\t	static const char *type_name; // name of the type ( "{type}" ) ')
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

	# UUID info
	lines.extend(print_type_information_header('UUID','UUID',1))

	# { static_assert(false, "Error: type_information template: The value type T does not have type information"); }
	# end of file
	lines.append('    };')
	hlp.write_lines_to_file("../ISD/ISD_DataTypes.h",lines)

def print_type_information_source( type , value , value_count ):
	lines = []
	lines.append(f'\tconst char *type_information<{type}>::value_name = "{value}";')
	lines.append(f'\tconst char *type_information<{type}>::type_name = "{type}";')
	lines.append('')
	return lines

def ISD_DataTypes_cpp():
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#include "ISD_Types.h"')
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

	# UUID info
	lines.extend(print_type_information_source('UUID','UUID',1))
	
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
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}float *value_ptr( {const_type}fvec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}double *value_ptr( {const_type}dvec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}float *value_ptr( {const_type}fmat{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}double *value_ptr( {const_type}dmat{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for const_type in nonconst_const_range:
		lines.append(f"\tinline {const_type}float *value_ptr( {const_type}fquat &value ) {{ return glm::value_ptr(value); }}")
	for const_type in nonconst_const_range:
		lines.append(f"\tinline {const_type}double *value_ptr( {const_type}dquat &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for const_type in nonconst_const_range:
		lines.append(f"\tinline {const_type}UUID *value_ptr( {const_type}UUID &value ) {{ return &value; }}")

	# end of namespace
	lines.append('    };')
	hlp.write_lines_to_file("../ISD/ISD_DataValuePointers.h",lines)

def run():
	ISD_DataTypes_h()
	ISD_DataTypes_cpp()
	ISD_DataValuePointers_h()