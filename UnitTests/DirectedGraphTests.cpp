// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\ISD\ISD_MemoryReadStream.h"
#include "..\ISD\ISD_MemoryWriteStream.h"
#include "..\ISD\ISD_EntityWriter.h"
#include "..\ISD\ISD_EntityReader.h"
#include "..\ISD\ISD_DirectedGraph.h"
#include "..\ISD\ISD_EntityValidator.h"

namespace UnitTests
	{
	TEST_CLASS( DirectedGraphTests )
		{
		template<class Graph>
		void GenerateRandomTreeRecursive( Graph &graph , uint total_levels , uint current_level = 0, typename Graph::node_type parent_node = random_value<Graph::node_type>() )
			{
			typedef Graph::node_type _Ty;

			// generate a random number of subnodes
			size_t sub_nodes = capped_rand( 1, 7 );

			// add to tree
			for( size_t i = 0; i < sub_nodes; ++i )
				{
				_Ty child_node = random_value<Graph::node_type>();

				graph.GetEdges().insert( std::pair<_Ty, _Ty>( parent_node, child_node ) );

				if( current_level < total_levels )
					{
					GenerateRandomTreeRecursive( graph, total_levels, current_level + 1, child_node );
					}
				}
			}

		TEST_METHOD( DirectedGraphBasicTest )
			{
			setup_random_seed();

			// create basic graph
			typedef DirectedGraph<int,0> Graph;
			Graph dg;

			dg.GetEdges().emplace(0,1);
			dg.GetEdges().emplace(1,2);
			dg.GetEdges().emplace(2,3);
			dg.GetEdges().emplace(3,1);

			EntityValidator validator;
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() == 0 );
			}

		TEST_METHOD( DirectedGraphDuplicateEdgesTest )
			{
			setup_random_seed();

			// create basic graph
			typedef DirectedGraph<int,0> Graph;
			Graph dg;

			// create two identical edges in the graph
			dg.InsertEdge(0,1);
			dg.InsertEdge(0,1);

			// the second edge should never be added
			Assert::IsTrue( dg.GetEdges().size() == 1 );

			// make sure this is invalid
			EntityValidator validator;
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() == 0 );
			}

		TEST_METHOD( DirectedGraphAcyclicTest )
			{
			setup_random_seed();

			typedef DirectedGraph<i64,DirectedGraphFlags::Acyclic> Graph;

			// create a tree, which by definition does not have cycles
			Graph dg;
			GenerateRandomTreeRecursive( dg, 3 );

			// make sure this is valid
			EntityValidator validator;
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() == 0 );

			// now, insert a cycle into the tree
			dg.GetEdges().emplace(0,1);
			dg.GetEdges().emplace(1,2);
			dg.GetEdges().emplace(2,0);

			// make sure this is not valid anymore, and that the error is the cycle
			validator.ClearErrorCount();
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() != 0 );
			Assert::IsTrue( validator.GetErrorIds() == ValidationError::InvalidSetup );
			}

		TEST_METHOD( DirectedGraphSingleRootTest )
			{
			setup_random_seed();

			typedef DirectedGraph<i64,DirectedGraphFlags::SingleRoot> Graph;

			// create a tree, which by definition only has one root
			Graph dg;
			GenerateRandomTreeRecursive( dg, 2 );

			// make sure this is valid
			EntityValidator validator;
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() == 0 );

			// now, insert a second root into the tree, by adding two random nodes
			dg.GetEdges().emplace(random_value<i64>(),random_value<i64>());

			// make sure this is not valid anymore, and that the error is multiple roots
			validator.ClearErrorCount();
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() != 0 );
			Assert::IsTrue( validator.GetErrorIds() == ValidationError::InvalidCount );
			}

		TEST_METHOD( DirectedGraphRootedTest )
			{
			setup_random_seed();

			typedef DirectedGraph<i64,DirectedGraphFlags::Rooted> Graph;

			// create a forest of trees with multiple roots
			Graph dg;
			size_t roots = capped_rand( 1, 9 );
			for( size_t i = 0; i < roots; ++i )
				{
				i64 rootid = random_value<i64>();

				dg.GetRoots().insert( rootid );
				GenerateRandomTreeRecursive( dg, 2, 0, rootid );
				}

			// make sure this is valid
			EntityValidator validator;
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() == 0 );

			// now, remove the first root in the roots list
			dg.GetRoots().erase( dg.GetRoots().begin() );

			// make sure this is not valid anymore, and that the error the missing root node in the Roots list
			validator.ClearErrorCount();
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() != 0 );
			Assert::IsTrue( validator.GetErrorIds() == (ValidationError::InvalidSetup|ValidationError::MissingObject) );
			}

		TEST_METHOD( DirectedGraphSceneGraphTest )
			{
			setup_random_seed();

			// single root, acyclic, with root defined in the Roots list
			typedef DirectedGraph<uuid,(DirectedGraphFlags::Acyclic|DirectedGraphFlags::Rooted|DirectedGraphFlags::SingleRoot)> Graph;

			// create a tree, which by definition does not have cycles, a single root, and add the root to the roots list
			Graph dg;
			uuid root_node = random_value<uuid>();
			dg.GetRoots().insert( root_node );
			GenerateRandomTreeRecursive( dg, 3, 0, root_node );

			// get a set of all downstream nodes
			std::set<Graph::node_type> downstream_nodes;
			for( const auto &p : dg.GetEdges() )
				{
				if( downstream_nodes.find( p.second ) == downstream_nodes.end() )
					downstream_nodes.insert( p.second );
				}

			// make all downstream nodes point at a single leaf node 
			uuid leaf_node = random_value<uuid>();
			for( auto p : downstream_nodes )
				{
				dg.GetEdges().insert( std::pair<uuid, uuid>( p, leaf_node ) );
				}

			// make sure this is valid (no cycles)
			EntityValidator validator;
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() == 0 );

			// add two new roots, insert a cycle (leaf node points at original root, which is no longer a root), dont add the new roots to the root list, and add two identical edges to the graph
			dg.GetEdges().insert( std::pair<uuid, uuid>( random_value<uuid>(), root_node ) );
			dg.GetEdges().insert( std::pair<uuid, uuid>( random_value<uuid>(), root_node ) );
			dg.GetEdges().insert( std::pair<uuid, uuid>( leaf_node, root_node ) );
			dg.GetEdges().insert( std::pair<uuid, uuid>( leaf_node, root_node ) );

			// make sure this is not valid anymore, and that the error the missing root node in the Roots list
			validator.ClearErrorCount();
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() != 0 );
			const u64 expected_error = (
				  ValidationError::InvalidSetup
				| ValidationError::MissingObject
				| ValidationError::InvalidObject
				| ValidationError::InvalidCount
				);
			Assert::IsTrue( validator.GetErrorIds() == expected_error );
			}

		template<class _Ty, uint _Flags>
		void ReadWriteTest( MemoryWriteStream &ws , EntityWriter &ew )
			{
			typedef DirectedGraph<_Ty,_Flags> Graph;

			// generate a tree, one or multiple roots 
			Graph dg;
			size_t roots = (Graph::type_single_root) ? 1 : capped_rand( 0, 9 );
			for( size_t i = 0; i < roots; ++i )
				{
				_Ty rootid = random_value<_Ty>();
				dg.GetRoots().insert( rootid );
				GenerateRandomTreeRecursive( dg, 2, 0, rootid );
				}

			// store to file
			u64 start_pos = ws.GetPosition();
			Assert::IsTrue( Graph::MF::Write( dg, ew ) );

			// read from file
			MemoryReadStream rs( ws.GetData(), ws.GetSize(), ws.GetFlipByteOrder() );
			EntityReader er( rs );
			rs.SetPosition( start_pos );

			// read back the graph 
			Graph readback_dg;
			Assert::IsTrue( Graph::MF::Read( readback_dg , er ) );

			// compare values
			Assert::IsTrue( dg.GetEdges() == readback_dg.GetEdges() );
			}

		template<class _Ty>
		void ReadWriteTypeTest( MemoryWriteStream &ws, EntityWriter &ew )
			{
			// all combinations of Acyclic (0x1), Rooted (0x2), and Single root (0x4)
			ReadWriteTest<_Ty, 0x0>( ws, ew );
			ReadWriteTest<_Ty, 0x1>( ws, ew );
			ReadWriteTest<_Ty, 0x2>( ws, ew );
			ReadWriteTest<_Ty, 0x3>( ws, ew );
			ReadWriteTest<_Ty, 0x4>( ws, ew );
			ReadWriteTest<_Ty, 0x5>( ws, ew );
			ReadWriteTest<_Ty, 0x6>( ws, ew );
			ReadWriteTest<_Ty, 0x7>( ws, ew );
			}

		TEST_METHOD( DirectedGraphSerializeTest )
			{
			setup_random_seed();

			for( uint pass_index=0; pass_index<(2*global_number_of_passes); ++pass_index )
				{
				MemoryWriteStream ws;
				EntityWriter ew( ws );

				ws.SetFlipByteOrder( (pass_index & 0x1) != 0 );

				// log the pass
				std::stringstream ss;
				ss << "Pass #" << (pass_index / 2)+1 << " ";
				if( ws.GetFlipByteOrder() )
					ss << "Testing flipped byte order\n";
				else
					ss << "Testing native byte order\n";
				Logger::WriteMessage(ss.str().c_str());

				ReadWriteTypeTest<int>( ws, ew );
				ReadWriteTypeTest<uint>( ws, ew );
				ReadWriteTypeTest<i64>( ws, ew );
				ReadWriteTypeTest<string>( ws, ew );
				ReadWriteTypeTest<uuid>( ws, ew );
				}
			}
		};
	}