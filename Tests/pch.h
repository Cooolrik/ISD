// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "..\ISD\ISD_Types.h"

#include <string>
#include <vector>
#include <set>

#include "..\TestHelpers\random_vals.h"

#ifdef _DEBUG
#define ISDExpectSanityCheckDebugFailMacro( statement ) Assert::ExpectException<std::exception>( [&]() { statement } );
#define ISDExpectSanityCheckCoreDebugFailMacro( statement ) Assert::ExpectException<std::exception>( [&]() { statement } );
#else
#define ISDExpectSanityCheckDebugFailMacro( statement ) 
#define ISDExpectSanityCheckCoreDebugFailMacro( statement ) 
#endif

#define STANDARD_TEST_INIT() \
	TEST_METHOD_INITIALIZE( InitMethod )\
		{\
		setup_random_seed();\
		}

#endif //PCH_H
