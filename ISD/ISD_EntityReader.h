// ISD Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ISD/blob/main/LICENSE

#pragma once

#include "ISD_Types.h"
#include "ISD_Entity.h"

namespace ISD
    {
    class MemoryReadStream;

    class EntityReader
        {
        private:
            MemoryReadStream &sstream;

        public:
            EntityReader( MemoryReadStream &_sstream ) : sstream( _sstream ) {}

            // The Read function template, specifically implemented below for all supported value types.
            template <class T> bool Read( const char *key, const uint8 key_length, T &value );

            // VT_Bool
            template <> bool Read<bool>( const char *key, const uint8 key_length, bool &value );
            template <> bool Read<optional_value<bool>>( const char *key, const uint8 key_length, optional_value<bool> &value );

            // VT_Int
            template <> bool Read<int8>( const char *key, const uint8 key_length, int8 &value );
            template <> bool Read<optional_value<int8>>( const char *key, const uint8 key_length, optional_value<int8> &value );
            template <> bool Read<int16>( const char *key, const uint8 key_length, int16 &value );
            template <> bool Read<optional_value<int16>>( const char *key, const uint8 key_length, optional_value<int16> &value );
            template <> bool Read<int32>( const char *key, const uint8 key_length, int32 &value );
            template <> bool Read<optional_value<int32>>( const char *key, const uint8 key_length, optional_value<int32> &value );
            template <> bool Read<int64>( const char *key, const uint8 key_length, int64 &value );
            template <> bool Read<optional_value<int64>>( const char *key, const uint8 key_length, optional_value<int64> &value );

            // VT_UInt
            template <> bool Read<uint8>( const char *key, const uint8 key_length, uint8 &value );
            template <> bool Read<optional_value<uint8>>( const char *key, const uint8 key_length, optional_value<uint8> &value );
            template <> bool Read<uint16>( const char *key, const uint8 key_length, uint16 &value );
            template <> bool Read<optional_value<uint16>>( const char *key, const uint8 key_length, optional_value<uint16> &value );
            template <> bool Read<uint32>( const char *key, const uint8 key_length, uint32 &value );
            template <> bool Read<optional_value<uint32>>( const char *key, const uint8 key_length, optional_value<uint32> &value );
            template <> bool Read<uint64>( const char *key, const uint8 key_length, uint64 &value );
            template <> bool Read<optional_value<uint64>>( const char *key, const uint8 key_length, optional_value<uint64> &value );

            // VT_Float
            template <> bool Read<float>( const char *key, const uint8 key_length, float &value );
            template <> bool Read<optional_value<float>>( const char *key, const uint8 key_length, optional_value<float> &value );
            template <> bool Read<double>( const char *key, const uint8 key_length, double &value );
            template <> bool Read<optional_value<double>>( const char *key, const uint8 key_length, optional_value<double> &value );

            // VT_Vec2
            template <> bool Read<fvec2>( const char *key, const uint8 key_length, fvec2 &value );
            template <> bool Read<optional_value<fvec2>>( const char *key, const uint8 key_length, optional_value<fvec2> &value );
            template <> bool Read<dvec2>( const char *key, const uint8 key_length, dvec2 &value );
            template <> bool Read<optional_value<dvec2>>( const char *key, const uint8 key_length, optional_value<dvec2> &value );

            // VT_Vec3
            template <> bool Read<fvec3>( const char *key, const uint8 key_length, fvec3 &value );
            template <> bool Read<optional_value<fvec3>>( const char *key, const uint8 key_length, optional_value<fvec3> &value );
            template <> bool Read<dvec3>( const char *key, const uint8 key_length, dvec3 &value );
            template <> bool Read<optional_value<dvec3>>( const char *key, const uint8 key_length, optional_value<dvec3> &value );

            // VT_Vec4
            template <> bool Read<fvec4>( const char *key, const uint8 key_length, fvec4 &value );
            template <> bool Read<optional_value<fvec4>>( const char *key, const uint8 key_length, optional_value<fvec4> &value );
            template <> bool Read<dvec4>( const char *key, const uint8 key_length, dvec4 &value );
            template <> bool Read<optional_value<dvec4>>( const char *key, const uint8 key_length, optional_value<dvec4> &value );

            // VT_IVec2
            template <> bool Read<i8vec2>( const char *key, const uint8 key_length, i8vec2 &value );
            template <> bool Read<optional_value<i8vec2>>( const char *key, const uint8 key_length, optional_value<i8vec2> &value );
            template <> bool Read<i16vec2>( const char *key, const uint8 key_length, i16vec2 &value );
            template <> bool Read<optional_value<i16vec2>>( const char *key, const uint8 key_length, optional_value<i16vec2> &value );
            template <> bool Read<i32vec2>( const char *key, const uint8 key_length, i32vec2 &value );
            template <> bool Read<optional_value<i32vec2>>( const char *key, const uint8 key_length, optional_value<i32vec2> &value );
            template <> bool Read<i64vec2>( const char *key, const uint8 key_length, i64vec2 &value );
            template <> bool Read<optional_value<i64vec2>>( const char *key, const uint8 key_length, optional_value<i64vec2> &value );

            // VT_IVec3
            template <> bool Read<i8vec3>( const char *key, const uint8 key_length, i8vec3 &value );
            template <> bool Read<optional_value<i8vec3>>( const char *key, const uint8 key_length, optional_value<i8vec3> &value );
            template <> bool Read<i16vec3>( const char *key, const uint8 key_length, i16vec3 &value );
            template <> bool Read<optional_value<i16vec3>>( const char *key, const uint8 key_length, optional_value<i16vec3> &value );
            template <> bool Read<i32vec3>( const char *key, const uint8 key_length, i32vec3 &value );
            template <> bool Read<optional_value<i32vec3>>( const char *key, const uint8 key_length, optional_value<i32vec3> &value );
            template <> bool Read<i64vec3>( const char *key, const uint8 key_length, i64vec3 &value );
            template <> bool Read<optional_value<i64vec3>>( const char *key, const uint8 key_length, optional_value<i64vec3> &value );

            // VT_IVec4
            template <> bool Read<i8vec4>( const char *key, const uint8 key_length, i8vec4 &value );
            template <> bool Read<optional_value<i8vec4>>( const char *key, const uint8 key_length, optional_value<i8vec4> &value );
            template <> bool Read<i16vec4>( const char *key, const uint8 key_length, i16vec4 &value );
            template <> bool Read<optional_value<i16vec4>>( const char *key, const uint8 key_length, optional_value<i16vec4> &value );
            template <> bool Read<i32vec4>( const char *key, const uint8 key_length, i32vec4 &value );
            template <> bool Read<optional_value<i32vec4>>( const char *key, const uint8 key_length, optional_value<i32vec4> &value );
            template <> bool Read<i64vec4>( const char *key, const uint8 key_length, i64vec4 &value );
            template <> bool Read<optional_value<i64vec4>>( const char *key, const uint8 key_length, optional_value<i64vec4> &value );

            // VT_UVec2
            template <> bool Read<u8vec2>( const char *key, const uint8 key_length, u8vec2 &value );
            template <> bool Read<optional_value<u8vec2>>( const char *key, const uint8 key_length, optional_value<u8vec2> &value );
            template <> bool Read<u16vec2>( const char *key, const uint8 key_length, u16vec2 &value );
            template <> bool Read<optional_value<u16vec2>>( const char *key, const uint8 key_length, optional_value<u16vec2> &value );
            template <> bool Read<u32vec2>( const char *key, const uint8 key_length, u32vec2 &value );
            template <> bool Read<optional_value<u32vec2>>( const char *key, const uint8 key_length, optional_value<u32vec2> &value );
            template <> bool Read<u64vec2>( const char *key, const uint8 key_length, u64vec2 &value );
            template <> bool Read<optional_value<u64vec2>>( const char *key, const uint8 key_length, optional_value<u64vec2> &value );

            // VT_UVec3
            template <> bool Read<u8vec3>( const char *key, const uint8 key_length, u8vec3 &value );
            template <> bool Read<optional_value<u8vec3>>( const char *key, const uint8 key_length, optional_value<u8vec3> &value );
            template <> bool Read<u16vec3>( const char *key, const uint8 key_length, u16vec3 &value );
            template <> bool Read<optional_value<u16vec3>>( const char *key, const uint8 key_length, optional_value<u16vec3> &value );
            template <> bool Read<u32vec3>( const char *key, const uint8 key_length, u32vec3 &value );
            template <> bool Read<optional_value<u32vec3>>( const char *key, const uint8 key_length, optional_value<u32vec3> &value );
            template <> bool Read<u64vec3>( const char *key, const uint8 key_length, u64vec3 &value );
            template <> bool Read<optional_value<u64vec3>>( const char *key, const uint8 key_length, optional_value<u64vec3> &value );

            // VT_UVec4
            template <> bool Read<u8vec4>( const char *key, const uint8 key_length, u8vec4 &value );
            template <> bool Read<optional_value<u8vec4>>( const char *key, const uint8 key_length, optional_value<u8vec4> &value );
            template <> bool Read<u16vec4>( const char *key, const uint8 key_length, u16vec4 &value );
            template <> bool Read<optional_value<u16vec4>>( const char *key, const uint8 key_length, optional_value<u16vec4> &value );
            template <> bool Read<u32vec4>( const char *key, const uint8 key_length, u32vec4 &value );
            template <> bool Read<optional_value<u32vec4>>( const char *key, const uint8 key_length, optional_value<u32vec4> &value );
            template <> bool Read<u64vec4>( const char *key, const uint8 key_length, u64vec4 &value );
            template <> bool Read<optional_value<u64vec4>>( const char *key, const uint8 key_length, optional_value<u64vec4> &value );

            // VT_Mat2
            template <> bool Read<fmat2>( const char *key, const uint8 key_length, fmat2 &value );
            template <> bool Read<optional_value<fmat2>>( const char *key, const uint8 key_length, optional_value<fmat2> &value );
            template <> bool Read<dmat2>( const char *key, const uint8 key_length, dmat2 &value );
            template <> bool Read<optional_value<dmat2>>( const char *key, const uint8 key_length, optional_value<dmat2> &value );

            // VT_Mat3
            template <> bool Read<fmat3>( const char *key, const uint8 key_length, fmat3 &value );
            template <> bool Read<optional_value<fmat3>>( const char *key, const uint8 key_length, optional_value<fmat3> &value );
            template <> bool Read<dmat3>( const char *key, const uint8 key_length, dmat3 &value );
            template <> bool Read<optional_value<dmat3>>( const char *key, const uint8 key_length, optional_value<dmat3> &value );

            // VT_Mat4
            template <> bool Read<fmat4>( const char *key, const uint8 key_length, fmat4 &value );
            template <> bool Read<optional_value<fmat4>>( const char *key, const uint8 key_length, optional_value<fmat4> &value );
            template <> bool Read<dmat4>( const char *key, const uint8 key_length, dmat4 &value );
            template <> bool Read<optional_value<dmat4>>( const char *key, const uint8 key_length, optional_value<dmat4> &value );

            // VT_UUID
            template <> bool Read<UUID>( const char *key, const uint8 key_length, UUID &value );
            template <> bool Read<optional_value<UUID>>( const char *key, const uint8 key_length, optional_value<UUID> &value );

            // VT_Array_Bool
            template <> bool Read<std::vector<bool>>( const char *key, const uint8 key_length, std::vector<bool> &value );
            template <> bool Read<optional_value<std::vector<bool>>>( const char *key, const uint8 key_length, optional_value<std::vector<bool>> &value );
            template <> bool Read<indexed_array<bool>>( const char *key, const uint8 key_length, indexed_array<bool> &value );
            template <> bool Read<optional_value<indexed_array<bool>>>( const char *key, const uint8 key_length, optional_value<indexed_array<bool>> &value );

            // VT_Array_Int
            template <> bool Read<std::vector<int8>>( const char *key, const uint8 key_length, std::vector<int8> &value );
            template <> bool Read<optional_value<std::vector<int8>>>( const char *key, const uint8 key_length, optional_value<std::vector<int8>> &value );
            template <> bool Read<indexed_array<int8>>( const char *key, const uint8 key_length, indexed_array<int8> &value );
            template <> bool Read<optional_value<indexed_array<int8>>>( const char *key, const uint8 key_length, optional_value<indexed_array<int8>> &value );
            template <> bool Read<std::vector<int16>>( const char *key, const uint8 key_length, std::vector<int16> &value );
            template <> bool Read<optional_value<std::vector<int16>>>( const char *key, const uint8 key_length, optional_value<std::vector<int16>> &value );
            template <> bool Read<indexed_array<int16>>( const char *key, const uint8 key_length, indexed_array<int16> &value );
            template <> bool Read<optional_value<indexed_array<int16>>>( const char *key, const uint8 key_length, optional_value<indexed_array<int16>> &value );
            template <> bool Read<std::vector<int32>>( const char *key, const uint8 key_length, std::vector<int32> &value );
            template <> bool Read<optional_value<std::vector<int32>>>( const char *key, const uint8 key_length, optional_value<std::vector<int32>> &value );
            template <> bool Read<indexed_array<int32>>( const char *key, const uint8 key_length, indexed_array<int32> &value );
            template <> bool Read<optional_value<indexed_array<int32>>>( const char *key, const uint8 key_length, optional_value<indexed_array<int32>> &value );
            template <> bool Read<std::vector<int64>>( const char *key, const uint8 key_length, std::vector<int64> &value );
            template <> bool Read<optional_value<std::vector<int64>>>( const char *key, const uint8 key_length, optional_value<std::vector<int64>> &value );
            template <> bool Read<indexed_array<int64>>( const char *key, const uint8 key_length, indexed_array<int64> &value );
            template <> bool Read<optional_value<indexed_array<int64>>>( const char *key, const uint8 key_length, optional_value<indexed_array<int64>> &value );

            // VT_Array_UInt
            template <> bool Read<std::vector<uint8>>( const char *key, const uint8 key_length, std::vector<uint8> &value );
            template <> bool Read<optional_value<std::vector<uint8>>>( const char *key, const uint8 key_length, optional_value<std::vector<uint8>> &value );
            template <> bool Read<indexed_array<uint8>>( const char *key, const uint8 key_length, indexed_array<uint8> &value );
            template <> bool Read<optional_value<indexed_array<uint8>>>( const char *key, const uint8 key_length, optional_value<indexed_array<uint8>> &value );
            template <> bool Read<std::vector<uint16>>( const char *key, const uint8 key_length, std::vector<uint16> &value );
            template <> bool Read<optional_value<std::vector<uint16>>>( const char *key, const uint8 key_length, optional_value<std::vector<uint16>> &value );
            template <> bool Read<indexed_array<uint16>>( const char *key, const uint8 key_length, indexed_array<uint16> &value );
            template <> bool Read<optional_value<indexed_array<uint16>>>( const char *key, const uint8 key_length, optional_value<indexed_array<uint16>> &value );
            template <> bool Read<std::vector<uint32>>( const char *key, const uint8 key_length, std::vector<uint32> &value );
            template <> bool Read<optional_value<std::vector<uint32>>>( const char *key, const uint8 key_length, optional_value<std::vector<uint32>> &value );
            template <> bool Read<indexed_array<uint32>>( const char *key, const uint8 key_length, indexed_array<uint32> &value );
            template <> bool Read<optional_value<indexed_array<uint32>>>( const char *key, const uint8 key_length, optional_value<indexed_array<uint32>> &value );
            template <> bool Read<std::vector<uint64>>( const char *key, const uint8 key_length, std::vector<uint64> &value );
            template <> bool Read<optional_value<std::vector<uint64>>>( const char *key, const uint8 key_length, optional_value<std::vector<uint64>> &value );
            template <> bool Read<indexed_array<uint64>>( const char *key, const uint8 key_length, indexed_array<uint64> &value );
            template <> bool Read<optional_value<indexed_array<uint64>>>( const char *key, const uint8 key_length, optional_value<indexed_array<uint64>> &value );

            // VT_Array_Float
            template <> bool Read<std::vector<float>>( const char *key, const uint8 key_length, std::vector<float> &value );
            template <> bool Read<optional_value<std::vector<float>>>( const char *key, const uint8 key_length, optional_value<std::vector<float>> &value );
            template <> bool Read<indexed_array<float>>( const char *key, const uint8 key_length, indexed_array<float> &value );
            template <> bool Read<optional_value<indexed_array<float>>>( const char *key, const uint8 key_length, optional_value<indexed_array<float>> &value );
            template <> bool Read<std::vector<double>>( const char *key, const uint8 key_length, std::vector<double> &value );
            template <> bool Read<optional_value<std::vector<double>>>( const char *key, const uint8 key_length, optional_value<std::vector<double>> &value );
            template <> bool Read<indexed_array<double>>( const char *key, const uint8 key_length, indexed_array<double> &value );
            template <> bool Read<optional_value<indexed_array<double>>>( const char *key, const uint8 key_length, optional_value<indexed_array<double>> &value );

            // VT_Array_Vec2
            template <> bool Read<std::vector<fvec2>>( const char *key, const uint8 key_length, std::vector<fvec2> &value );
            template <> bool Read<optional_value<std::vector<fvec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<fvec2>> &value );
            template <> bool Read<indexed_array<fvec2>>( const char *key, const uint8 key_length, indexed_array<fvec2> &value );
            template <> bool Read<optional_value<indexed_array<fvec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<fvec2>> &value );
            template <> bool Read<std::vector<dvec2>>( const char *key, const uint8 key_length, std::vector<dvec2> &value );
            template <> bool Read<optional_value<std::vector<dvec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<dvec2>> &value );
            template <> bool Read<indexed_array<dvec2>>( const char *key, const uint8 key_length, indexed_array<dvec2> &value );
            template <> bool Read<optional_value<indexed_array<dvec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<dvec2>> &value );

            // VT_Array_Vec3
            template <> bool Read<std::vector<fvec3>>( const char *key, const uint8 key_length, std::vector<fvec3> &value );
            template <> bool Read<optional_value<std::vector<fvec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<fvec3>> &value );
            template <> bool Read<indexed_array<fvec3>>( const char *key, const uint8 key_length, indexed_array<fvec3> &value );
            template <> bool Read<optional_value<indexed_array<fvec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<fvec3>> &value );
            template <> bool Read<std::vector<dvec3>>( const char *key, const uint8 key_length, std::vector<dvec3> &value );
            template <> bool Read<optional_value<std::vector<dvec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<dvec3>> &value );
            template <> bool Read<indexed_array<dvec3>>( const char *key, const uint8 key_length, indexed_array<dvec3> &value );
            template <> bool Read<optional_value<indexed_array<dvec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<dvec3>> &value );

            // VT_Array_Vec4
            template <> bool Read<std::vector<fvec4>>( const char *key, const uint8 key_length, std::vector<fvec4> &value );
            template <> bool Read<optional_value<std::vector<fvec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<fvec4>> &value );
            template <> bool Read<indexed_array<fvec4>>( const char *key, const uint8 key_length, indexed_array<fvec4> &value );
            template <> bool Read<optional_value<indexed_array<fvec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<fvec4>> &value );
            template <> bool Read<std::vector<dvec4>>( const char *key, const uint8 key_length, std::vector<dvec4> &value );
            template <> bool Read<optional_value<std::vector<dvec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<dvec4>> &value );
            template <> bool Read<indexed_array<dvec4>>( const char *key, const uint8 key_length, indexed_array<dvec4> &value );
            template <> bool Read<optional_value<indexed_array<dvec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<dvec4>> &value );

            // VT_Array_IVec2
            template <> bool Read<std::vector<i8vec2>>( const char *key, const uint8 key_length, std::vector<i8vec2> &value );
            template <> bool Read<optional_value<std::vector<i8vec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<i8vec2>> &value );
            template <> bool Read<indexed_array<i8vec2>>( const char *key, const uint8 key_length, indexed_array<i8vec2> &value );
            template <> bool Read<optional_value<indexed_array<i8vec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i8vec2>> &value );
            template <> bool Read<std::vector<i16vec2>>( const char *key, const uint8 key_length, std::vector<i16vec2> &value );
            template <> bool Read<optional_value<std::vector<i16vec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<i16vec2>> &value );
            template <> bool Read<indexed_array<i16vec2>>( const char *key, const uint8 key_length, indexed_array<i16vec2> &value );
            template <> bool Read<optional_value<indexed_array<i16vec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i16vec2>> &value );
            template <> bool Read<std::vector<i32vec2>>( const char *key, const uint8 key_length, std::vector<i32vec2> &value );
            template <> bool Read<optional_value<std::vector<i32vec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<i32vec2>> &value );
            template <> bool Read<indexed_array<i32vec2>>( const char *key, const uint8 key_length, indexed_array<i32vec2> &value );
            template <> bool Read<optional_value<indexed_array<i32vec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i32vec2>> &value );
            template <> bool Read<std::vector<i64vec2>>( const char *key, const uint8 key_length, std::vector<i64vec2> &value );
            template <> bool Read<optional_value<std::vector<i64vec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<i64vec2>> &value );
            template <> bool Read<indexed_array<i64vec2>>( const char *key, const uint8 key_length, indexed_array<i64vec2> &value );
            template <> bool Read<optional_value<indexed_array<i64vec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i64vec2>> &value );

            // VT_Array_IVec3
            template <> bool Read<std::vector<i8vec3>>( const char *key, const uint8 key_length, std::vector<i8vec3> &value );
            template <> bool Read<optional_value<std::vector<i8vec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<i8vec3>> &value );
            template <> bool Read<indexed_array<i8vec3>>( const char *key, const uint8 key_length, indexed_array<i8vec3> &value );
            template <> bool Read<optional_value<indexed_array<i8vec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i8vec3>> &value );
            template <> bool Read<std::vector<i16vec3>>( const char *key, const uint8 key_length, std::vector<i16vec3> &value );
            template <> bool Read<optional_value<std::vector<i16vec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<i16vec3>> &value );
            template <> bool Read<indexed_array<i16vec3>>( const char *key, const uint8 key_length, indexed_array<i16vec3> &value );
            template <> bool Read<optional_value<indexed_array<i16vec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i16vec3>> &value );
            template <> bool Read<std::vector<i32vec3>>( const char *key, const uint8 key_length, std::vector<i32vec3> &value );
            template <> bool Read<optional_value<std::vector<i32vec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<i32vec3>> &value );
            template <> bool Read<indexed_array<i32vec3>>( const char *key, const uint8 key_length, indexed_array<i32vec3> &value );
            template <> bool Read<optional_value<indexed_array<i32vec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i32vec3>> &value );
            template <> bool Read<std::vector<i64vec3>>( const char *key, const uint8 key_length, std::vector<i64vec3> &value );
            template <> bool Read<optional_value<std::vector<i64vec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<i64vec3>> &value );
            template <> bool Read<indexed_array<i64vec3>>( const char *key, const uint8 key_length, indexed_array<i64vec3> &value );
            template <> bool Read<optional_value<indexed_array<i64vec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i64vec3>> &value );

            // VT_Array_IVec4
            template <> bool Read<std::vector<i8vec4>>( const char *key, const uint8 key_length, std::vector<i8vec4> &value );
            template <> bool Read<optional_value<std::vector<i8vec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<i8vec4>> &value );
            template <> bool Read<indexed_array<i8vec4>>( const char *key, const uint8 key_length, indexed_array<i8vec4> &value );
            template <> bool Read<optional_value<indexed_array<i8vec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i8vec4>> &value );
            template <> bool Read<std::vector<i16vec4>>( const char *key, const uint8 key_length, std::vector<i16vec4> &value );
            template <> bool Read<optional_value<std::vector<i16vec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<i16vec4>> &value );
            template <> bool Read<indexed_array<i16vec4>>( const char *key, const uint8 key_length, indexed_array<i16vec4> &value );
            template <> bool Read<optional_value<indexed_array<i16vec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i16vec4>> &value );
            template <> bool Read<std::vector<i32vec4>>( const char *key, const uint8 key_length, std::vector<i32vec4> &value );
            template <> bool Read<optional_value<std::vector<i32vec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<i32vec4>> &value );
            template <> bool Read<indexed_array<i32vec4>>( const char *key, const uint8 key_length, indexed_array<i32vec4> &value );
            template <> bool Read<optional_value<indexed_array<i32vec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i32vec4>> &value );
            template <> bool Read<std::vector<i64vec4>>( const char *key, const uint8 key_length, std::vector<i64vec4> &value );
            template <> bool Read<optional_value<std::vector<i64vec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<i64vec4>> &value );
            template <> bool Read<indexed_array<i64vec4>>( const char *key, const uint8 key_length, indexed_array<i64vec4> &value );
            template <> bool Read<optional_value<indexed_array<i64vec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<i64vec4>> &value );

            // VT_Array_UVec2
            template <> bool Read<std::vector<u8vec2>>( const char *key, const uint8 key_length, std::vector<u8vec2> &value );
            template <> bool Read<optional_value<std::vector<u8vec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<u8vec2>> &value );
            template <> bool Read<indexed_array<u8vec2>>( const char *key, const uint8 key_length, indexed_array<u8vec2> &value );
            template <> bool Read<optional_value<indexed_array<u8vec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u8vec2>> &value );
            template <> bool Read<std::vector<u16vec2>>( const char *key, const uint8 key_length, std::vector<u16vec2> &value );
            template <> bool Read<optional_value<std::vector<u16vec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<u16vec2>> &value );
            template <> bool Read<indexed_array<u16vec2>>( const char *key, const uint8 key_length, indexed_array<u16vec2> &value );
            template <> bool Read<optional_value<indexed_array<u16vec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u16vec2>> &value );
            template <> bool Read<std::vector<u32vec2>>( const char *key, const uint8 key_length, std::vector<u32vec2> &value );
            template <> bool Read<optional_value<std::vector<u32vec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<u32vec2>> &value );
            template <> bool Read<indexed_array<u32vec2>>( const char *key, const uint8 key_length, indexed_array<u32vec2> &value );
            template <> bool Read<optional_value<indexed_array<u32vec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u32vec2>> &value );
            template <> bool Read<std::vector<u64vec2>>( const char *key, const uint8 key_length, std::vector<u64vec2> &value );
            template <> bool Read<optional_value<std::vector<u64vec2>>>( const char *key, const uint8 key_length, optional_value<std::vector<u64vec2>> &value );
            template <> bool Read<indexed_array<u64vec2>>( const char *key, const uint8 key_length, indexed_array<u64vec2> &value );
            template <> bool Read<optional_value<indexed_array<u64vec2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u64vec2>> &value );

            // VT_Array_UVec3
            template <> bool Read<std::vector<u8vec3>>( const char *key, const uint8 key_length, std::vector<u8vec3> &value );
            template <> bool Read<optional_value<std::vector<u8vec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<u8vec3>> &value );
            template <> bool Read<indexed_array<u8vec3>>( const char *key, const uint8 key_length, indexed_array<u8vec3> &value );
            template <> bool Read<optional_value<indexed_array<u8vec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u8vec3>> &value );
            template <> bool Read<std::vector<u16vec3>>( const char *key, const uint8 key_length, std::vector<u16vec3> &value );
            template <> bool Read<optional_value<std::vector<u16vec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<u16vec3>> &value );
            template <> bool Read<indexed_array<u16vec3>>( const char *key, const uint8 key_length, indexed_array<u16vec3> &value );
            template <> bool Read<optional_value<indexed_array<u16vec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u16vec3>> &value );
            template <> bool Read<std::vector<u32vec3>>( const char *key, const uint8 key_length, std::vector<u32vec3> &value );
            template <> bool Read<optional_value<std::vector<u32vec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<u32vec3>> &value );
            template <> bool Read<indexed_array<u32vec3>>( const char *key, const uint8 key_length, indexed_array<u32vec3> &value );
            template <> bool Read<optional_value<indexed_array<u32vec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u32vec3>> &value );
            template <> bool Read<std::vector<u64vec3>>( const char *key, const uint8 key_length, std::vector<u64vec3> &value );
            template <> bool Read<optional_value<std::vector<u64vec3>>>( const char *key, const uint8 key_length, optional_value<std::vector<u64vec3>> &value );
            template <> bool Read<indexed_array<u64vec3>>( const char *key, const uint8 key_length, indexed_array<u64vec3> &value );
            template <> bool Read<optional_value<indexed_array<u64vec3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u64vec3>> &value );

            // VT_Array_UVec4
            template <> bool Read<std::vector<u8vec4>>( const char *key, const uint8 key_length, std::vector<u8vec4> &value );
            template <> bool Read<optional_value<std::vector<u8vec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<u8vec4>> &value );
            template <> bool Read<indexed_array<u8vec4>>( const char *key, const uint8 key_length, indexed_array<u8vec4> &value );
            template <> bool Read<optional_value<indexed_array<u8vec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u8vec4>> &value );
            template <> bool Read<std::vector<u16vec4>>( const char *key, const uint8 key_length, std::vector<u16vec4> &value );
            template <> bool Read<optional_value<std::vector<u16vec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<u16vec4>> &value );
            template <> bool Read<indexed_array<u16vec4>>( const char *key, const uint8 key_length, indexed_array<u16vec4> &value );
            template <> bool Read<optional_value<indexed_array<u16vec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u16vec4>> &value );
            template <> bool Read<std::vector<u32vec4>>( const char *key, const uint8 key_length, std::vector<u32vec4> &value );
            template <> bool Read<optional_value<std::vector<u32vec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<u32vec4>> &value );
            template <> bool Read<indexed_array<u32vec4>>( const char *key, const uint8 key_length, indexed_array<u32vec4> &value );
            template <> bool Read<optional_value<indexed_array<u32vec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u32vec4>> &value );
            template <> bool Read<std::vector<u64vec4>>( const char *key, const uint8 key_length, std::vector<u64vec4> &value );
            template <> bool Read<optional_value<std::vector<u64vec4>>>( const char *key, const uint8 key_length, optional_value<std::vector<u64vec4>> &value );
            template <> bool Read<indexed_array<u64vec4>>( const char *key, const uint8 key_length, indexed_array<u64vec4> &value );
            template <> bool Read<optional_value<indexed_array<u64vec4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<u64vec4>> &value );

            // VT_Array_Mat2
            template <> bool Read<std::vector<fmat2>>( const char *key, const uint8 key_length, std::vector<fmat2> &value );
            template <> bool Read<optional_value<std::vector<fmat2>>>( const char *key, const uint8 key_length, optional_value<std::vector<fmat2>> &value );
            template <> bool Read<indexed_array<fmat2>>( const char *key, const uint8 key_length, indexed_array<fmat2> &value );
            template <> bool Read<optional_value<indexed_array<fmat2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<fmat2>> &value );
            template <> bool Read<std::vector<dmat2>>( const char *key, const uint8 key_length, std::vector<dmat2> &value );
            template <> bool Read<optional_value<std::vector<dmat2>>>( const char *key, const uint8 key_length, optional_value<std::vector<dmat2>> &value );
            template <> bool Read<indexed_array<dmat2>>( const char *key, const uint8 key_length, indexed_array<dmat2> &value );
            template <> bool Read<optional_value<indexed_array<dmat2>>>( const char *key, const uint8 key_length, optional_value<indexed_array<dmat2>> &value );

            // VT_Array_Mat3
            template <> bool Read<std::vector<fmat3>>( const char *key, const uint8 key_length, std::vector<fmat3> &value );
            template <> bool Read<optional_value<std::vector<fmat3>>>( const char *key, const uint8 key_length, optional_value<std::vector<fmat3>> &value );
            template <> bool Read<indexed_array<fmat3>>( const char *key, const uint8 key_length, indexed_array<fmat3> &value );
            template <> bool Read<optional_value<indexed_array<fmat3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<fmat3>> &value );
            template <> bool Read<std::vector<dmat3>>( const char *key, const uint8 key_length, std::vector<dmat3> &value );
            template <> bool Read<optional_value<std::vector<dmat3>>>( const char *key, const uint8 key_length, optional_value<std::vector<dmat3>> &value );
            template <> bool Read<indexed_array<dmat3>>( const char *key, const uint8 key_length, indexed_array<dmat3> &value );
            template <> bool Read<optional_value<indexed_array<dmat3>>>( const char *key, const uint8 key_length, optional_value<indexed_array<dmat3>> &value );

            // VT_Array_Mat4
            template <> bool Read<std::vector<fmat4>>( const char *key, const uint8 key_length, std::vector<fmat4> &value );
            template <> bool Read<optional_value<std::vector<fmat4>>>( const char *key, const uint8 key_length, optional_value<std::vector<fmat4>> &value );
            template <> bool Read<indexed_array<fmat4>>( const char *key, const uint8 key_length, indexed_array<fmat4> &value );
            template <> bool Read<optional_value<indexed_array<fmat4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<fmat4>> &value );
            template <> bool Read<std::vector<dmat4>>( const char *key, const uint8 key_length, std::vector<dmat4> &value );
            template <> bool Read<optional_value<std::vector<dmat4>>>( const char *key, const uint8 key_length, optional_value<std::vector<dmat4>> &value );
            template <> bool Read<indexed_array<dmat4>>( const char *key, const uint8 key_length, indexed_array<dmat4> &value );
            template <> bool Read<optional_value<indexed_array<dmat4>>>( const char *key, const uint8 key_length, optional_value<indexed_array<dmat4>> &value );

            // VT_Array_UUID
            template <> bool Read<std::vector<UUID>>( const char *key, const uint8 key_length, std::vector<UUID> &value );
            template <> bool Read<optional_value<std::vector<UUID>>>( const char *key, const uint8 key_length, optional_value<std::vector<UUID>> &value );
            template <> bool Read<indexed_array<UUID>>( const char *key, const uint8 key_length, indexed_array<UUID> &value );
            template <> bool Read<optional_value<indexed_array<UUID>>>( const char *key, const uint8 key_length, optional_value<indexed_array<UUID>> &value );

		};

	// Read method. Specialized for all supported value types.
	template <class T> bool EntityReader::Read( const char *key, const uint8 key_length, T &value )
		{
		static_assert(false, "Error: EntityReader::Read template: The value type T cannot be serialized.");
		}
	};
