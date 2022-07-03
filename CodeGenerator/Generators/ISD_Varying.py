# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

import CodeGeneratorHelpers as hlp

def ISD_Varying_h():
	lines = []
	lines.append('// ISD Copyright (c) 2021,2022 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#include "ISD_Types.h"')
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')
	lines.append('    class Varying')
	lines.append('        {')
	lines.append('        public:')
	lines.append('            class MF;')
	lines.append('            friend MF;')
	lines.append('')
	lines.append('            Varying() = default;')
	lines.append('            Varying( const Varying &rval );')
	lines.append('            Varying &operator=( const Varying &rval );')
	lines.append('            Varying( Varying &&rval );')
	lines.append('            Varying &operator=( Varying &&rval );')
	lines.append('            ~Varying() = default;')
	lines.append('')
	lines.append('            // value compare operators')
	lines.append('            bool operator==( const Varying &rval ) const;')
	lines.append('            bool operator!=( const Varying &rval ) const;')
	lines.append('')
	lines.append('        protected:')
	lines.append('            data_type data_type_m = {};')
	lines.append('            container_type container_type_m = {};')
	lines.append('            void *data_m = {};')
	lines.append('')
	lines.append('        public:')
	lines.append('            // The current data type in the object')
	lines.append('            std::pair<data_type,container_type> Type() const noexcept { return std::pair<data_type,container_type>(this->data_type_m,this->container_type_m); }')
	lines.append('')
	lines.append('            // Returns true if the object is empty (ie has no data)')
	lines.append('            bool Empty() const noexcept { return this->data_m == nullptr; }')
	lines.append('')
	lines.append('            // Check if the data is of the template class type _Ty')
	lines.append('            template<class _Ty> bool IsA() const noexcept;')
	lines.append('')
	lines.append('            // Retreive a reference to the data in the object (use IsA to make sure the data type is correct)')
	lines.append('            template<class _Ty> const _Ty & Data() const;')
	lines.append('            template<class _Ty> _Ty & Data();')
	lines.append('		};')
	lines.append('')
	lines.append('    class EntityWriter;')
	lines.append('    class EntityReader;')
	lines.append('    class EntityValidator;')
	lines.append('')
	lines.append(f'    class Varying::MF')
	lines.append('        {')
	lines.append('        public:')
	lines.append(f'            static void Clear( Varying &obj );')
	lines.append(f'            static void DeepCopy( Varying &dest, const Varying *source );')
	lines.append(f'            static bool Equals( const Varying *lvar, const Varying *rvar );')
	lines.append('')
	lines.append(f'            static bool Write( const Varying &obj, EntityWriter &writer );')
	lines.append(f'            static bool Read( Varying &obj, EntityReader &reader );')
	lines.append('')
	lines.append(f'            static bool Validate( const Varying &obj, EntityValidator &validator );')
	lines.append('')
	lines.append('            // Method to set the type of the data in the varying object, either using a parameter, or as a template method')
	lines.append(f'            static void SetType( Varying &obj, data_type dataType, container_type containerType );')
	lines.append(f'            template <class _Ty> static _Ty & SetType( Varying &obj );')
	lines.append('        };')

	# print all base type combos for the template specializations
	method_line_list = [ 
		'    // {base_type_combo}',
		'    template <> bool Varying::IsA<{base_type_combo}>() const noexcept;' ,
		'    template <> const {base_type_combo} & Varying::Data<{base_type_combo}>() const;' ,
		'    template <> {base_type_combo} & Varying::Data<{base_type_combo}>();' ,
		'    template <> static {base_type_combo} & Varying::MF::SetType<{base_type_combo}>( Varying &obj );' ,
		''
		]
	lines.append('')
	lines.extend( hlp.generate_lines_for_all_basetype_combos( method_line_list ) )

	lines.append('    // other types are not possible')
	lines.append('	template <class _Ty> bool Varying::IsA() const noexcept')
	lines.append('		{')
	lines.append('		static_assert(false, "Error: Varying::IsA template: This template type is not possible in a Varying object.");')
	lines.append('		}')
	lines.append('	template <class _Ty> const _Ty & Varying::Data() const')
	lines.append('		{')
	lines.append('		static_assert(false, "Error: Varying::Data template: This template type is not possible in a Varying object.");')
	lines.append('		}')
	lines.append('	template <class _Ty> _Ty & Varying::Data()')
	lines.append('		{')
	lines.append('		static_assert(false, "Error: Varying::Data template: This template type is not possible in a Varying object.");')
	lines.append('		}')
	lines.append('	template <class _Ty> _Ty & Varying::MF::SetType( Varying &obj )')
	lines.append('		{')
	lines.append('		static_assert(false, "Error: Varying::MF::SetType template: This template type is not possible in a Varying object.");')
	lines.append('		}')
	lines.append('	};')
	hlp.write_lines_to_file("../ISD/ISD_Varying.h",lines)

