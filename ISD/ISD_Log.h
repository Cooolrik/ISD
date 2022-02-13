// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#pragma once

#include "ISD_Types.h"

#include <map>
#include <mutex>
#include <iostream>

namespace ISD
	{
	class Log
		{
		public:
			static std::ostream &Error( const char *funcsig, const char *filename, int fileline ) 
				{ 
				std::cout << "Error: " << "\n" << "\t";
				return std::cout; 
				}

		};

	};