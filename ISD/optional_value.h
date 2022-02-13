// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include <memory>
#include "ISD_DataTypes.h"

namespace ISD
	{
	template<class T> class optional_value
		{
		protected:
			std::unique_ptr<T> value_m;

		public:
			optional_value() = default;
			optional_value( const T &_value ) : value_m( new T( _value ) ) {}
			optional_value( const optional_value &other ) : value_m( std::copy( other.value_m ) ) {}
			optional_value &operator = ( const optional_value &_other ) { this->value_m = std::copy( _other.value_m ); return *this; }
			optional_value( optional_value &&other ) : value_m( std::move( other.value_m ) ) {}
			optional_value &operator = ( optional_value &&_other ) { this->value_m = std::move( _other.value_m ); return *this; }

			bool operator==( const T &_other ) const
				{
				if( !this->value_m )
					return false;
				return *(this->value_m) == _other;
				}

			bool operator!=( const T &_other ) const
				{
				if( !this->value_m )
					return true;
				return *(this->value_m) != _other;
				}

			bool operator==( const optional_value &_other ) const
				{
				if( this->value_m )
					{
					if( _other.value_m )
						return *(this->value_m) == *(_other.value_m);
					return false;
					}
				else
					{
					if( _other.value_m )
						return false;
					return true;
					}
				}

			bool operator!=( const optional_value &_other ) const
				{
				if( this->value_m )
					{
					if( _other.value_m )
						return *(this->value_m) != *(_other.value_m);
					return true;
					}
				else
					{
					if( _other.value_m )
						return true;
					return false;
					}
				}

			void reset() { this->value_m.reset(); }
			void set( const T &_value = {} ) { this->value_m = std::make_unique<T>( _value ); }
			bool has_value() const { return bool( this->value_m ); }

			T &value() { ISDSanityCheckDebugMacro( this->has_value() ); return *(this->value_m); }
			const T &value() const { ISDSanityCheckDebugMacro( this->has_value() );	return *(this->value_m); }

			operator T &() { return value(); }
			operator const T &() const { return value(); }
		};
	};