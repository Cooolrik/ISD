// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include <vector>
#include "ISD_DataTypes.h"

namespace ISD
	{
	// idx_vector: std::vector of values, with an std::vector of int as index into the values
	template <class _Ty, class _Alloc = std::allocator<_Ty>, class _IdxAlloc = std::allocator<i32>>
	class idx_vector 
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
			std::vector<_Ty,_Alloc> values_m;
			std::vector<i32,_IdxAlloc> index_m;

		public:
			idx_vector() = default;
			idx_vector( const idx_vector &_other ) : values_m( _other.values_m ) , index_m(_other.index_m) {}
			idx_vector &operator = ( const idx_vector &_other ) { this->values_m = _other.values_m; this->index_m = _other.index_m; return *this; }
			idx_vector( idx_vector &&_other ) : values_m( std::move(_other.values_m) ) , index_m( std::move(_other.index_m) ) {}
			idx_vector &operator = ( idx_vector &&_other ) { this->values_m = std::move(_other.values_m); this->index_m = std::move(_other.index_m); return *this; }

			void clear() { this->values_m.clear(); this->index_m.clear(); }

			std::vector<_Ty,_Alloc> &values() { return this->values_m; }
			const std::vector<_Ty,_Alloc> &values() const { return this->values_m; }

			std::vector<i32,_IdxAlloc> &index() { return this->index_m; }
			const std::vector<i32,_IdxAlloc> &index() const { return this->index_m; }
		};

	};