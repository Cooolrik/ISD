// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

// Note: This is a test file for development, and will be replaced with a generated file in the future

#pragma once

#include "ISD_Types.h"
#include "ISD_Entity.h"
#include "ISD_EntityWriter.h"
#include "ISD_EntityReader.h"
#include "ISD_EntityValidator.h"

#include "ISD_Dictionary.h"
#include "ISD_DirectedGraph.h"

namespace ISD 
	{
	namespace ver1
		{
		//using SceneGraph = DirectedGraph<uuid, DirectedGraphFlags::Acyclic | DirectedGraphFlags::Rooted>;

		//class SceneNode
		//	{
		//	public:
		//		std::string name;
		//		glm::mat4 translation = {};
		//		glm::mat4 rotation = {};

		//		class MF;
		//		friend MF;

		//	public:
		//		const std::string &GetName() const { return name; }
		//		const glm::mat4 &GetTranslation() const { return translation; }
		//		const glm::mat4 &GetRotation() const { return rotation; }
		//	};

		//class SceneNode::MF
		//	{
		//	public:
		//		static bool Write( SceneNode &obj, EntityWriter &writer )
		//			{
		//			if( !writer.Write<string>( ISDKeyMacro( "Name" ) , obj.name ) )
		//				return false;
		//			if( !writer.Write<mat4>( ISDKeyMacro( "Translation" ) , obj.translation ) )
		//				return false;
		//			if( !writer.Write<mat4>( ISDKeyMacro( "Rotation" ) , obj.rotation ) )
		//				return false;
		//			return true;
		//			}
		//	};

		//class SceneMesh
		//	{
		//	public:
		//		uuid meshId;

		//		class MF;
		//		friend MF;

		//	public:
		//		const uuid &GetMeshId() const { return meshId; }
		//	};

		//class SceneMesh::MF
		//	{
		//	public:
		//		static bool Write( SceneMesh &obj, EntityWriter &writer )
		//			{
		//			if( !writer.Write<uuid>( ISDKeyMacro( "MeshId" ) , obj.meshId ) )
		//				return false;
		//			return true;
		//			}
		//	};

		//class SceneLayer
		//	{
		//	public:
		//		SceneGraph Graph; // DAG graph of nodes in layer
		//		Dictionary<uuid,SceneNode> Nodes; // all nodes in the layer
		//		Dictionary<uuid,SceneMesh> Meshes; // all meshes connected to nodes 

		//		class MF;
		//		friend MF;

		//	public:
		//		SceneLayer()
		//			{
		//			}
		//	};

		//class SceneLayer::MF
		//	{
		//	public:
		//		static bool Write( SceneLayer &obj, EntityWriter &writer )
		//			{
		//			EntityWriter *section_writer;
		//			
		//			// write Graph
		//			if( EntityWriter *section_writer = writer.BeginWriteSection( ISDKeyMacro( "Graph" ) ) != nullptr )
		//				{
		//				if( !SceneGraph::MF::Write( obj.Graph, *section_writer ) )
		//					return false;
		//				writer.EndWriteSection( section_writer );
		//				}
		//			else
		//				return false;
		//			
		//			// write Nodes
		//			section_writer = writer.BeginWriteSection( ISDKeyMacro( "Nodes" ) );
		//			if( !section_writer || !Dictionary<uuid,SceneNode>::MF::Write( obj.Nodes, *section_writer ) )
		//				return false;
		//			writer.EndWriteSection( section_writer );

		//			// write Meshes
		//			section_writer = writer.BeginWriteSection( ISDKeyMacro( "Meshes" ) );
		//			if( !section_writer || !Dictionary<uuid,SceneMesh>::MF::Write( obj.Meshes, *section_writer ) )
		//				return false;
		//			writer.EndWriteSection( section_writer );

		//			return true;
		//			}
		//	};


		};
	};
