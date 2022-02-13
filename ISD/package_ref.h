// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include <ostream>

#include "ISD_DataTypes.h"

namespace ISD
	{
	// Reference to another package. 
	class package_ref
		{
		private:
			hash hash_m;

		public:
			// ctors/dtor
			package_ref() noexcept : hash_m( hash_zero ) {}
			package_ref( const hash &_hash ) noexcept : hash_m( _hash ) {}
			package_ref( const package_ref &other ) noexcept : hash_m( other.hash_m ) {}
			package_ref &operator=( const package_ref &other ) noexcept { this->hash_m = other.hash_m; return *this; }
			package_ref( package_ref &&other ) noexcept : hash_m( other.hash_m ) { other.hash_m = hash_zero; }
			package_ref &operator=( package_ref &&other ) noexcept { this->hash_m = other.hash_m; other.hash_m = hash_zero; return *this; }
			~package_ref() = default;

			// comparing & sorting
			bool operator==( const package_ref &rval ) const { return this->hash_m == rval.hash_m; }
			bool operator!=( const package_ref &rval ) const { return this->hash_m != rval.hash_m; }
			bool operator<( const package_ref &rval ) const { return this->hash_m < rval.hash_m; }

			// conversions to boolean (behaves like a pointer, true if non-null) 
			operator bool() const { return this->hash_m != hash_zero; }
			operator hash() const { return this->hash_m; }
		};

	inline std::ostream &operator<<( std::ostream &os, const package_ref &ref )
		{
		return os << hash( ref );
		}

	template<> struct type_information<package_ref>
		{
		using value_type = package_ref; // the value type of package_ref ( package_ref )
		static constexpr size_t value_count = 1; // the number of values in package_ref ( 1 )
		static constexpr const char * value_name = "package_ref"; // name of the value in package_ref ( "package_ref" ) 
		static constexpr const char * type_name = "package_ref"; // name of the type ( "package_ref" ) 
		static const package_ref zero; // zero value of package_ref
		static const package_ref inf; // limit inferior (minimum possible value) of package_ref
		static const package_ref sup; // limit superior (maximum possible value) of package_ref
		};
	};
