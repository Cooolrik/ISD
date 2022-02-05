// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

namespace ISD
    {
    class TestEntity
        {
        public:
            class MF;
            friend MF;

            TestEntity() = default;
            TestEntity( const TestEntity &rval );
            TestEntity &operator=( const TestEntity &rval );
            TestEntity( TestEntity &&rval ) = default;
            TestEntity &operator=( TestEntity &&rval ) = default;
            ~TestEntity() = default;

            // value compare operators
            bool operator==( const TestEntity &rval ) const;
            bool operator!=( const TestEntity &rval ) const;

        protected:
            string v_Name = {};
            optional_value<string> v_OptionalText;

        public:
            // accessor for referencing variable Name;
            const string & Name() const { return this->v_Name; }
            string & Name() { return this->v_Name; }

            // accessor for referencing variable OptionalText;
            const optional_value<string> & OptionalText() const { return this->v_OptionalText; }
            optional_value<string> & OptionalText() { return this->v_OptionalText; }

        };

    class EntityWriter;
    class EntityReader;
    class EntityValidator;

    class TestEntity::MF
        {
        public:
            static void Clear( TestEntity &obj );
            static void DeepCopy( TestEntity &dest, const TestEntity *source );
            static bool Equals( const TestEntity *lvar, const TestEntity *rvar );

            static bool Write( const TestEntity &obj, EntityWriter &writer );
            static bool Read( TestEntity &obj, EntityReader &reader );

            static bool Validate( const TestEntity &obj, EntityValidator &validator );
        };

    inline TestEntity::TestEntity( const TestEntity &rval )
        {
        MF::DeepCopy( *this , &rval );
        }

    inline TestEntity &TestEntity::operator=( const TestEntity &rval )
        {
        MF::DeepCopy( *this , &rval );
        return *this;
        }

    inline bool TestEntity::operator==( const TestEntity &rval ) const
        {
        return MF::Equals( this, &rval );
        }

    inline bool TestEntity::operator!=( const TestEntity &rval ) const
        {
        return !(MF::Equals( this, &rval ));
        }

    };
