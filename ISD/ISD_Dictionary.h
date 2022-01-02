// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

#include <map>

namespace ISD
	{
	template<class _Kty, class _Ty, bool _null_entites_allowed = true, class _Pr = std::less<_Kty>, class _Alloc = std::allocator<std::pair<const _Kty, std::unique_ptr<_Ty>>>>
	class Dictionary
		{
		public:
			using key_type = _Kty;
			using mapped_type = std::unique_ptr<_Ty>;
			using key_compare = _Pr;
			using allocator_type = _Alloc;
			using map_type = std::map<_Kty, std::unique_ptr<_Ty>, _Pr, _Alloc>;
			using map_type_ref = std::map<_Kty, std::unique_ptr<_Ty>, _Pr, _Alloc> &;
			using const_map_type_ref = const map_type_ref;
			using value_type = typename map_type::value_type;
			using iterator = typename map_type::iterator;

		private:
			map_type Entities = {};

		public:
			bool Write( EntityWriter &writer );
			bool Read( EntityReader &reader );

			map_type_ref GetEntities() { return this->Entities; }
			const_map_type_ref GetEntities() const { return this->Entities; }
		};

	template<class _Kty, class _Ty, bool _null_entites_allowed, class _Pr, class _Alloc>
	bool Dictionary<_Kty,_Ty, _null_entites_allowed,_Pr,_Alloc>::Write( EntityWriter &writer )
		{
		// collect the keys into a vector, and store in stream as an array
		std::vector<_Kty> keys(this->Entities.size());
		size_t index = 0;
		for( auto it = this->Entities.begin(); it != this->Entities.end(); ++it, ++index )
			{
			keys[index] = it->first;
			}
		if( !writer.Write( ISDKeyMacro("IDs"), keys ) )
			return false;
		keys.clear();

		// create a sections array for the entities
		EntityWriter *section_writer = writer.BeginWriteSectionsArray( ISDKeyMacro("Entities"), this->Entities.size() );
		if( !section_writer )
			return false;

		// write out all the entities as an array
		// for each non-empty entity, call the write method of the entity
		index = 0;
		for( auto it = this->Entities.begin(); it != this->Entities.end(); ++it, ++index )
			{
			if( !writer.BeginWriteSectionInArray( section_writer, index ) )
				return false;
			if( it->second )
				{
				if( !it->second->Write( *section_writer ) )
					return false;
				}
			if( !writer.EndWriteSectionInArray( section_writer, index ) )
				return false;
			}

		// sanity check, make sure all sections were written
		ISDSanityCheckDebugMacro( index == this->Entities.size() );

		// end the Entities sections array
		if( !writer.EndWriteSectionsArray( section_writer ) )
			return false;

		return true;
		}

	template<class _Kty, class _Ty, bool _null_entites_allowed, class _Pr, class _Alloc>
	bool Dictionary<_Kty,_Ty, _null_entites_allowed,_Pr,_Alloc>::Read( EntityReader &reader )
		{
		EntityReader *section_reader = {};
		size_t map_size = {};
		bool success = {};
		iterator it = {};

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
		this->Entities.clear();
		for( size_t index = 0; index < map_size ; ++index )
			{
			bool has_data = false;
			if( !reader.BeginReadSectionInArray( section_reader, index, &has_data ) )
				return false;

			if( has_data )
				{
				// create the entity, and read in its contents
				std::tie(it,success) = this->Entities.emplace( keys[index], std::make_unique<_Ty>() );
				if( !success )
					{
					ISDErrorLog << "Failed inserting key-value pair in Dictionary" << ISDErrorLogEnd;
					return false;
					}
				if( !it->second->Read( *section_reader ) )
					return false;
				}
			else if( _null_entites_allowed )
				{
				std::tie(it,success) = this->Entities.emplace( keys[index], nullptr );
				if( !success )
					{
					ISDErrorLog << "Failed inserting nullptr key-value pair in Dictionary" << ISDErrorLogEnd;
					return false;
					}
				}
			else
				{
				ISDErrorLog << "Null entity found in Dictionary, but is not allowed" << ISDErrorLogEnd;
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


	};