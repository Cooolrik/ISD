# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE
import CodeGeneratorHelpers as hlp
import Entities as ents

def CreateEntityHeader(entity):
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append('#pragma once')
	lines.append('')
	lines.append('#include "ISD_Types.h"')
		
	# list dependences that needs to be included in the header
	for dep in entity.Dependencies:
		if dep.IncludeInHeader:
			lines.append(f'#include "ISD_{dep.Name}.h"')

	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')

	# list dependences that only needs a forward reference in the header
	for dep in entity.Dependencies:
		if not dep.IncludeInHeader:
			lines.append(f'    class {dep.Name};')

	lines.append(f'    class {entity.Name}')
	lines.append('        {')
	lines.append('        public:')

	# list typedefs of templates
	if len(entity.Templates) > 0:
		for typ in entity.Templates:
			decl = f'            using {typ.Name} = {typ.Template}<'
			has_value = False
			for ty in typ.Types:
				if has_value:
					decl += ','
				else:
					has_value = True
				decl += ty

			decl += '>;'
			lines.append(decl)
		lines.append('')

	lines.append('            class MF;')
	lines.append('            friend MF;')
	lines.append('')

	lines.append(f'            {entity.Name}() = default;')
	lines.append(f'            {entity.Name}( const {entity.Name} &rval );')
	lines.append(f'            {entity.Name} &operator=( const {entity.Name} &rval );')
	lines.append(f'            {entity.Name}( {entity.Name} &&rval ) = default;')
	lines.append(f'            {entity.Name} &operator=( {entity.Name} &&rval ) = default;')
	lines.append(f'            ~{entity.Name}() = default;')
	lines.append('')

	lines.append('            // value compare operators')
	lines.append(f'            bool operator==( const {entity.Name} &rval ) const;')
	lines.append(f'            bool operator!=( const {entity.Name} &rval ) const;')
	lines.append('')

	lines.append('        protected:')
	
	# list variables in entity
	for var in entity.Variables:
		base_type,base_variant = hlp.get_base_type_variant(var.Type)
		if var.Optional:
			lines.append(f'            optional_value<{var.Type}> v_{var.Name};')
		else:
			if base_type is not None:
				lines.append(f'            {var.Type} v_{var.Name} = {{}};')
			else:
				lines.append(f'            {var.Type} v_{var.Name};')

	lines.append('')
	lines.append('        public:')

	# create accessor ref for variables, const and non-const versions
	for var in entity.Variables:
		lines.append(f'            // accessor for referencing variable {var.Name};')
		if var.Optional:
			lines.append(f'            const optional_value<{var.Type}> & {var.Name}() const {{ return this->v_{var.Name}; }}')
			lines.append(f'            optional_value<{var.Type}> & {var.Name}() {{ return this->v_{var.Name}; }}')
		else:	
			lines.append(f'            const {var.Type} & {var.Name}() const {{ return this->v_{var.Name}; }}')
			lines.append(f'            {var.Type} & {var.Name}() {{ return this->v_{var.Name}; }}')
		lines.append('')

	lines.append('        };')

	lines.append('')
	lines.append('    class EntityWriter;')
	lines.append('    class EntityReader;')
	lines.append('    class EntityValidator;')

	lines.append('')
	lines.append(f'    class {entity.Name}::MF')
	lines.append('        {')
	lines.append('        public:')
	lines.append(f'            static void Clear( {entity.Name} &obj );')
	lines.append(f'            static void DeepCopy( {entity.Name} &dest, const {entity.Name} *source );')
	lines.append(f'            static bool Equals( const {entity.Name} *lvar, const {entity.Name} *rvar );')
	lines.append('')
	lines.append(f'            static bool Write( const {entity.Name} &obj, EntityWriter &writer );')
	lines.append(f'            static bool Read( {entity.Name} &obj, EntityReader &reader );')
	lines.append('')
	lines.append(f'            static bool Validate( const {entity.Name} &obj, EntityValidator &validator );')
	lines.append('        };')
	lines.append('')
	
	# ctors and copy operator code
	lines.append(f'    inline {entity.Name}::{entity.Name}( const {entity.Name} &rval )')
	lines.append('        {')
	lines.append('        MF::DeepCopy( *this , &rval );')
	lines.append('        }')
	lines.append('')
	lines.append(f'    inline {entity.Name} &{entity.Name}::operator=( const {entity.Name} &rval )')
	lines.append('        {')
	lines.append('        MF::DeepCopy( *this , &rval );')
	lines.append('        return *this;')
	lines.append('        }')
	lines.append('')
	lines.append(f'    inline bool {entity.Name}::operator==( const {entity.Name} &rval ) const')
	lines.append('        {')
	lines.append('        return MF::Equals( this, &rval );')
	lines.append('        }')
	lines.append('')
	lines.append(f'    inline bool {entity.Name}::operator!=( const {entity.Name} &rval ) const')
	lines.append('        {')
	lines.append('        return !(MF::Equals( this, &rval ));')
	lines.append('        }')
	lines.append('')

	lines.append('    };')

	hlp.write_lines_to_file(f"../ISD/ISD_{entity.Name}.h",lines)

