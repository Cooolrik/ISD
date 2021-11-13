// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "..\ISD\ISD_Types.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>

using namespace ISD;

// add headers that you want to pre-compile here
inline uint8 uint8_rand() { return (uint8)(rand() & 0xff); }
inline uint16 uint16_rand() { return (rand() << 4) ^ rand(); }
inline uint32 uint32_rand() { return uint32(uint16_rand()) << 16 | uint32(uint16_rand()); }
inline uint64 uint64_rand() { return uint64(uint32_rand()) << 32 | uint64(uint32_rand()); }
inline float float_rand() { return float(uint64_rand()); }
inline double double_rand() { return double(uint64_rand()); }

inline UUID uuid_rand() 
	{ 
	const UUID id = {uint32_rand(), uint16_rand(), uint16_rand(), { uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand(), uint8_rand() }};
	return id;
	}

#endif //PCH_H
