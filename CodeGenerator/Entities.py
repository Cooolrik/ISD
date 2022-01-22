# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE
class Entity:
    def __init__(self,name,variables):
        self.Name = name
        self.Variables = variables

class Variable:
    def __init__(self,type,name):
        self.Type = type
        self.Name = name

# -----------------

SceneNode = Entity(
    "SceneNode", [
        Variable("string" , "Name"),
        Variable("mat4" , "Translation"),
        Variable("mat4" , "Rotation")
    ])

SceneMesh = Entity(
    "SceneMesh", [
        Variable("uuid" , "MeshPacketId")
    ])
       
SceneLayer = Entity(
    "SceneLayer", [
        Variable("SceneGraph" , "Graph"),
        Variable("Dictionary<uuid,SceneNode>" , "Nodes"),
        Variable("Dictionary<uuid,SceneMesh>" , "Meshes")
    ])
      
Entities = [SceneNode, SceneMesh, SceneLayer]


