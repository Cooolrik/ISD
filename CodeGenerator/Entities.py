# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

class Entity:
    def __init__(self, name, variables, dependencies = [], templates = []):
        self.Name = name
        self.Dependencies = dependencies
        self.Templates = templates
        self.Variables = variables

class Dependency:
    def __init__(self, name, include_in_header):
        self.Name = name
        self.IncludeInHeader = include_in_header

class Template:
    def __init__(self, name, template, types, flags = [] ):
        self.Name = name
        self.Template = template
        self.Types = types
        self.Flags = flags

class Variable:
    def __init__(self, type, name, optional = False):
        self.Type = type
        self.Name = name
        self.Optional = optional

# -----------------

SceneNode = Entity(
    name = "SceneNode", 
    dependencies = [ Dependency("DataValuePointers", include_in_header = True) ],
    variables = [ Variable("string", "Name"),
                  Variable("fmat4", "Translation"),
                  Variable("fmat4", "Rotation") ]
    )

SceneMesh = Entity(
    name = "SceneMesh", 
    variables = [ Variable("uuid" , "MeshPacketId") ]
    )
       
SceneLayer = Entity(
    name = "SceneLayer", 
    dependencies = [ Dependency("DirectedGraph", include_in_header = True),
                     Dependency("Dictionary", include_in_header = True),
                     Dependency("SceneNode", include_in_header = True),
                     Dependency("SceneMesh", include_in_header = True) ],
    templates = [ Template("scene_graph", template = "DirectedGraph", types = ["uuid"], flags = ["Acyclic","Rooted"]),
                  Template("scene_nodes", template = "Dictionary", types = ["uuid","SceneNode"] ),
                  Template("scene_meshes", template = "Dictionary", types = ["uuid","SceneMesh"] ) ],
    variables = [ Variable("string", "Name", optional = True ),
                  Variable("scene_graph" , "Graph"),
                  Variable("scene_nodes" , "Nodes"),
                  Variable("scene_meshes" , "Meshes", optional = True ) ]
    )
      
Entities = [SceneNode, SceneMesh, SceneLayer]


