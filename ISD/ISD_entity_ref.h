// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include <ostream>

#include "ISD_DataTypes.h"

namespace ISD
	{
	// Reference to an element within the same package. Acts like a memory handle. 
	// It can be created, held and copied, but cannot be set to a specific value other than null.
	class entity_ref
		{
		private:
			uuid id_m;

			// set entity reference, only allow this internally and from EntityReader
			entity_ref( const uuid &id ) : id_m( id ) {};
			friend class EntityReader;

		public:
			// ctors/dtor
			entity_ref() noexcept : id_m( uuid_zero ) {}
			entity_ref( const entity_ref &other ) noexcept : id_m( other.id_m ) {}
			entity_ref &operator=( const entity_ref &other ) noexcept { this->id_m = other.id_m; return *this; }
			entity_ref( entity_ref &&other ) noexcept : id_m( other.id_m ) { other.id_m = uuid_zero; }
			entity_ref &operator=( entity_ref&& other ) noexcept { this->id_m = other.id_m; other.id_m = uuid_zero; return *this; }
			~entity_ref() = default;

			// make a new reference with a unique uuid
			static entity_ref make_ref();

			// comparing & sorting
			bool operator==( const entity_ref &rval ) const { return this->id_m == rval.id_m; }
			bool operator!=( const entity_ref &rval ) const { return this->id_m != rval.id_m; }
			bool operator<( const entity_ref &rval ) const { return this->id_m < rval.id_m; }

			// conversions to boolean (behaves like a pointer, true if non-null) 
			operator bool() const { return this->id_m != uuid_zero; }
			operator uuid() const { return this->id_m; }

			// returns the "null" entity_ref value 
			static const entity_ref &null() { static entity_ref null_id; return null_id; }
		};

	inline std::ostream &operator<<( std::ostream &os, const entity_ref &ref )
		{
		return os << uuid( ref );
		}

	template<> struct type_information<entity_ref>
		{
		using value_type = entity_ref; // the value type of entity_ref ( entity_ref )
		static constexpr size_t value_count = 1; // the number of values in entity_ref ( 1 )
		static constexpr const char * value_name = "entity_ref"; // name of the value in entity_ref ( "entity_ref" ) 
		static constexpr const char * type_name = "entity_ref"; // name of the type ( "entity_ref" ) 
		static const entity_ref zero; // zero value of entity_ref
		static const entity_ref inf; // limit inferior (minimum possible value) of entity_ref
		static const entity_ref sup; // limit superior (maximum possible value) of entity_ref
		};

	};
