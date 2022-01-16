// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"

#include <map>
#include <stack>
#include <set>
#include <queue>

namespace ISD
	{
	struct DirectedGraphFlags 
		{
		static const uint Acyclic = 0x1; // if set, validation make sure the directed graph is acyclic (DAG)
		static const uint Rooted = 0x2; // if set, validation will make sure all graph vertices can be reachable from the root(s)
		static const uint SingleRoot = 0x4; // if set, validation will make sure there is a single graph root vertex
		static const uint UniqueEdges = 0x8; // if set, validation will make sure that there are only unique edges (key-value pairs) in the graph 
		};

	template<class _Ty, uint _Flags = 0, class _Alloc = std::allocator<std::pair<const _Ty, _Ty>>>
	class DirectedGraph
		{
		public:
			using key_type = _Ty;
			using mapped_type = _Ty;
			using allocator_type = _Alloc;

			using map_type = std::multimap<_Ty, _Ty, std::less<_Ty>, _Alloc>;
			using value_type = typename map_type::value_type;
			using iterator = typename map_type::iterator;

			static const bool type_acyclic = (_Flags & DirectedGraphFlags::Acyclic) != 0;
			static const bool type_rooted = (_Flags & DirectedGraphFlags::Rooted) != 0;
			static const bool type_single_root = (_Flags & DirectedGraphFlags::SingleRoot) != 0;
			static const bool type_unique_edges = (_Flags & DirectedGraphFlags::UniqueEdges) != 0;

			class MF;
			friend MF;

		private:
			std::set<_Ty> Roots = {};
			map_type Edges = {};

		public:
			// inserts an edge, but if flags is set to UniqueEdges, will first check if it already exists, and only insert if not found
			void InsertEdge( const key_type &key, const mapped_type &value ) noexcept;

			// find a particular key-value pair (directed edge)
			const bool HasEdge( const key_type &key, const mapped_type &value ) const noexcept;

			// direct access to edges structure
			map_type &GetEdges() noexcept { return this->Edges; }
			const map_type &GetEdges() const noexcept { return this->Edges; }

			// direct access to roots set
			std::set<_Ty> &GetRoots() noexcept { return this->Roots; }
			const std::set<_Ty> &GetRoots() const noexcept { return this->Roots; }
		};

	template<class _Ty, uint _Flags, class _Alloc>
	inline void DirectedGraph<_Ty, _Flags, _Alloc>::InsertEdge( const key_type &key, const mapped_type &value ) noexcept
		{
		if( type_unique_edges && HasEdge( key, value ) )
			return;
		this->Edges.emplace( key, value );
		}

	template<class _Ty, uint _Flags, class _Alloc>
	inline const bool DirectedGraph<_Ty,_Flags,_Alloc>::HasEdge( const key_type &key, const mapped_type &value ) const noexcept
		{
		auto itr = this->Edges.lower_bound( key );
		auto itr_end = this->Edges.upper_bound( key );
		while( itr != itr_end )
			{
			if( itr->second == value )
				return true;
			++itr;
			}
		return false;
		}

	class EntityWriter;
	class EntityReader;
	class EntityValidator;

	template<class _Ty, uint _Flags, class _Alloc>
	class DirectedGraph<_Ty,_Flags,_Alloc>::MF
		{
		using _MgmCl = DirectedGraph<_Ty,_Flags,_Alloc>;

		inline static bool set_contains( const std::set<_Ty> &set, const _Ty &val )
			{
			return set.find( val ) != set.end();
			}

		public:
			static bool Write( _MgmCl &obj , EntityWriter &writer )
				{
				// store the roots 
				std::vector<_Ty> roots( obj.Roots.begin(), obj.Roots.end() );
				if( !writer.Write( ISDKeyMacro("Roots"), roots ) )
					return false;

				// collect the keys-value pairs into a vector and store as an array
				std::vector<_Ty> graph_pairs(obj.Edges.size()*2);
				size_t index = 0;
				for( auto it = obj.Edges.begin(); it != obj.Edges.end(); ++it, ++index )
					{
					graph_pairs[index*2+0] = it->first;
					graph_pairs[index*2+1] = it->second;
					}
				if( !writer.Write( ISDKeyMacro("Edges"), graph_pairs ) )
					return false;

				// sanity check, make sure all sections were written
				ISDSanityCheckDebugMacro( index == obj.Edges.size() );

				return true;
				}

			static bool Read( _MgmCl &obj , EntityReader &reader )
				{
				size_t map_size = {};
				bool success = {};
				typename _MgmCl::iterator it = {};

				// read the roots 
				std::vector<_Ty> roots;
				if( !reader.Read( ISDKeyMacro("Roots"), roots ) )
					return false;
				obj.Roots = std::set<_Ty>(roots.begin(), roots.end());
				
				// read in the graph pairs
				std::vector<_Ty> graph_pairs;
				if( !reader.Read( ISDKeyMacro("Edges"), graph_pairs ) )
					return false;
				
				// insert into map
				obj.Edges.clear();
				map_size = graph_pairs.size() / 2;
				for( size_t index = 0; index < map_size; ++index )
					{
					it = obj.Edges.emplace( graph_pairs[index * 2 + 0], graph_pairs[index * 2 + 1] );
					if( it == obj.Edges.end() )
						{
						ISDErrorLog << "Failed inserting key-value pair into DirectedGraph" << ISDErrorLogEnd;
						return false;
						}
					}

				return true;
				}

		private:
			static void ValidateNoCycles( const _MgmCl::map_type &graph, EntityValidator &validator )
				{
				// Do a depth-first search, find all nodes starting from the root nodes 
				// Note: Only reports first found cycle, if any
				std::stack<_Ty> stack;
				std::set<_Ty> on_stack;
				std::set<_Ty> checked;

				// try all nodes
				for( const auto &p : graph )
					{
					const _Ty &node = p.first;

					// if already checked, skip
					if( set_contains( checked, node ) )
						continue;

					// push on to stack
					stack.push( node );

					// run until all items on stack are popped again
					while( !stack.empty() )
						{
						// get the top from the stack
						_Ty curr = stack.top();

						if( !set_contains( checked, curr ) )
							{
							// if we havent checked, mark the node as on the stack
							checked.insert( curr );
							on_stack.insert( curr );
							}
						else
							{
							// we are done with it, remove from stack
							on_stack.erase( curr );
							stack.pop();
							}

						// check nodes downstream from this
						auto itr = graph.lower_bound( curr );
						auto itr_end = graph.upper_bound( curr );
						while( itr != itr_end )
							{
							ISDSanityCheckCoreDebugMacro( itr->first == curr );
							const _Ty &child = itr->second;

							if( !set_contains( checked, child ) )
								{
								// this has not been checked, add on top of stack to be checked next
								stack.push( child );
								}
							else if( set_contains( on_stack, child ) )
								{
								// This child node is already marked on the stack, so we have already visited it once 
								// We have a cycle, report it, and return
								ISDValidationError( ValidationError::InvalidSetup )
									<< "The node " << child << " in Graph is a part of a cycle, but the graph is acyclic."
									<< ISDValidationErrorEnd;
								return;
								}

							++itr;
							}
						}
					}

				// no cycles found, all good
				}
			
			static void ValidateRooted( const std::set<_Ty> &roots , const std::set<_Ty> &downstream_nodes , const _MgmCl::map_type &graph, EntityValidator &validator )
				{
				std::queue<_Ty> queue;
				std::set<_Ty> reached;

				// push all the roots onto the queue
				for( const auto &n : roots )
					{
					queue.push( n );
					}

				// try to reach all downstream nodes from the roots
				while( !queue.empty() )
					{
					// get the first value in the queue
					_Ty curr = queue.front();
					queue.pop();

					// if already reached, skip
					if( set_contains( reached, curr ) )
						continue;

					// mark it as reached
					reached.insert( curr );

					// check downstream nodes
					auto itr = graph.lower_bound( curr );
					auto itr_end = graph.upper_bound( curr );
					while( itr != itr_end )
						{
						ISDSanityCheckCoreDebugMacro( itr->first == curr );
						const _Ty &child = itr->second;

						if( !set_contains( reached, child ) )
							{
							queue.push( child );
							}

						++itr;
						}
					}

				// make sure all downstream nodes were reached
				for( auto n : downstream_nodes )
					{
					if( !set_contains( reached, n ) )
						{
						// This child node is already marked on the stack, so we have already visited it once 
						// We have a cycle, report it, and return
						ISDValidationError( ValidationError::InvalidSetup )
							<< "The node " << n << " in Graph could not be reached from (any of) the root(s) in the Roots set."
							<< ISDValidationErrorEnd;
						}
					}
				}

		public:
			static bool Validate( const _MgmCl &obj, EntityValidator &validator )
				{
				// make a set of all nodes with incoming edges
				std::set<_Ty> downstream_nodes;
				for( const auto &p : obj.Edges )
					{
					if( !set_contains( downstream_nodes , p.second ) )
						downstream_nodes.insert( p.second );
					}

				// the rest of the nodes are root nodes (no incoming edges)
				std::set<_Ty> root_nodes;
				for( const auto &p : obj.Edges )
					{
					if( !set_contains( downstream_nodes , p.first ) )
						root_nodes.insert( p.first );
					}

				// check for single root object
				if( type_single_root )
					{
					if( root_nodes.size() != 1 )
						{
						ISDValidationError( ValidationError::InvalidCount ) << "The number of roots found when searching through the graph is " << root_nodes.size() << " but the graph is required to have exactly one root." << ISDValidationErrorEnd;
						}
					}

				// if this is a rooted graph, make sure that the Roots set is correct
				if( type_rooted )
					{
					if( type_single_root )
						{
						if( obj.Roots.size() != 1 )
							{
							ISDValidationError( ValidationError::InvalidCount ) << "The graph is single rooted, but the Roots set has " << obj.Roots.size() << " nodes. The Roots set must have exactly one node." << ISDValidationErrorEnd;
							}
						}

					// make sure that all nodes in the Roots list do not have incoming edges
					for( auto n : obj.Roots )
						{
						if( set_contains( downstream_nodes , n ) )
							{
							ISDValidationError( ValidationError::InvalidObject )
								<< "Node " << n << " in the Roots set has incoming edges, which makes it invalid as a root node."
								<< ISDValidationErrorEnd;
							}
						}

					// make sure that all nodes that are root nodes (no incoming edges) are in the Roots list
					for( auto n : root_nodes )
						{
						if( !set_contains( obj.Roots , n ) )
							{
							ISDValidationError( ValidationError::MissingObject )
								<< "Node " << n << " has no incoming edges, so is by definition a root, but is not listed in the Roots set."
								<< ISDValidationErrorEnd;
							}
						}

					// make sure no node is unreachable from the roots
					ValidateRooted( obj.Roots, downstream_nodes, obj.Edges, validator );
					}

				// check for cycles if the graph is acyclic
				if( type_acyclic )
					{
					ValidateNoCycles( obj.Edges, validator );
					}

				return true;
				}
		};


	};