// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#pragma once

#include "../ISD/ISD.h"
#include "../ISD/ISD_MemoryReadSteam.h"
#include "../ISD/ISD_MemoryWriteSteam.h"

#include <Rpc.h>

using namespace ISD;

#define TEST_ASSERT( must_be_true )\
	if( !(must_be_true) )\
		{\
		printf("Test failed: file:%s line: %d \n", __FILE__ , __LINE__ );\
		exit(-1);\
		}
