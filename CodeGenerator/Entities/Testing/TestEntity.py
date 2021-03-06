# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

from Entities import *

Entities.append(
    Entity(
        name = "TestEntity", 
        dependencies = [],
        variables = [ Variable("string", "Name"),
                      Variable("string", "OptionalText", optional = True ) ]
        )
    )
