// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include <vector>
#include "ISD_DataTypes.h"

namespace ISD
	{
	// optional_vector: optional std::vector 
	template <class _Ty, class _Alloc = std::allocator<_Ty>>
	class optional_vector 
		{
		public:
			using value_type = _Ty;
			using allocator_type = _Alloc;
			using pointer = typename std::vector<_Ty,_Alloc>::pointer;
			using const_pointer = typename std::vector<_Ty,_Alloc>::const_pointer;
			using reference = typename std::vector<_Ty,_Alloc>::reference;
			using const_reference = typename std::vector<_Ty,_Alloc>::const_reference;
			using size_type = typename std::vector<_Ty,_Alloc>::size_type;

		private:
			std::vector<_Ty,_Alloc> vector_m;
			bool has_value_m = false;

		public:
			optional_vector() = default;
			optional_vector( const optional_vector &_other ) : vector_m( _other.vector_m ), has_value_m(_other.has_value_m) {}
			optional_vector &operator = ( const optional_vector &_other ) { this->has_value_m = _other.has_value_m; this->vector_m = _other.vector_m; return *this; }
			optional_vector( optional_vector &&_other ) : vector_m( std::move(_other.vector_m) ), has_value_m(_other.has_value_m) { _other.has_value_m = false; }
			optional_vector &operator = ( optional_vector &&_other ) { this->has_value_m = _other.has_value_m; this->vector_m = std::move( _other.vector_m ); _other.has_value_m = false; return *this; }

			void reset() { this->has_value_m = false; this->vector_m.clear(); }
			void set() { this->has_value_m = true; this->vector_m.clear(); }
			void set( const std::vector<_Ty,_Alloc> &_values ) { this->has_value_m = true; this->vector_m = _values; }
			bool has_value() const { return this->has_value_m; }

			std::vector<_Ty,_Alloc> &vector() { ISDSanityCheckDebugMacro( this->has_value_m ); return this->vector_m; }
			const std::vector<_Ty,_Alloc> &vector() const { ISDSanityCheckDebugMacro( this->has_value_m ); return this->vector_m; }

			std::vector<_Ty,_Alloc> &values() { return vector(); }
			const std::vector<_Ty,_Alloc> &values() const { return vector(); }

			operator std::vector<_Ty, _Alloc> &() { return vector(); }
			operator const std::vector<_Ty,_Alloc> &() const { return vector(); }
		};
	};