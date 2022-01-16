// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "ISD_SHA256.h"

#include <librock_sha256.c>

namespace ISD
	{
	SHA256::SHA256( const u8 *Data , size_t DataLength )
		{
		this->MDData = malloc( librock_SHA256_Init(0) );
		librock_SHA256_Init( (librock_SHA256_CTX*)this->MDData );
		if( Data != nullptr )
			{
			this->Update( Data, DataLength );
			}
		}

	SHA256::~SHA256()
		{
		free( this->MDData );
		}

	void SHA256::Update( const u8 *Data, size_t DataLength )
		{
		const u8 *End = &Data[DataLength];

		// run until end, in blocks of INT_MAX
		while( Data < End )
			{
			size_t data_len = End - Data;
			if( data_len > INT_MAX )
				data_len = INT_MAX;
			int int_data_len = (int)data_len;

			// update sha hash
			librock_SHA256_Update( (librock_SHA256_CTX *)this->MDData, Data, int_data_len );

			Data += int_data_len;
			}

		// done
		}

	// get the calculated digest 
	void SHA256::GetDigest( u8 DestDigest[32] )
		{
		librock_SHA256_StoreFinal( DestDigest, (librock_SHA256_CTX *)this->MDData );
		}
	}
