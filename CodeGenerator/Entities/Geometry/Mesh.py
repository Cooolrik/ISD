# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

from Entities import *

Entities.append(
    Entity(
        name = "Mesh", 
        dependencies = [ Dependency("EntityTable", include_in_header = True),
                         Dependency("AttributeLayer", include_in_header = True),
                         Dependency("IndexedVector", include_in_header = True),
                         Dependency("Varying"),
                         ],
        templates = [ Template("attribute_layers", template = "EntityTable", types = ["entity_ref","AttributeLayer"] ),
                      Template("attribute_layers_fvec2", template = "EntityTable", types = ["entity_ref","IndexedVector<fvec2>"] ),
                      Template("attribute_layers_fvec3", template = "EntityTable", types = ["entity_ref","IndexedVector<fvec3>"] ),
                      Template("attribute_layers_fvec4", template = "EntityTable", types = ["entity_ref","IndexedVector<fvec4>"] ),
                      Template("attribute_layers_custom", template = "EntityTable", types = ["entity_ref","Varying"] )
                     ],
        variables = [ Variable("attribute_layers" , "Layers"),
                      Variable("attribute_layers_fvec2" , "TextureCoordsData"),
                      Variable("attribute_layers_fvec3" , "TangentsData"),
                      Variable("attribute_layers_fvec3" , "BitangentsData"),
                      Variable("attribute_layers_fvec3" , "NormalsData"),
                      Variable("attribute_layers_fvec4" , "ColorsData"),
                      Variable("attribute_layers_custom" , "CustomData")
                      ]
        )
    )