def ImplementClearCall(entity,var):
	lines = []

	lines.append('')
	lines.append(f'        // clear variable "{var.Name}"')

	# clear all values, base values and Entities
	base_type,base_variant = hlp.get_base_type_variant(var.Type)
	if base_type is not None:
		# we have a base type, add the write code directly
		if var.Optional:
			lines.append(f'        obj.v_{var.Name}.reset();')
		else:
			lines.append(f'        obj.v_{var.Name} = {{}};')
	else:
		lines.append(f'        {var.Type}::MF::Clear( obj.v_{var.Name} );')

	return lines

def ImplementDeepCopyCall(entity,var):
	lines = []

	lines.append('')
	lines.append(f'        // copy variable "{var.Name}"')

	# clear all base values, Entities will clear themselves
	# deep copy all values
	base_type,base_variant = hlp.get_base_type_variant(var.Type)
	if base_type is not None:
		# we have a base type, add the write code directly
		if var.Optional:
			lines.append(f'        if( source->v_{var.Name}.has_value() )')
			lines.append(f'            dest.v_{var.Name}.set( source->v_{var.Name}.value() );')
			lines.append(f'        else')
			lines.append(f'            dest.v_{var.Name}.reset();')
		else:
			lines.append(f'        dest.v_{var.Name} = source->v_{var.Name};')
	else:
		if var.Optional:
			lines.append(f'        if( source->v_{var.Name}.has_value() )')
			lines.append('            {')
			lines.append(f'            dest.v_{var.Name}.set();')
			lines.append(f'            {var.Type}::MF::DeepCopy( dest.v_{var.Name}.value() , &(source->v_{var.Name}.value()) );')
			lines.append('            }')
			lines.append(f'        else')
			lines.append('            {')
			lines.append(f'            dest.v_{var.Name}.reset();')			
			lines.append('            }')
		else:
			lines.append(f'        {var.Type}::MF::DeepCopy( dest.v_{var.Name} , &(source->v_{var.Name}) );')


	return lines

def ImplementEqualsCall(entity,var):
	lines = []

	lines.append(f'        // check variable "{var.Name}"')

	# do we have a base type or entity?
	base_type,base_variant = hlp.get_base_type_variant(var.Type)
	if base_type is not None:
		# we have a base type, do the compare directly
		if var.Optional:
			lines.append(f'        if( lvar->v_{var.Name}.has_value() ) ')
			lines.append('            {')
			lines.append('            // lvar has a value. if rval has no value, not equal')
			lines.append(f'            if( !rvar->v_{var.Name}.has_value() ) ')
			lines.append('                return false;')
			lines.append('            // both have values, compare')
			lines.append(f'            if( lvar->v_{var.Name}.value() != rvar->v_{var.Name}.value() )')
			lines.append('                return false;')
			lines.append('            }')
			lines.append('        else')
			lines.append('            {')
			lines.append('            // lvar has a value. if rval has a value, not equal')
			lines.append(f'            if( rvar->v_{var.Name}.has_value() ) ')
			lines.append('                return false;')
			lines.append('            }')

		else:
			lines.append(f'        if( lvar->v_{var.Name} != rvar->v_{var.Name} )')
			lines.append(f'            return false;')
	else:
		# not a base type, so an entity. check entity
		if var.Optional:
			lines.append(f'        if( !{entity.Name}::{var.Type}::MF::Equals(')
			lines.append(f'            lvar->v_{var.Name}.has_value() ? &lvar->v_{var.Name}.value() : nullptr,  ')
			lines.append(f'            rvar->v_{var.Name}.has_value() ? &rvar->v_{var.Name}.value() : nullptr')
			lines.append(f'            ) )')
			lines.append('            return false;')
		else:
			lines.append(f'        if( !{entity.Name}::{var.Type}::MF::Equals( &lvar->v_{var.Name} , &rvar->v_{var.Name} ) )')
			lines.append('            return false;')

	lines.append('')

	return lines

