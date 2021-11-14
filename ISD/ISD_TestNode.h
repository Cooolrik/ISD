// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

// Note: This is a test file for development, and will be replaced with a generated file in the future

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ISD_Types.h"
#include "ISD_Entity.h"

namespace ISD
	{
	namespace ver1
			{
			class Transformation
				{
				public:
					glm::mat4 translation = {};
					glm::mat4 rotation = {};

				public:
					const glm::mat4 &GetTranslation() const { return translation; }
					const glm::mat4 &GetRotation() const { return rotation; }
				};

			class TestNode
				{
				protected:
					optional_value<std::string> name;
					optional_value<int> id;
					optional_value<Transformation> transformation;

				public:
					TestNode()
						{
						//this->Name.Set( "Name" );
						this->id.Set( -45 );
						this->transformation.Set( Transformation() );
						}

					//const std::string &GetName() const
					//	{
					//	return name.Value().second;
					//	}

					//const Transformation &GetTransformation() const
					//	{
					//	return this->transformation.Value().second;
					//	}

					//virtual std::pair<bool, ValueType> Reflection_GetValueType( const char *Key ) const
					//	{
					//	if( strcmp( Key, "Name" ) == 0 )
					//		return std::pair<bool, ValueType>( true, ValueType::VT_String );
					//	return std::pair<bool, ValueType>( false, ValueType::VT_Null );
					//	}

					//virtual std::vector<std::string> Reflection_ListKeys() const
					//	{
					//	std::vector<std::string> ret;
					//	ret.push_back( "Name" );
					//	return ret;
					//	}
				};

			class TestNodeBuilder : public TestNode
				{
				public:
					//void ClearTransformation()
					//	{
					//	this->transformation.Clear();
					//	}
					//
					//void SetTransformation( const glm::mat4 &value )
					//	{
					//	this->transformation.Set( value );
					//	}


				};



			};
	};
