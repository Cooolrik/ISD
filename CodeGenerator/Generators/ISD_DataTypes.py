# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

import CodeGeneratorHelpers as hlp

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

	int_bit_range = [8,16,32,64]
	float_type_range = ['float','double']
	vector_dimension_range = [2,3,4] 
	nonconst_const_range = ['','const ']

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

	# end of namespace
	lines.append('    };')
	hlp.write_lines_to_file("../ISD/ISD_DataTypes.h",lines)


def ISD_DataItemPointers_h():
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

	int_bit_range = [8,16,32,64]
	float_type_range = ['float','double']
	vector_dimension_range = [2,3,4] 
	nonconst_const_range = ['','const ']

	# type pointer functions (return pointer to first item in each type)
	lines.append(f"\t// item pointer functions, returns a pointer to the first item of each type")
	for bit_size in int_bit_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}i{bit_size} *item_ptr( {const_type}i{bit_size} &value ) {{ return &value; }}")
	lines.append('')
	for bit_size in int_bit_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}u{bit_size} *item_ptr( {const_type}u{bit_size} &value ) {{ return &value; }}")
	lines.append('')
	for float_type in float_type_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}{float_type} *item_ptr( {const_type}{float_type} &value ) {{ return &value; }}")
	lines.append('')
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			for const_type in nonconst_const_range:
				lines.append(f"\tinline {const_type}i{bit_size} *item_ptr( {const_type}i{bit_size}vec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for bit_size in int_bit_range:
		for vec_dim in vector_dimension_range:
			for const_type in nonconst_const_range:
				lines.append(f"\tinline {const_type}u{bit_size} *item_ptr( {const_type}u{bit_size}vec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}float *item_ptr( {const_type}fvec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}double *item_ptr( {const_type}dvec{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}float *item_ptr( {const_type}fmat{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	for vec_dim in vector_dimension_range:
		for const_type in nonconst_const_range:
			lines.append(f"\tinline {const_type}double *item_ptr( {const_type}dmat{vec_dim} &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for const_type in nonconst_const_range:
		lines.append(f"\tinline {const_type}float *item_ptr( {const_type}fquat &value ) {{ return glm::value_ptr(value); }}")
	for const_type in nonconst_const_range:
		lines.append(f"\tinline {const_type}double *item_ptr( {const_type}dquat &value ) {{ return glm::value_ptr(value); }}")
	lines.append('')
	for const_type in nonconst_const_range:
		lines.append(f"\tinline {const_type}UUID *item_ptr( {const_type}UUID &value ) {{ return &value; }}")

	# end of namespace
	lines.append('    };')
	hlp.write_lines_to_file("../ISD/ISD_DataItemPointers.h",lines)

def run():
	ISD_DataTypes_h()
	ISD_DataItemPointers_h()