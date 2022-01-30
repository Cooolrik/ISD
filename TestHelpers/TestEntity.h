
#include "..\ISD\ISD_Types.h"

#include <string>
#include <vector>
#include <set>

#include <glm/glm.hpp>

#include "..\ISD\ISD_Dictionary.h"
#include "..\ISD\ISD_DirectedGraph.h"

class TestEntity
	{
	ISD::string Name;
	ISD::optional_value<ISD::string> OptionalText;

	public:
		class MF;
		friend MF;

		bool operator==( const TestEntity &other ) const { return this->Name == other.Name; }
		bool operator!=( const TestEntity &other ) const { return this->Name != other.Name; }

		void SetName( std::string &name ) { this->Name = name; }
		std::string GetName() { return this->Name; }
	};

class TestEntity::MF
	{
	using _MgmCl = TestEntity;

	public:
		static void Clear( _MgmCl &obj  )
			{
			obj.Name = string();
			obj.OptionalText.reset();
			}

		static void DeepCopy( _MgmCl &dest, const _MgmCl *source )
			{
			Clear( dest );
			if( source )
				{
				dest.Name = source->Name;
				if( source->OptionalText.has_value() )
					dest.OptionalText.set( source->OptionalText.value() );
				}
			}

		static bool Equals( const _MgmCl *lval, const _MgmCl *rval )
			{
			if( lval == rval )
				return true;
			if( !lval || !rval )
				return false;
			if( lval->Name != rval->Name )
				return false;
			if( lval->OptionalText.has_value() != rval->OptionalText.has_value() )
				return false;
			if( lval->OptionalText.has_value() )
				{
				if( lval->OptionalText.value() != rval->OptionalText.value() )
					return false;
				}
			return true;
			}

		static bool Write( _MgmCl &obj , EntityWriter &writer )
			{
			if( !writer.Write( ISDKeyMacro( "Name" ), obj.Name ) )
				return false;
			if( !writer.Write( ISDKeyMacro( "OptionalText" ), obj.OptionalText ) )
				return false;
			return true;
			}

		static bool Read( _MgmCl &obj , EntityReader &reader )
			{
			if( !reader.Read( ISDKeyMacro( "Name" ), obj.Name ) )
				return false;
			if( !reader.Read( ISDKeyMacro( "OptionalText" ), obj.OptionalText ) )
				return false;
			return true;
			}

		static bool Validate( _MgmCl &obj , EntityValidator &validator )
			{
			return true;
			}
	};
