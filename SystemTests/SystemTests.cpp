// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#include "../ISD/ISD.h"
#include "../ISD/ISD_MemoryReadStream.h"
#include "../ISD/ISD_MemoryWriteStream.h"
#include "../ISD/ISD_Log.h"
#include "../ISD/ISD_EntityWriter.h"
#include "../ISD/ISD_EntityReader.h"
#include "../ISD/ISD_EntityValidator.h"

#include "../ISD/ISD_Dictionary.h"
#include "../ISD/ISD_DirectedGraph.h"

#include "../ISD/ISD_SceneLayer.h"

#include <Rpc.h>
extern void safe_thread_map_test();

#define RUN_TEST( name )\
	printf("Running test: " #name "\n");\
	name();\
	printf(" - Test: " #name " done\n");

using namespace ISD;

bool LoadFile( std::string file_path , std::vector<u8> &allocation )
	{
	// open the file
	HANDLE file_handle = ::CreateFileA( file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr );
	if( file_handle == INVALID_HANDLE_VALUE )
		{
		// failed to open the file
		return false;
		}

	// get the size
	LARGE_INTEGER dfilesize = {};
	if( !::GetFileSizeEx( file_handle, &dfilesize ) )
		{
		// failed to get the size
		return false;
		}
	u64 total_bytes_to_read = dfilesize.QuadPart;

	// read in all of the file
	allocation.resize( total_bytes_to_read );
	if( allocation.size() != total_bytes_to_read )
		{
		// failed to allocate the memory
		return false;
		}
	u8 *buffer = allocation.data();

	u64 bytes_read = 0;
	while( bytes_read < total_bytes_to_read )
		{
		// check how much to read and cap each read at UINT_MAX
		u64 bytes_left = total_bytes_to_read - bytes_read;
		u32 bytes_to_read_this_time = UINT_MAX;
		if( bytes_left < UINT_MAX )
			bytes_to_read_this_time = (u32)bytes_left;

		// read in bytes into the memory allocation
		DWORD bytes_that_were_read = 0;
		if( !::ReadFile( file_handle, &buffer[bytes_read], bytes_to_read_this_time, &bytes_that_were_read, nullptr ) )
			{
			// failed to read
			return false;
			}

		// update number of bytes that were read
		bytes_read += bytes_that_were_read;
		}

	::CloseHandle( file_handle );
	return true;
	}

class named_object
	{
	public:
		optional_value<std::string> object_name;
	};

void write_geometry()
	{
	idx_vector<vec3> *Vertices = new idx_vector<vec3>();

	auto &values = Vertices->values();
	auto &index = Vertices->index();

	values.resize( 100000000 );
	index.resize( 100000000 );
	for( size_t i = 0; i < 100000000; ++i )
		{
		values[i] = vec3( i, i, i );
		index[i] = (int)i;
		}

	MemoryWriteStream ws;
	EntityWriter wr(ws);

	wr.Write( "Vertices", 8, *Vertices );

	delete Vertices;

	FILE *fp;
	if( fopen_s( &fp, "test_write.dat", "wb" ) == 0 )
		{
		fwrite( ws.GetData(), ws.GetSize(), 1, fp );
		fclose( fp );
		}
	}

void read_geometry()
	{
	std::vector<u8> allocation;
	LoadFile( "test_write.dat", allocation );

	MemoryReadStream rs(allocation.data(),allocation.size());
	EntityReader er( rs );

	idx_vector<vec3> DestVertices;

	er.Read( "Vertices", 8, DestVertices );
	}

//
//template<class Graph>
//void GenerateRandomTreeRecursive( Graph &graph , uint total_levels , uint current_level = 0, typename Graph::node_type parent_node = random_value<Graph::node_type>() )
//	{
//	typedef Graph::node_type _Ty;
//
//	// generate a random number of subnodes
//	size_t sub_nodes = capped_rand( 1, 7 );
//
//	// add to tree
//	for( size_t i = 0; i < sub_nodes; ++i )
//		{
//		_Ty child_node = random_value<Graph::node_type>();
//
//		graph.GetEdges().insert( std::pair<_Ty, _Ty>( parent_node, child_node ) );
//
//		if( current_level < total_levels )
//			{
//			GenerateRandomTreeRecursive( graph, total_levels, current_level + 1, child_node );
//			}
//		}
//	}

int main()
	{
	ISD::SceneLayer layer;
	
	uuid id = {};

	ISD::SceneNode node;

	node.Name() = "Nej";

	layer.Nodes().Entities().emplace( id, new ISD::SceneNode(node) );

	MemoryWriteStream ws;
	EntityWriter writer(ws);

	SceneLayer::MF::Write( layer, writer );

	ISD::SceneLayer layer2;

	MemoryReadStream rs( ws.GetData(), ws.GetSize() );
	EntityReader reader(rs);

	SceneLayer::MF::Read( layer2, reader );

	//EntityValidator validator;

	//Graph::MF::Validate( dg, validator );

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

	return 0;
	}

