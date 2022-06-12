# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

from Entities import *

Entities.append(
    Entity(
        name = "AttributeLayer", 
        dependencies = [ Dependency("DataValuePointers", include_in_header = True) ],
        variables = [ Variable("string", "Name"),
                      Variable("fvec2", "UVs", optional = True , indexed = True , vector = True ),
                      Variable("fvec3", "Tangents", optional = True , indexed = True , vector = True ),
                      Variable("fvec3", "Bitangents", optional = True , indexed = True , vector = True ),
                      Variable("fvec3", "Normals", optional = True , indexed = True , vector = True ),
                      Variable("fvec4", "Colors", optional = True , indexed = True , vector = True ),
                      ]
        )
    )

