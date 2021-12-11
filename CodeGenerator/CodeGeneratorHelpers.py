# ISD Copyright (c) 2021 Ulrik Lindahl
# Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE


import os
from stat import S_IRUSR, S_IRGRP, S_IROTH, S_IWUSR
import importlib

class BaseType:
    def __init__(self,name,variants):
        self.name = name
        self.variants = variants

class BaseTypeVariant:
    def __init__(self,implementing_type,item_type,num_items_per_object):
        self.implementing_type = implementing_type
        self.item_type = item_type
        self.num_items_per_object = num_items_per_object

base_type_Bool = BaseType('Bool',[BaseTypeVariant('bool','bool',1)])
base_type_Int =  BaseType('Int',[BaseTypeVariant('i8','i8',1),BaseTypeVariant('i16','i16',1),BaseTypeVariant('i32','i32',1),BaseTypeVariant('i64','i64',1)])
base_type_UInt =  BaseType('UInt',[BaseTypeVariant('u8','u8',1),BaseTypeVariant('u16','u16',1),BaseTypeVariant('u32','u32',1),BaseTypeVariant('u64','u64',1)])
base_type_Float =  BaseType('Float',[BaseTypeVariant('float','float',1),BaseTypeVariant('double','double',1)])
base_type_Vec2 =  BaseType('Vec2',[BaseTypeVariant('fvec2','float',2),BaseTypeVariant('dvec2','double',2)]) 
base_type_Vec3 =  BaseType('Vec3',[BaseTypeVariant('fvec3','float',3),BaseTypeVariant('dvec3','double',3)])  
base_type_Vec4 =  BaseType('Vec4',[BaseTypeVariant('fvec4','float',4),BaseTypeVariant('dvec4','double',4)])  
base_type_IVec2 =  BaseType('IVec2',[BaseTypeVariant('i8vec2','i8',2),BaseTypeVariant('i16vec2','i16',2),BaseTypeVariant('i32vec2','i32',2),BaseTypeVariant('i64vec2','i64',2)])    
base_type_IVec3 =  BaseType('IVec3',[BaseTypeVariant('i8vec3','i8',3),BaseTypeVariant('i16vec3','i16',3),BaseTypeVariant('i32vec3','i32',3),BaseTypeVariant('i64vec3','i64',3)])    
base_type_IVec4 =  BaseType('IVec4',[BaseTypeVariant('i8vec4','i8',4),BaseTypeVariant('i16vec4','i16',4),BaseTypeVariant('i32vec4','i32',4),BaseTypeVariant('i64vec4','i64',4)])    
base_type_UVec2 =  BaseType('UVec2',[BaseTypeVariant('u8vec2','u8',2),BaseTypeVariant('u16vec2','u16',2),BaseTypeVariant('u32vec2','u32',2),BaseTypeVariant('u64vec2','u64',2)])    
base_type_UVec3 =  BaseType('UVec3',[BaseTypeVariant('u8vec3','u8',3),BaseTypeVariant('u16vec3','u16',3),BaseTypeVariant('u32vec3','u32',3),BaseTypeVariant('u64vec3','u64',3)])    
base_type_UVec4 =  BaseType('UVec4',[BaseTypeVariant('u8vec4','u8',4),BaseTypeVariant('u16vec4','u16',4),BaseTypeVariant('u32vec4','u32',4),BaseTypeVariant('u64vec4','u64',4)])    
base_type_Mat2 =  BaseType('Mat2',[BaseTypeVariant('fmat2','float',4),BaseTypeVariant('dmat2','double',4)])    
base_type_Mat3 =  BaseType('Mat3',[BaseTypeVariant('fmat3','float',9),BaseTypeVariant('dmat3','double',9)])    
base_type_Mat4 =  BaseType('Mat4',[BaseTypeVariant('fmat4','float',16),BaseTypeVariant('dmat4','double',16)])
base_type_Quat =  BaseType('Quat',[BaseTypeVariant('fquat','float',4),BaseTypeVariant('dquat','double',4)])
base_type_UUID =  BaseType('UUID',[BaseTypeVariant('UUID','UUID',1)])
base_type_String = BaseType('String',[BaseTypeVariant('string','string',1)])

base_types = [base_type_Bool,
              base_type_Int,
              base_type_UInt,
              base_type_Float,
              base_type_Vec2,
              base_type_Vec3,
              base_type_Vec4,
              base_type_IVec2,
              base_type_IVec3,
              base_type_IVec4,
              base_type_UVec2,
              base_type_UVec3,
              base_type_UVec4,
              base_type_Mat2,
              base_type_Mat3,
              base_type_Mat4,
              base_type_Quat,
              base_type_UUID,
              base_type_String]

def run_module( name ):
    print('Running: ' + name )
    importlib.import_module('Generators.' + name ).run()
    print('')

def write_lines_to_file( path , lines ):
    # make into one long string
    new_text = ''
    for line in lines:
        new_text += line + '\n'

    # if the file already exists, check if we have an update
    if os.path.exists(path):
        # try reading in the current file if one exists
        with open(path,'r') as f:
            existing_text = f.read()
            f.close()

        #if no difference was found, return
        if new_text == existing_text:
            print( '\tSkipping: ' + path + ', (it is identical)...')
            return

        # if a difference was found, remove the old file
        os.chmod(path, S_IWUSR)
        os.remove( path ) 

    # we should write to the new file
    print( '\tWriting: ' + path + '...')
    with open(path,'w') as f:
        f.write(new_text)
        f.close()

    # change mode of file to readonly
    os.chmod(path, S_IRUSR|S_IRGRP|S_IROTH)