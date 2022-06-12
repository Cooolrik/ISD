# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license
# https://github.com/Cooolrik/ISD/blob/main/LICENSE

import CodeGeneratorHelpers as hlp

class Entity:
	def __init__(self, name, variables, dependencies = [], templates = []):
		self.Name = name
		self.Dependencies = dependencies
		self.Templates = templates
		self.Variables = variables

class Dependency:
	def __init__(self, name, include_in_header = False ):
		self.Name = name
		self.IncludeInHeader = include_in_header

class Template:
	def __init__(self, name, template, types, flags = [] ):
		self.Name = name
		self.Template = template
		self.Types = types
		self.Flags = flags

		# create declaration of template
		self.Declaration = f'            using {self.Name} = {self.Template}<'
		has_value = False
		for ty in self.Types:
			if has_value:
				self.Declaration += ','
			self.Declaration += ty
			has_value = True
		if len(self.Flags) > 0:
			if has_value:
				self.Declaration += ','
			has_flag = False;
			for fl in self.Flags:
				if has_flag:
					self.Declaration += '|'
				self.Declaration += self.Template + 'Flags::' + fl
				has_flag = True
			has_value = True
		self.Declaration += '>;'

class Variable:
	def __init__(self, type, name, optional = False, vector = False, indexed = False ):
		self.Type = type
		self.Name = name
		self.Optional = optional
		self.Vector = vector
		self.IndexedVector = indexed
		if self.IndexedVector and not self.Vector:
			sys.exit("Variable.__init__: IndexedVector requires Vector flag to be set as well")

		# build the type string
		if self.Optional:
			if self.Vector:
				if self.IndexedVector:
					self.TypeString = f"optional_idx_vector<{self.Type}>"
				else:
					self.TypeString = f"optional_vector<{self.Type}>"
			else:
				self.TypeString = f"optional_value<{self.Type}>"
		else:
			if self.Vector:
				if self.IndexedVector:
					self.TypeString = f"idx_vector<{self.Type}>"
				else:
					self.TypeString = f"std::vector<{self.Type}>"
			else:
				self.TypeString = self.Type

		# check if this is a simple value, or a complex value (which is using one of the wrapper classes)
		self.IsSimpleValue = (not self.Optional) and (not self.Vector) and (not self.IndexedVector)
		self.IsComplexValue = not self.IsSimpleValue

		# look up BaseType and BaseVariant
		self.BaseType,self.BaseVariant = hlp.get_base_type_variant(self.Type)
		self.IsBaseType = self.BaseType is not None

		# check if this is a simple value which is a base type
		self.IsSimpleBaseType = self.BaseType and self.IsSimpleValue


	 
# define the Entities list, we will fill this in in the submodules
Entities = []
   
# import all submodules
from .Nodes import Node
from .Nodes import NodeGeometry
from .Scene import Scene
from .Scene import SceneLayer
from .Geometry import Mesh
from .Geometry import AttributeLayer
from .Testing import TestEntity
