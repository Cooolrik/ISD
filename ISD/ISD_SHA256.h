// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

#include <vector>

namespace ISD
	{
	class SHA256
		{
		private:
			void *MDData = nullptr;

		public:
			SHA256( const uint8 *Data = nullptr , size_t DataLength = 0 );
			~SHA256();

			// update the SHA with the data at Data, length DataLength
			void Update( const uint8 *Data, size_t DataLength );

			// get the calculated digest 
			void GetDigest( uint8 DestDigest[32] );
		};
	};