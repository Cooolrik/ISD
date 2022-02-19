// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include <vector>
#include "ISD_DataTypes.h"

namespace ISD
	{
	// optional_idx_vector: optional idx_vector 
	template <class _Ty, class _Alloc = std::allocator<_Ty>, class _IdxAlloc = std::allocator<i32>>
	class optional_idx_vector 
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
			idx_vector<_Ty,_Alloc> vector_m;
			bool has_value_m = false;

		public:
			optional_idx_vector() = default;
			optional_idx_vector( const optional_idx_vector &_other ) noexcept : vector_m( _other.vector_m ), has_value_m(_other.has_value_m) {}
			optional_idx_vector &operator = ( const optional_idx_vector &_other ) { this->vector_m = _other.vector_m; this->has_value_m = _other.has_value_m; return *this; }
			optional_idx_vector( optional_idx_vector &&_other ) noexcept : vector_m( std::move(_other.vector_m) ), has_value_m(_other.has_value_m) { _other.has_value_m = false; }
			optional_idx_vector &operator = ( optional_idx_vector &&_other ) { this->vector_m = std::move(_other.vector_m); this->has_value_m = _other.has_value_m; _other.has_value_m = false; return *this; }

			bool operator==( const optional_idx_vector &_other ) const;
			bool operator!=( const optional_idx_vector &_other ) const;

			void reset() { this->has_value_m = false; this->vector_m.clear(); }
			void set() { this->has_value_m = true; this->vector_m.clear(); }
			void set( const idx_vector<_Ty,_Alloc,_IdxAlloc> &_vector ) { this->has_value_m = true; this->vector_m = _vector; }
			void set( const std::vector<_Ty,_Alloc> &_values , const std::vector<i32,_IdxAlloc> &_index ) { this->has_value_m = true; this->vector_m.values() = _values; this->vector_m.index() = _index; }
			bool has_value() const { return this->has_value_m; }

			idx_vector<_Ty,_Alloc> &vector() { ISDSanityCheckDebugMacro( this->has_value_m ); return this->vector_m; }
			const idx_vector<_Ty,_Alloc> &vector() const { ISDSanityCheckDebugMacro( this->has_value_m ); return this->vector_m; }

			std::vector<_Ty,_Alloc> &values() { return this->vector().values(); }
			const std::vector<_Ty,_Alloc> &values() const { return this->vector().values(); }

			std::vector<i32,_IdxAlloc> &index() { return this->vector().index(); }
			const std::vector<i32,_IdxAlloc> &index() const { return this->vector().index(); }
		};

	template <class _Ty, class _Alloc, class _IdxAlloc>
	bool optional_idx_vector<_Ty,_Alloc,_IdxAlloc>::operator==( const optional_idx_vector &_other ) const
		{
		if( this->has_value_m )
			{
			if( _other.has_value_m )
				return this->vector_m == _other.vector_m;
			return false;
			}
		else
			{
			if( _other.has_value_m )
				return false;
			return true;
			}
		}

	template <class _Ty, class _Alloc, class _IdxAlloc>
	bool optional_idx_vector<_Ty,_Alloc,_IdxAlloc>::operator!=( const optional_idx_vector &_other ) const
		{
		if( this->has_value_m )
			{
			if( _other.has_value_m )
				return this->vector_m != _other.vector_m;
			return true;
			}
		else
			{
			if( _other.has_value_m )
				return true;
			return false;
			}
		}

	};