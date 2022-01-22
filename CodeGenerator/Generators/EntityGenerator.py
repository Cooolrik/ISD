# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE
import CodeGeneratorHelpers as hlp
import Entities as ents

def CreateEntityHeader(entity):
	lines = []
	lines.append( '// ISD Copyright (c) 2021 Ulrik Lindahl')
	lines.append( '// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE')
	lines.append( '')
	lines.append( '#pragma once')
	lines.append( '')
	lines.append( '#include "ISD_Types.h"')
	lines.append( '')
	lines.append( 'namespace ISD')
	lines.append( '    {')
	lines.append(f'    class {entity.Name};')
	lines.append( '')
	lines.append( '    class EntityWriter;')
	lines.append( '    class EntityReader;')
	lines.append( '    class EntityValidator;')
	lines.append( '    };')
	hlp.write_lines_to_file(f"../ISD/ISD_{entity.Name}.h",lines)

def run():
	for entity in ents.Entities:
		CreateEntityHeader( entity )

