// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

#include <map>
#include <mutex>

namespace ISD
	{
	class EntityFactory;
	
	class Entity
		{
		protected:
			UUID Id;
			std::string Name;

		public:
			const UUID &GetId() const { return this->Id; }
			const std::string &GetName() const { return this->Name; }

			static const EntityFactory &FactoryObject();
		};

	class EntityFactory
		{
		public:
			virtual Entity *NewEntity() const = 0;
		};
	};