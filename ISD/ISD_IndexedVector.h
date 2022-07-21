// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

namespace ISD
	{
	template <class _Ty, class _Alloc = std::allocator<_Ty>, class _IdxAlloc = std::allocator<i32>>
	class IndexedVector : public idx_vector<_Ty,_Alloc,_IdxAlloc>
		{
		public:
			class MF;
			friend MF;

			// ctors/dtor and copy/move operators
			IndexedVector() = default;
			IndexedVector( const IndexedVector &rval ) = default;
			IndexedVector &operator=( const IndexedVector &rval ) = default;
			IndexedVector( IndexedVector &&rval ) = default;
			IndexedVector &operator=( IndexedVector &&rval ) = default;
			~IndexedVector() = default;

			// value compare operators
			bool operator==( const IndexedVector &rval ) const { return MF::Equals( this, &rval ); }
			bool operator!=( const IndexedVector &rval ) const { return !(MF::Equals( this, &rval )); }
		};

	class EntityWriter;
	class EntityReader;
	class EntityValidator;

	template<class _Ty, class _Alloc, class _IdxAlloc>
	class IndexedVector<_Ty,_Alloc,_IdxAlloc>::MF
		{
		using _MgmCl = IndexedVector<_Ty,_Alloc,_IdxAlloc>;

		public:
			static void Clear( _MgmCl &obj );
			static void DeepCopy( _MgmCl &dest, const _MgmCl *source );
			static bool Equals( const _MgmCl *lval, const _MgmCl *rval );

			static bool Write( const _MgmCl &obj, EntityWriter &writer );
			static bool Read( _MgmCl &obj, EntityReader &reader );

			static bool Validate( const _MgmCl &obj, EntityValidator &validator );
		};

	template<class _Ty, class _Alloc, class _IdxAlloc>
	void IndexedVector<_Ty,_Alloc,_IdxAlloc>::MF::Clear( _MgmCl &obj )
		{
		obj.clear();
		}

	template<class _Ty, class _Alloc, class _IdxAlloc>
	void IndexedVector<_Ty,_Alloc,_IdxAlloc>::MF::DeepCopy( _MgmCl &dest, const _MgmCl *source )
		{
		MF::Clear( dest );
		if( !source )
			return;
		dest = *source;
		}

	template<class _Ty, class _Alloc, class _IdxAlloc>
	bool IndexedVector<_Ty,_Alloc,_IdxAlloc>::MF::Equals( const _MgmCl *lval, const _MgmCl *rval )
		{
		// early out if the pointers are equal (includes nullptr)
		if( lval == rval )
			return true;

		// early out if one of the pointers is nullptr (both can't be null because of above test)
		if( !lval || !rval )
			return false;

		// deep compare the values
		const idx_vector<_Ty, _Alloc, _IdxAlloc> &_lval = *lval;
		const idx_vector<_Ty, _Alloc, _IdxAlloc> &_rval = *rval;
		return _lval == _rval;
		}

	template<class _Ty, class _Alloc, class _IdxAlloc>
	bool IndexedVector<_Ty,_Alloc,_IdxAlloc>::MF::Write( const _MgmCl &obj, EntityWriter &writer )
		{
		const idx_vector<_Ty, _Alloc, _IdxAlloc> &_obj = obj;
		if( !writer.Write( ISDKeyMacro("Values"), _obj ) )
			return false;
		return true;
		}

	template<class _Ty, class _Alloc, class _IdxAlloc>
	bool IndexedVector<_Ty,_Alloc,_IdxAlloc>::MF::Read( _MgmCl &obj , EntityReader &reader )
		{
		idx_vector<_Ty, _Alloc, _IdxAlloc> &_obj = obj;
		if( !reader.Read( ISDKeyMacro("Values"), _obj ) )
			return false;
		return true;
		}

	template<class _Ty, class _Alloc, class _IdxAlloc>
	bool IndexedVector<_Ty,_Alloc,_IdxAlloc>::MF::Validate( const _MgmCl &obj , EntityValidator &validator )
		{
		if( obj.values().size() > (size_t)i32_sup )
			{
			ISDValidationError( ValidationError::InvalidCount ) << "This IndexedVector has too many values in the values vector. The limit is 2^31 values, which can be indexed by a 32-bit int." << ISDErrorLogEnd;
			}

		// cap the count to 32 bit int
		const u32 values_count = (u32)std::min( obj.values().size() , (size_t)i32_sup );
		for( size_t i=0; i<obj.index().size(); ++i )
			{
			if( (u32)obj.index()[i] >= values_count )
				{
				ISDValidationError( ValidationError::InvalidValue ) << "The value " << obj.index()[i] << " at position " << i << " of the index vector is out of bounds." << ISDErrorLogEnd;
				}
			}

		return true;
		}


	};