def ImplementWriterCall(entity,var):
	lines = []

	# do we have a base type or entity?
	base_type,base_variant = hlp.get_base_type_variant(var.Type)
	if base_type is not None:
		# we have a base type, add the write code directly
		lines.append(f'        // write variable "{var.Name}"')
		if var.Optional:
			lines.append(f'        success = writer.Write<optional_value<{var.Type}>>( ISDKeyMacro("{var.Name}") , obj.v_{var.Name} );')
		else:
			lines.append(f'        success = writer.Write<{var.Type}>( ISDKeyMacro("{var.Name}") , obj.v_{var.Name} );')
		lines.append(f'        if( !success )')
		lines.append(f'            return false;')
		lines.append('')
	else:
		# not a base type, so an entity. add a block
		lines.append(f'        // write section "{var.Name}"')
		lines.append(f'        success = (section_writer = writer.BeginWriteSection( ISDKeyMacro("{var.Name}") ));')
		lines.append('        if( !success )')
		lines.append('            return false;')
		if var.Optional:
			lines.append(f'        if( obj.v_{var.Name}.has_value() )')
			lines.append('            {')
			lines.append(f'            if( !{entity.Name}::{var.Type}::MF::Write( obj.v_{var.Name}.value(), *section_writer ) )')
			lines.append('                return false;')
			lines.append('            }')
		else:
			lines.append(f'        if( !{entity.Name}::{var.Type}::MF::Write( obj.v_{var.Name}, *section_writer ) )')
			lines.append('            return false;')
		lines.append('        writer.EndWriteSection( section_writer );')
		lines.append('        section_writer = nullptr;')
		lines.append('')

	return lines

def ImplementReaderCall(entity,var):
	lines = []

	if var.Optional:
		value_can_be_null = "true"
	else:
		value_can_be_null = "false"

	# do we have a base type or entity?
	base_type,base_variant = hlp.get_base_type_variant(var.Type)
	if base_type is not None:
		# we have a base type, add the read code directly
		lines.append(f'        // read variable "{var.Name}"')
		if var.Optional:
			lines.append(f'        success = reader.Read<optional_value<{var.Type}>>( ISDKeyMacro("{var.Name}") , obj.v_{var.Name} );')
		else:
			lines.append(f'        success = reader.Read<{var.Type}>( ISDKeyMacro("{var.Name}") , obj.v_{var.Name} );')
		lines.append(f'        if( !success )')
		lines.append(f'            return false;')
		lines.append('')
	else:
		# not a base type, so an entity. add a block
		lines.append(f'        // read section "{var.Name}"')
		lines.append(f'        std::tie(section_reader,success) = reader.BeginReadSection( ISDKeyMacro("{var.Name}") , {value_can_be_null} );')
		lines.append('        if( !success )')
		lines.append('            return false;')
		lines.append('        if( section_reader )')
		lines.append('            {')
		if var.Optional:
			lines.append(f'            obj.v_{var.Name}.set();')
			lines.append(f'            if( !{entity.Name}::{var.Type}::MF::Read( obj.v_{var.Name}.value(), *section_reader ) )')
		else:
			lines.append(f'            if( !{entity.Name}::{var.Type}::MF::Read( obj.v_{var.Name}, *section_reader ) )')
		lines.append('                return false;')
		lines.append('            reader.EndReadSection( section_reader );')
		lines.append('            section_reader = nullptr;')
		lines.append('            }')
		if var.Optional:
			lines.append('        else')
			lines.append(f'            obj.v_{var.Name}.reset();')
		lines.append('')

	return lines

def ImplementValidatorCall(entity,var):
	lines = []

	# clear all values, base values and Entities
	base_type,base_variant = hlp.get_base_type_variant(var.Type)
	if base_type is None:
		lines.append(f'        // validate variable "{var.Name}"')
		if var.Optional:
			lines.append(f'        if( obj.v_{var.Name}.has_value() )')
			lines.append('            {')
			lines.append(f'            success = {var.Type}::MF::Validate( obj.v_{var.Name}.value() , validator );')
			lines.append('            if( !success )')
			lines.append('                return false;')
			lines.append('            }')
		else:
			lines.append(f'        success = {var.Type}::MF::Validate( obj.v_{var.Name} , validator );')
			lines.append('        if( !success )')
			lines.append('            return false;')
		lines.append('')

	return lines


