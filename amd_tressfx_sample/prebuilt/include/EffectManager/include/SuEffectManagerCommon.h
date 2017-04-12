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

#ifndef _SUEFFECTMANAGERCOMMON_H
#define _SUEFFECTMANAGERCOMMON_H

/// Supported shading languages
///
/// \ingroup EffectManager
enum SuShadingLanguage
{
   SU_SHADING_LANG_HLSL,
   SU_SHADING_LANG_GLSL
};

/// Supported shader types
///
/// \ingroup EffectManager
enum SuShaderType
{
   SU_VERTEX_SHADER,
   SU_GEOMETRY_SHADER,
   SU_PIXEL_SHADER,
   SU_COMPUTE_SHADER,
   SU_HULL_SHADER,
   SU_DOMAIN_SHADER
};

/// Combinable shader visiblity flags
enum SuShaderVisibility
{
	SHADER_VISIBILITY_NONE = 0,
	SHADER_VISIBILITY_VERTEX = 0x1,
	SHADER_VISIBILITY_HULL = 0x2,
	SHADER_VISIBILITY_DOMAIN = 0x4,
	SHADER_VISIBILITY_GEOMETRY = 0x8,
	SHADER_VISIBILITY_PIXEL = 0x10,
	SHADER_VISIBILITY_COMPUTE = 0x20,
	SHADER_VISIBILITY_ALL = (SHADER_VISIBILITY_VERTEX | SHADER_VISIBILITY_HULL | SHADER_VISIBILITY_DOMAIN | SHADER_VISIBILITY_GEOMETRY | SHADER_VISIBILITY_PIXEL | SHADER_VISIBILITY_COMPUTE),
};

// Maximum number of allowed render targets per pass:
const int gc_nMAX_RENDER_TARGETS = 8;

#endif