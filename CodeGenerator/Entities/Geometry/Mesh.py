# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

from Entities import *

Entities.append(
    Entity(
        name = "Mesh", 
        dependencies = [ Dependency("Dictionary", include_in_header = True),
                         Dependency("AttributeLayer", include_in_header = True),
                         Dependency("IndexedVector", include_in_header = True),
                         Dependency("Varying"),
                         ],
        templates = [ Template("attributes_layer", template = "Dictionary", flags = ['NoZeroKeys','NoNullEntities'] , types = ["entity_ref","AttributeLayer"] ),
                      Template("attributes_layer_fvec2", template = "Dictionary", flags = ['NoZeroKeys','NoNullEntities'] , types = ["entity_ref","IndexedVector<fvec2>"] ),
                      Template("attributes_layer_fvec3", template = "Dictionary", flags = ['NoZeroKeys','NoNullEntities'] , types = ["entity_ref","IndexedVector<fvec3>"] ),
                      Template("attributes_layer_fvec4", template = "Dictionary", flags = ['NoZeroKeys','NoNullEntities'] , types = ["entity_ref","IndexedVector<fvec4>"] ),
                      Template("attributes_layer_custom", template = "Dictionary", flags = ['NoZeroKeys','NoNullEntities'] , types = ["entity_ref","Varying"] )
                     ],
        variables = [ Variable("attributes_layer" , "Layers"),
                      Variable("attributes_layer_fvec2" , "TextureCoordsData"),
                      Variable("attributes_layer_fvec3" , "TangentsData"),
                      Variable("attributes_layer_fvec3" , "BitangentsData"),
                      Variable("attributes_layer_fvec3" , "NormalsData"),
                      Variable("attributes_layer_fvec4" , "ColorsData"),
                      Variable("attributes_layer_custom" , "CustomData")
                      ]
        )
    )

