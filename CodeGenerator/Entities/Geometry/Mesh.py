# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

from Entities import *

Entities.append(
    Entity(
        name = "Mesh", 
        dependencies = [ Dependency("Dictionary", include_in_header = True),
                         Dependency("AttributeLayer", include_in_header = True) ],
        templates = [ Template("attribute_layers", template = "Dictionary", types = ["entity_ref","AttributeLayer"] ) ],
        variables = [ Variable("attribute_layers" , "Layers") ]
        )
    )

