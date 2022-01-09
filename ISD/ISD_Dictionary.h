// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

#include <map>

namespace ISD
	{
	enum DictionaryFlags : uint
		{
		ValidationNoNullEntities = 0x1, // if set, validation will fail if null entities exist in the dictionary
		};

	template<class _Kty, class _Ty, uint _Flags = 0, class _Pr = std::less<_Kty>, class _Alloc = std::allocator<std::pair<const _Kty, std::unique_ptr<_Ty>>>>
	class Dictionary
		{
		public:
			using key_type = _Kty;
			using mapped_type = _Ty;
			using key_compare = _Pr;
			using allocator_type = _Alloc;

			using map_type = std::map<_Kty, std::unique_ptr<_Ty>, _Pr, _Alloc>;
			using value_type = typename map_type::value_type;
			using iterator = typename map_type::iterator;

			class MF;
			friend MF;

		private:
			map_type Entities = {};

		public:
			map_type &GetEntities() { return this->Entities; }
			const map_type &GetEntities() const { return this->Entities; }
		};

	class EntityWriter;
	class EntityReader;
	class EntityValidator;

	template<class _Kty, class _Ty, uint _Flags, class _Pr, class _Alloc>
	class Dictionary<_Kty,_Ty,_Flags,_Pr,_Alloc>::MF
		{
		using _MgmCl = Dictionary<_Kty,_Ty,_Flags,_Pr,_Alloc>;

		public:
			static bool Write( _MgmCl &obj , EntityWriter &writer )
				{
				// collect the keys into a vector, and store in stream as an array
				std::vector<_Kty> keys(obj.Entities.size());
				size_t index = 0;
				for( auto it = obj.Entities.begin(); it != obj.Entities.end(); ++it, ++index )
					{
					keys[index] = it->first;
					}
				if( !writer.Write( ISDKeyMacro("IDs"), keys ) )
					return false;
				keys.clear();

				// create a sections array for the entities
				EntityWriter *section_writer = writer.BeginWriteSectionsArray( ISDKeyMacro("Entities"), obj.Entities.size() );
				if( !section_writer )
					return false;

				// write out all the entities as an array
				// for each non-empty entity, call the write method of the entity
				index = 0;
				for( auto it = obj.Entities.begin(); it != obj.Entities.end(); ++it, ++index )
					{
					if( !writer.BeginWriteSectionInArray( section_writer, index ) )
						return false;
					if( it->second )
						{
						if( !_Ty::MF::Write( *(it->second), *(section_writer) ) )
							return false;
						}
					if( !writer.EndWriteSectionInArray( section_writer, index ) )
						return false;
					}

				// sanity check, make sure all sections were written
				ISDSanityCheckDebugMacro( index == obj.Entities.size() );

				// end the Entities sections array
				if( !writer.EndWriteSectionsArray( section_writer ) )
					return false;

				return true;
				}

			static bool Read( _MgmCl &obj , EntityReader &reader )
				{
				EntityReader *section_reader = {};
				size_t map_size = {};
				bool success = {};
				typename _MgmCl::iterator it = {};

				// read in the keys as a vector
				std::vector<_Kty> keys;
				if( !reader.Read( ISDKeyMacro("IDs"), keys ) )
					return false;

				// begin the named sections array
				std::tie( section_reader, map_size, success ) = reader.BeginReadSectionsArray( ISDKeyMacro("Entities"), false );
				if( !success )
					return false;
				ISDSanityCheckDebugMacro( section_reader );
				if( map_size != keys.size() )
					{
					ISDErrorLog << "Invalid size in Dictionary, the Keys and Entities arrays do not match in size." << ISDErrorLogEnd;
					return false;
					}

				// read in all the entities, push into map as key-value pairs
				obj.Entities.clear();
				for( size_t index = 0; index < map_size ; ++index )
					{
					bool has_data = false;
					if( !reader.BeginReadSectionInArray( section_reader, index, &has_data ) )
						return false;

					if( has_data )
						std::tie(it,success) = obj.Entities.emplace( keys[index], std::make_unique<_Ty>() );
					else 
						std::tie(it,success) = obj.Entities.emplace( keys[index], nullptr );

					if( !success )
						{
						ISDErrorLog << "Failed inserting key-value pair in Dictionary" << ISDErrorLogEnd;
						return false;
						}

					if( it->second )
						{
						if( !_Ty::MF::Read( *(it->second), *(section_reader) ) )
							return false;
						}

					if( !reader.EndReadSectionInArray( section_reader, index ) )
						return false;
					}

				// end the sections array
				if( !reader.EndReadSectionsArray( section_reader ) )
					return false;

				return true;
				}

			static bool Validate( const _MgmCl &obj , EntityValidator &validator )
				{
				for( auto it = obj.Entities.begin(); it != obj.Entities.end(); ++it )
					{
					if( it->second )
						{
						if( !_Ty::MF::Validate( *(it->second) ) )
							return false;
						}
					else if( (_Flags & DictionaryFlags::ValidationNoNullEntities) != 0 )
						{
						ISDValidationError( ValidationError::NullNotAllowed ) << "Null entities are not allowed in this Directory" << ISDErrorLogEnd;
						return false;
						}
					}
				return true;
				}
		};


	};