# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

from Entities import *

Entities.append(
    Entity(
        name = "SceneLayer", 
        dependencies = [ Dependency("DirectedGraph", include_in_header = True),
                         Dependency("EntityTable", include_in_header = True),
                         Dependency("Node", include_in_header = True),
                         Dependency("NodeGeometry", include_in_header = True) ],
        templates = [ Template("scene_graph", template = "DirectedGraph", types = ["entity_ref"], flags = ["Acyclic","Rooted"]),
                      Template("nodes", template = "EntityTable", types = ["entity_ref","Node"] ),
                      Template("node_geometries", template = "EntityTable", types = ["entity_ref","NodeGeometry"] ) ],
        variables = [ Variable("string", "Name" ),
                      Variable("scene_graph" , "Graph"),
                      Variable("nodes" , "Nodes"),
                      Variable("node_geometries" , "Geometries") ]
        )
    )
