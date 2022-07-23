# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

from Entities import *

Entities.append(
    Entity(
        name = "Scene", 
        dependencies = [ Dependency("EntityTable", include_in_header = True),
                         Dependency("SceneLayer", include_in_header = True) ],
        templates = [ Template("scene_layers", template = "EntityTable", types = ["package_ref","SceneLayer"] ) ],
        variables = [ Variable("scene_layers" , "Layers") ]
        )
    )

