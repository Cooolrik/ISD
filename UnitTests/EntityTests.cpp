// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\ISD\ISD_MemoryReadStream.h"
#include "..\ISD\ISD_MemoryWriteStream.h"
#include "..\ISD\ISD_EntityWriter.h"
#include "..\ISD\ISD_EntityReader.h"
#include "..\ISD\ISD_Dictionary.h"
#include "..\ISD\ISD_EntityValidator.h"

#include "..\TestHelpers\structure_generation.h"

namespace UnitTests
	{

	TEST_CLASS( EntityTests )
		{
		TEST_METHOD( EntityManagementBasicTests )
			{
			TestEntity ent1;
			TestEntity ent2;
			ISD::EntityValidator validator;

			// set a value, they should differ
			ent1.Name() = random_value<string>();
			Assert::IsTrue( !TestEntity::MF::Equals( &ent1, &ent2 ) );

			// clear values, they should be the same
			TestEntity::MF::Clear( ent1 );
			Assert::IsTrue( TestEntity::MF::Equals( &ent1, &ent2 ) );

			// set values, validate, should be fine
			ent1.Name() = random_value<string>();
			ent1.OptionalText().set(random_value<string>());
			Assert::IsTrue( TestEntity::MF::Validate( ent1 , validator ) );
			Assert::IsTrue( validator.GetErrorCount() == 0 );

			// set random values on ent2, should differ
			ent2.Name() = random_value<string>();
			ent2.OptionalText().set(random_value<string>());
			Assert::IsTrue( !TestEntity::MF::Equals( &ent1, &ent2 ) );

			// copy ent1 to ent2, compare, should be the same
			ent2 = ent1;
			Assert::IsTrue( TestEntity::MF::Equals( &ent1, &ent2 ) );

			// reset the OptionalText on ent2, copy to ent1
			ent2.OptionalText().reset();
			ent1 = ent2;
			Assert::IsTrue( TestEntity::MF::Equals( &ent1, &ent2 ) );

			// clear Name on ent1
			ent1.Name() = {};
			ent2 = ent1;
			Assert::IsTrue( TestEntity::MF::Equals( &ent1, &ent2 ) );

			// clear ent2, should be the same
			TestEntity::MF::Clear( ent2 );
			Assert::IsTrue( TestEntity::MF::Equals( &ent1, &ent2 ) );
			}

		};
	}
