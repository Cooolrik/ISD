// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "../ISD/ISD.h"
#include "../ISD/ISD_MemoryReadStream.h"
#include "../ISD/ISD_MemoryWriteStream.h"
#include "../ISD/ISD_Log.h"
#include "../ISD/ISD_EntityWriter.h"
#include "../ISD/ISD_EntityReader.h"
#include "../ISD/ISD_EntityValidator.h"

#include "../ISD/ISD_Registry.h"
#include "../ISD/ISD_DirectedGraph.h"

#include "../ISD/ISD_SceneLayer.h"
#include "../ISD/ISD_SHA256.h"

#include "../ISD/ISD_Mesh.h"

#include "../TestHelpers/random_vals.h"

//#include <fbxsdk.h>

#include <Rpc.h>
//#include "../ISD/ISD_Varying.h"

#include "../ISD/ISD_DynamicTypes.h"
#include "../ISD/ISD_CombinedTypes.h"
#include "../ISD/ISD_Varying.h"

#include <unordered_map>

extern void safe_thread_map_test();

using namespace ISD;


#define RUN_TEST( name )\
	printf("Running test: " #name "\n");\
	name();\
	printf(" - Test: " #name " done\n");
//
//enum RegistryFlags : uint
//	{
//	NoZeroKeys = 0x1, // if set, validation will fail if a key has a zero value (0 for ints, all 0 in a uuids, empty strings) 
//	NoNullEntities = 0x2, // if set, validation will fail if null entities exist in the dictionary
//	};
//
//template<class _Kty, class _Ty, uint _Flags = 0, class _Hsh = std::hash<_Kty>, class _Eq = std::equal_to<_Kty>, class _Alloc = std::allocator<std::pair<const _Kty, std::unique_ptr<_Ty>>>>
//class EntityTable
//	{
//	public:
//		using key_type = _Kty;
//		using mapped_type = _Ty;
//		using key_hash = _Hsh;
//		using key_equal = _Eq;
//		using allocator_type = _Alloc;
//
//		using map_type = std::unordered_map<_Kty, std::unique_ptr<_Ty>, _Hsh, _Eq, _Alloc>;
//		using value_type = typename map_type::value_type;
//		using iterator = typename map_type::iterator;
//
//		static const bool type_no_zero_keys = (_Flags & RegistryFlags::NoZeroKeys) != 0;
//		static const bool type_no_null_entities = (_Flags & RegistryFlags::NoNullEntities) != 0;
//
//		class MF;
//		friend MF;
//
//		// ctors/dtor and copy/move operators
//		EntityTable() = default;
//		EntityTable( const EntityTable &rval ) { }
//		EntityTable &operator=( const EntityTable &rval ) { return *this; }
//		EntityTable( EntityTable &&rval ) = default;
//		EntityTable &operator=( EntityTable &&rval ) = default;
//		~EntityTable() = default;
//
//	private:
//		map_type v_Entries;
//
//	public:
//		// returns the number of entries in the EntityTable
//		size_t Size() const noexcept { return this->v_Entries.size(); }
//
//		// direct access to the Entries map
//		map_type &Entries() noexcept { return this->v_Entries; }
//		const map_type &Entries() const noexcept { return this->v_Entries; }
//
//		// index access operator. node, dereferences the mapped value, so will throw if the value does not exist.
//		mapped_type &operator[]( const key_type &key ) { return *(this->v_Entries[key].get()); }
//		const mapped_type &operator[]( const key_type &key ) const { return *(this->v_Entries[key].get()); }
//
//		// insert a key and new empty value, returns reference to value
//		mapped_type &Insert( const key_type &key ) { this->v_Entries.emplace( key, std::make_unique<mapped_type>() ); return *(this->v_Entries[key].get()); }
//	};

int main()
	{
	ISD::EntityTable<ISD::package_ref, int> refint;

	for( uint i = 0; i < 20; ++i )
		{
		refint.Insert( random_value<ISD::package_ref>() ) = random_value<int>();
		}

	return 0;
	}

//
//bool LoadFile( std::string file_path , std::vector<u8> &allocation )
//	{
//	// open the file
//	HANDLE file_handle = ::CreateFileA( file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr );
//	if( file_handle == INVALID_HANDLE_VALUE )
//		{
//		// failed to open the file
//		return false;
//		}
//
//	// get the size
//	LARGE_INTEGER dfilesize = {};
//	if( !::GetFileSizeEx( file_handle, &dfilesize ) )
//		{
//		// failed to get the size
//		return false;
//		}
//	u64 total_bytes_to_read = dfilesize.QuadPart;
//
//	// read in all of the file
//	allocation.resize( total_bytes_to_read );
//	if( allocation.size() != total_bytes_to_read )
//		{
//		// failed to allocate the memory
//		return false;
//		}
//	u8 *buffer = allocation.data();
//
//	u64 bytes_read = 0;
//	while( bytes_read < total_bytes_to_read )
//		{
//		// check how much to read and cap each read at UINT_MAX
//		u64 bytes_left = total_bytes_to_read - bytes_read;
//		u32 bytes_to_read_this_time = UINT_MAX;
//		if( bytes_left < UINT_MAX )
//			bytes_to_read_this_time = (u32)bytes_left;
//
//		// read in bytes into the memory allocation
//		DWORD bytes_that_were_read = 0;
//		if( !::ReadFile( file_handle, &buffer[bytes_read], bytes_to_read_this_time, &bytes_that_were_read, nullptr ) )
//			{
//			// failed to read
//			return false;
//			}
//
//		// update number of bytes that were read
//		bytes_read += bytes_that_were_read;
//		}
//
//	::CloseHandle( file_handle );
//	return true;
//	}
//
//class named_object
//	{
//	public:
//		optional_value<std::string> object_name;
//	};
//
//void write_geometry()
//	{
//	idx_vector<vec3> *Vertices = new idx_vector<vec3>();
//
//	auto &values = Vertices->values();
//	auto &index = Vertices->index();
//
//	values.resize( 100000000 );
//	index.resize( 100000000 );
//	for( size_t i = 0; i < 100000000; ++i )
//		{
//		values[i] = vec3( i, i, i );
//		index[i] = (int)i;
//		}
//
//	MemoryWriteStream ws;
//	EntityWriter wr(ws);
//
//	wr.Write( "Vertices", 8, *Vertices );
//
//	delete Vertices;
//
//	FILE *fp;
//	if( fopen_s( &fp, "test_write.dat", "wb" ) == 0 )
//		{
//		fwrite( ws.GetData(), ws.GetSize(), 1, fp );
//		fclose( fp );
//		}
//	}
//
//void read_geometry()
//	{
//	std::vector<u8> allocation;
//	LoadFile( "test_write.dat", allocation );
//
//	MemoryReadStream rs(allocation.data(),allocation.size());
//	EntityReader er( rs );
//
//	idx_vector<vec3> DestVertices;
//
//	er.Read( "Vertices", 8, DestVertices );
//	}



	//ISD::Varying var;
	//
	//ISD::Varying::MF::SetType<bool>( var );
	//
	//bool & value = var.Data<bool>();
	//
	//ISD::Varying::MF::SetType<std::vector<int>>( var );
	//
	//std::vector<int> & value2 = var.Data<std::vector<int>>();

	//stde::unordered_bimap<int, std::string> bh;
	//
	//bh.insert( 2, "3" );
	//bh.insert( 3, "2" );
	//
	//int key = bh.get_key( "3" );
	//std::string value = bh.get_value( 2 );
	//
	//return 0;

	//ISD::optional_vector<bool> value;
	//value.set( {true, false, false} );
	//clear_combined_type( value );
	//
	//void *ptr;
	//bool ret;
	//
	//constexpr ISD::data_type_index mytype = combined_type_information<std::vector<bool>>::type_index;
	//constexpr ISD::container_type_index mycont = combined_type_information<std::vector<bool>>::container_index;
	//
	//std::tie(ptr,ret) = ISD::dynamic_types::new_type( mytype , mycont );
	//std::vector<bool> *vec = (std::vector<bool>*)ptr;
	//std::vector<bool> vec2;
	//
	//vec->resize( 100 , true );
	//ISD::dynamic_types::copy( ISD::data_type_index::dt_bool, ISD::container_type_index::ct_vector, &vec2, vec );
	
	//ISD::Mesh mesh;
	//ISD::entity_ref ref = ISD::entity_ref::make_ref();
	//
	//size_t v = mesh.BitangentsData().Size();
	//
	//auto &uv = mesh.TextureCoordsData().Insert( ref );
	//auto &tan = mesh.TangentsData().Insert( ref );
	//auto &btan = mesh.BitangentsData().Insert( ref );
	//
	//auto &cust = mesh.CustomData().Insert( ref );
	//
	//auto &vec = cust.Initialize<std::vector<fvec4>>();
	//vec.resize( 100 );
	//auto vecd = vec.data();
	//
	//v = mesh.BitangentsData().Size();
	//
	//
	//btan.index().resize( 1 );
	//btan.values().resize( 1 );
	//btan.index()[0] = 0;
	//
	//if( tan == btan )
	//	{
	//	printf( "hej" );
	//	}
	//
	//EntityValidator validator;
	//
	//Mesh::MF::Validate( mesh, validator );

	//DynamicValueTester<bool>();
	//DynamicValueTester<float>();

	//ISD::Varying var;
	//
	//auto &data = ISD::Varying::MF::SetType<optional_idx_vector<fvec4>>( var );
	//
	//data.set();
	//data.values().resize( 1000 );
	//data.values()[4].x = 12.f;
	//
	//ISD::Varying::MF::SetType<fvec3>( var );
	//auto &data2 = var.Data<fvec3>();
	//
	//data2.x = 78;
	//
	//if( var.IsA<optional_idx_vector<fvec4>>() )
	//	{
	//	const auto &data3 = var.Data<optional_idx_vector<fvec4>>();
	//	}


	
	//optional_value<std::string> test = std::string("hej");
	//optional_value<std::string> test2;
	//optional_value<std::string> test3;
	//
	//test2 = test;
	//test = test3;

	//EntityTable<entity_ref, SceneMesh> dict;
	//
	//auto id = entity_ref();
	//auto mesh = std::make_unique<SceneMesh>();
	//
	//dict.Entries().emplace( id , std::move(mesh) );
	//dict.Entries().emplace( entity_ref() , std::make_unique<SceneMesh>() );
	//
	//uuid meshid = dict.Entries()[id]->MeshPacketId();
	//
	//dict.Entries().erase( id );
	//
	//std::cout << meshid << std::endl;

	//ISD::SceneLayer layer;
	//
	//ISD::hash h = ISD::hash_sup; 
	//
	//bool b;
	//
	//b = (ISD::hash_sup == ISD::hash_inf);
	//b = ISD::uuid_sup == ISD::uuid_inf;
	//
	//std::vector<u8> randomvec;
	//random_vector( randomvec, 1000000000, 1000000000 );
	//
	//for( uint i = 0; i < 10; ++i )
	//	{
	//	SHA256 sha1;
	//	u8 digest1[33];
	//	sha1.Update( randomvec.data(), randomvec.size() );
	//	sha1.GetDigest( digest1 );
	//	digest1[32] = 0;
	//	printf( "%s\n", digest1 );
	//
	//	SHA256 sha2;
	//	u8 digest2[33];
	//	sha2.Update( randomvec.data(), randomvec.size() );
	//	sha2.GetDigest( digest2 );
	//	digest2[32] = 0;
	//	printf( "%s\n", digest2 );
	//	}

	//ISD::SceneNode node;
	//
	//node.Name() = "Nej";
	//
	//layer.Nodes().Entries().emplace( id, new ISD::SceneNode(node) );
	//
	//MemoryWriteStream ws;
	//EntityWriter writer(ws);
	//
	//SceneLayer::MF::Write( layer, writer );
	//
	//ISD::SceneLayer layer2;
	//
	//MemoryReadStream rs( ws.GetData(), ws.GetSize() );
	//EntityReader reader(rs);
	//
	//SceneLayer::MF::Read( layer2, reader );
	//
	//EntityValidator validator;
	//
	//SceneLayer::MF::Validate( layer2, validator );
	//
	//bool cmp = SceneLayer::MF::Equals( &layer, &layer2 );

	//
	//MemoryWriteStream ws;
	//EntityWriter writer(ws);
	//
	//DirectedGraph<int>::MF::Write( dg, writer );
	//
	//DirectedGraph<int> dg_read;
	//
	//
	//
	//
	//DirectedGraph<int>:


