// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

#include <map>
#include <mutex>

namespace ISD
	{
	class Entity
		{
		private:
			UUID Uuid;
			std::string Name;

		public:
			const UUID &GetUUID() const { return this->Uuid; }
			const std::string &GetName() const { return this->Name; }
		};

	class EntityLoader
		{
		private:
			std::wstring Path;

			thread_safe_map<UUID, const Entity *> LoadedEntities;
			thread_safe_map<UUID, void *> ActiveThreads;

		public:
			Status Initialize( const std::string &path );

			Status AsyncLoadEntity( const UUID &uuid );
			std::pair<bool, Status> IsEntityLoaded( const UUID &uuid );
			std::pair<const Entity *, Status> GetLoadedEntity( const UUID &uuid );
		};

	};