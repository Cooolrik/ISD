// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

#include <map>
#include <mutex>

namespace ISD
	{
	class EntityFactory;

	// small values of length < 256 bytes
	//  - single items of base types: bools, ints, floats, vec2s, vec3s, vec4s, UUIDs
	//struct Value
	//	{
	//	uint8 Type; // types 0x00 - 0x3f
	//	uint8 SizeInBytes; // can be used to skip value if not recognized
	//	uint8 Value[]; // <- defined size, based on Type (or with extra info inside)
	//	uint8 KeyData[];
	//	};

	// large values of length >= 256 bytes
	// used for:
	//  - nested blocks
	//  - arrays
	//  - strings
	// also used for larger strings of data
	//struct Block
	//	{
	//	uint8 Type; // types 0x40 - 0xff
	//	uint64 SizeInBytes; // to skip over block 
	//	uint8 KeySizeInBytes;
	//	uint8 KeyData[];
	//	uint8 Value[];
	//	};

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