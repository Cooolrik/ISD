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
		void GenerateRandomTreeRecursive( Graph &graph , uint total_levels , uint current_level = 0, typename Graph::key_type parent_node = random_value<Graph::key_type>() )
			{
			typedef Graph::key_type _Ty;

			// generate a random number of subnodes
			size_t sub_nodes = capped_rand( 1, 7 );

			// add to tree
			for( size_t i = 0; i < sub_nodes; ++i )
				{
				_Ty child_node = random_value<Graph::key_type>();

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
			dg.GetEdges().emplace(0,1);
			dg.GetEdges().emplace(0,1);

			// make sure this is invalid
			EntityValidator validator;
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() != 0 );
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
			typedef DirectedGraph<UUID,(DirectedGraphFlags::Acyclic|DirectedGraphFlags::Rooted|DirectedGraphFlags::SingleRoot)> Graph;

			// create a tree, which by definition does not have cycles, a single root, and add the root to the roots list
			Graph dg;
			UUID root_node = random_value<UUID>();
			dg.GetRoots().insert( root_node );
			GenerateRandomTreeRecursive( dg, 3, 0, root_node );

			// get a set of all downstream nodes
			std::set<Graph::key_type> downstream_nodes;
			for( auto p : dg.GetEdges() )
				{
				if( downstream_nodes.find( p.second ) == downstream_nodes.end() )
					downstream_nodes.insert( p.second );
				}

			// make all downstream nodes point at a single leaf node 
			UUID leaf_node = random_value<UUID>();
			for( auto p : downstream_nodes )
				{
				dg.GetEdges().insert( std::pair<UUID, UUID>( p, leaf_node ) );
				}

			// make sure this is valid (no cycles)
			EntityValidator validator;
			Graph::MF::Validate( dg, validator );
			Assert::IsTrue( validator.GetErrorCount() == 0 );

			// add two new roots, insert a cycle (leaf node points at original root, which is no longer a root), dont add the new roots to the root list, and add two identical edges to the graph
			dg.GetEdges().insert( std::pair<UUID, UUID>( random_value<UUID>(), root_node ) );
			dg.GetEdges().insert( std::pair<UUID, UUID>( random_value<UUID>(), root_node ) );
			dg.GetEdges().insert( std::pair<UUID, UUID>( leaf_node, root_node ) );
			dg.GetEdges().insert( std::pair<UUID, UUID>( leaf_node, root_node ) );

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


		};
	}