def ISD_Varying_cpp():
	lines = []
	lines.append('// ISD Copyright (c) 2021,2022 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#include "ISD_Varying.h"')
	lines.append('#include <glm/glm.hpp>')
	lines.append('#include <glm/gtc/type_ptr.hpp>')
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')
	lines.append('    void Varying::MF::Clear( Varying &obj ) ')
	lines.append('        {')
	lines.append('        if( obj.data_m == nullptr )')
	lines.append('            return;')
	lines.append('        delete_type( obj.data_type_m , obj.container_type_m , obj.data_m );')
	lines.append('        obj.data_m = nullptr;')
	lines.append('        }')
	lines.append('')
	lines.append('    void Varying::MF::SetType( Varying &obj, data_type dataType, container_type containerType ) ')
	lines.append('        {')
	lines.append('        Clear( obj );')
	lines.append('')
	lines.append('        obj.data_type_m = dataType;')
	lines.append('        obj.container_type_m = containerType;')
	lines.append('        obj.data_m = new_type( obj.data_type_m , obj.container_type_m );')
	lines.append('        }')
	 
	# print all base type combos for the template specializations
	method_line_list = [ 
		'    // {base_type_combo}',
		'    template <> bool Varying::IsA<{base_type_combo}>() const noexcept' ,
		'        {{' ,
		'        return this->data_type_m == data_type::dt_{implementing_type}' ,
		'            && this->container_type_m == container_type::ct_{container_type};' ,
		'        }}' ,
		'    template <> const {base_type_combo} & Varying::Data<{base_type_combo}>() const' ,
		'        {{' ,
		'        ISDSanityCheckDebugMacro( this->IsA<{base_type_combo}>() );' ,
		'        ISDSanityCheckDebugMacro( !this->Empty() );' ,
		'        const {base_type_combo}* ptr = (const {base_type_combo}*)(this->data_m);' ,
		'        return *ptr;' ,
		'        }}' ,
		'    template <> {base_type_combo} & Varying::Data<{base_type_combo}>()' ,
		'        {{' ,
		'        ISDSanityCheckDebugMacro( this->IsA<{base_type_combo}>() );' ,
		'        ISDSanityCheckDebugMacro( !this->Empty() );' ,
		'        {base_type_combo}* ptr = ({base_type_combo}*)(this->data_m);' ,
		'        return *ptr;' ,
		'        }}' ,
		'    template <> static {base_type_combo} & Varying::MF::SetType<{base_type_combo}>( Varying &obj ) ' ,
		'        {{' ,
		'        SetType( obj , data_type::dt_{implementing_type} , container_type::ct_{container_type} );' ,
		'        return obj.Data<{base_type_combo}>();' ,
		'        }}' ,
		''
		]
	lines.append('')
	lines.extend( hlp.generate_lines_for_all_basetype_combos( method_line_list ) )

	lines.append('	};')
	hlp.write_lines_to_file("../ISD/ISD_Varying.cpp",lines)

def run():
	ISD_Varying_h()
	ISD_Varying_cpp()