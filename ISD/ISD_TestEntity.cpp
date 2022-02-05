// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "ISD_Types.h"
#include "ISD_EntityWriter.h"
#include "ISD_EntityReader.h"
#include "ISD_EntityValidator.h"

#include "ISD_TestEntity.h"

namespace ISD
    {
    void TestEntity::MF::Clear( TestEntity &obj )
        {
        // direct clear calls on variables and Entities

        // clear variable "Name"
        obj.v_Name = {};

        // clear variable "OptionalText"
        obj.v_OptionalText.reset();
        }

    void TestEntity::MF::DeepCopy( TestEntity &dest, const TestEntity *source )
        {
        // just call Clear if source is nullptr
        if( !source )
            {
            MF::Clear( dest );
            return;
            }

        // copy variable "Name"
        dest.v_Name = source->v_Name;

        // copy variable "OptionalText"
        if( source->v_OptionalText.has_value() )
            dest.v_OptionalText.set( source->v_OptionalText.value() );
        else
            dest.v_OptionalText.reset();
        }

    bool TestEntity::MF::Equals( const TestEntity *lvar, const TestEntity *rvar )
        {
        // early out if pointers are equal
        if( lvar == rvar )
            return true;

        // early out if one of the pointers is nullptr
        if( !lvar || !rvar )
            return false;

        // check variable "Name"
        if( lvar->v_Name != rvar->v_Name )
            return false;

        // check variable "OptionalText"
        if( lvar->v_OptionalText.has_value() != rvar->v_OptionalText.has_value() )
            return false;
        if( lvar->v_OptionalText.has_value() )
            {
            if( lvar->v_OptionalText.value() != rvar->v_OptionalText.value() )
                return false;
            }

        return true;
        }

    bool TestEntity::MF::Write( const TestEntity &obj, EntityWriter &writer )
        {
        bool success = true;
        EntityWriter *section_writer = nullptr;

        // write variable "Name"
        success = writer.Write<string>( ISDKeyMacro("Name") , obj.v_Name );
        if( !success )
            return false;

        // write variable "OptionalText"
        success = writer.Write<optional_value<string>>( ISDKeyMacro("OptionalText") , obj.v_OptionalText );
        if( !success )
            return false;

        return true;
        }

    bool TestEntity::MF::Read( TestEntity &obj, EntityReader &reader )
        {
        bool success = true;
        EntityReader *section_reader = nullptr;

        // read variable "Name"
        success = reader.Read<string>( ISDKeyMacro("Name") , obj.v_Name );
        if( !success )
            return false;

        // read variable "OptionalText"
        success = reader.Read<optional_value<string>>( ISDKeyMacro("OptionalText") , obj.v_OptionalText );
        if( !success )
            return false;

        return true;
        }

    bool TestEntity::MF::Validate( const TestEntity &obj, EntityValidator &validator )
        {
        bool success = true;

        return true;
        }

    };
