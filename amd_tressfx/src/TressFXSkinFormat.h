//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//
// TressFXSkinFormat.h
//
// Definition of the binary .tfxskin file format which is used for specifying the
// skinning information to use with a .tfx hair file. Note that the number of hairs
// for this file must match the number of hairs in the .tfx file it's used with in the .tfxproj file.

#pragma once

// There is one HairToTriangleMapping structure for each hair in the .tfx file
struct HairToTriangleMapping
{
    UINT    mesh;                           // index to the mesh
    UINT    triangle;                       // index to triangle in the skinned mesh that contains this hair
    FLOAT   barycentricCoord[3];            // barycentric coordinate of the hair within the triangle
    UINT    reserved;                       // for future use
};

//
// File format exported by the authoring tool
//

struct TressFXSkinFileObject
{
    UINT version;                           // .tfxskin file version number
    UINT numHairs;                          // number of hairs that this file maps to the skinned mesh
    UINT numTriangles;                      // number of triangles in the skinned mesh that contain hairs
    UINT reserved1[31];

    // Offsets to data
    // Offset values are in bytes, aligned on 8 bytes boundaries, and relative to beginning of the .tfxsim file
    UINT hairToMeshMap_Offset;              // Array size: HairToTriangleMapping[numHairs]
    UINT perStrandUVCoordniate_Offset;
    UINT reserved2[31];
};