def CreateEntitySource(entity):
	lines = []
	lines.append('// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append('// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append('')
	lines.append(f'#include "ISD_Types.h"')
	lines.append(f'#include "ISD_EntityWriter.h"')
	lines.append(f'#include "ISD_EntityReader.h"')
	lines.append(f'#include "ISD_EntityValidator.h"')
	lines.append('')
	lines.append(f'#include "ISD_{entity.Name}.h"')
		
	# include dependences that were forward referenced in the header
	for dep in entity.Dependencies:
		if not dep.IncludeInHeader:
			lines.append(f'#include "ISD_{dep.Name}.h"')
		
	lines.append('')
	lines.append('namespace ISD')
	lines.append('    {')
	
	# check if there are entities in the variable list
	vars_have_entity = False
	for var in entity.Variables:
		base_type,base_variant = hlp.get_base_type_variant(var.Type)
		if base_type is not None:
			vars_have_entity = True
			break
	
	# clear code
	lines.append(f'    void {entity.Name}::MF::Clear( {entity.Name} &obj )')
	lines.append('        {')
	lines.append('        // direct clear calls on variables and Entities')
	for var in entity.Variables:
		lines.extend(ImplementClearCall(entity,var))
	lines.append('        }')
	lines.append('')

	# deep copy code
	lines.append(f'    void {entity.Name}::MF::DeepCopy( {entity.Name} &dest, const {entity.Name} *source )')
	lines.append('        {')
	lines.append('        // just call Clear if source is nullptr')
	lines.append('        if( !source )')
	lines.append('            {')
	lines.append('            MF::Clear( dest );')
	lines.append('            return;')
	lines.append('            }')
	for var in entity.Variables:
		lines.extend(ImplementDeepCopyCall(entity,var))
	lines.append('        }')
	lines.append('')

	# equals code
	lines.append(f'    bool {entity.Name}::MF::Equals( const {entity.Name} *lvar, const {entity.Name} *rvar )')
	lines.append('        {')
	lines.append('        // early out if pointers are equal')
	lines.append('        if( lvar == rvar )')
	lines.append('            return true;')
	lines.append('')
	lines.append('        // early out if one of the pointers is nullptr')
	lines.append('        if( !lvar || !rvar )')
	lines.append('            return false;')
	lines.append('')
	for var in entity.Variables:
		lines.extend(ImplementEqualsCall(entity,var))
	lines.append('        return true;')
	lines.append('        }')
	lines.append('')

	# writer code
	lines.append(f'    bool {entity.Name}::MF::Write( const {entity.Name} &obj, EntityWriter &writer )')
	lines.append('        {')
	lines.append('        bool success = true;')
	if vars_have_entity:
		lines.append('        EntityWriter *section_writer = nullptr;')
	lines.append('')
	for var in entity.Variables:
		lines.extend(ImplementWriterCall(entity,var))
	lines.append('        return true;')
	lines.append('        }')
	lines.append('')
	
	# reader code
	lines.append(f'    bool {entity.Name}::MF::Read( {entity.Name} &obj, EntityReader &reader )')
	lines.append('        {')
	lines.append('        bool success = true;')
	if vars_have_entity:
		lines.append('        EntityReader *section_reader = nullptr;')
	lines.append('')
	for var in entity.Variables:
		lines.extend(ImplementReaderCall(entity,var))
	lines.append('        return true;')
	lines.append('        }')
	lines.append('')
	
	# validator code
	lines.append(f'    bool {entity.Name}::MF::Validate( const {entity.Name} &obj, EntityValidator &validator )')
	lines.append('        {')
	lines.append('        bool success = true;')
	lines.append('')
	for var in entity.Variables:
		lines.extend(ImplementValidatorCall(entity,var))
	lines.append('        return true;')
	lines.append('        }')
	lines.append('')

	lines.append('    };')
	hlp.write_lines_to_file(f"../ISD/ISD_{entity.Name}.cpp",lines)


def run():
	for entity in ents.Entities:
		CreateEntityHeader(entity)
		CreateEntitySource(entity)

