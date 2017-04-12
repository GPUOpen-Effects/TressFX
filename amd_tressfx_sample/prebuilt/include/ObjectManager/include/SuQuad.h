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

#ifndef _SU_QUAD_H_
#define _SU_QUAD_H_

#include "SuDrawableObject.h"
#include "SuEffectReloadListener.h"
#include "SuPoint3.h"
#include "SuVector2.h"

// forward declarations
class SuGPUVertexBuffer;
class SuGPUIndexBuffer;
class SuGPUTexture2DArray;
struct SuSlotViewPair;

typedef SuCountedPtr< SuGPUTexture2DArray > SuGPUTexture2DArrayPtr;
typedef SuCountedPtr< SuGPUIndexBuffer > SuGPUIndexBufferPtr;
typedef SuCountedPtr< SuGPUVertexBuffer > SuGPUVertexBufferPtr;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A genereric full-screen quad class, which can render a quad with an arbitrary shader
//=================================================================================================================================
// tolua_begin
class SuQuad : public SuDrawableObject, public SuEffectReloadListener
{
public:

	/// Factory method for quad creation
	static SU_DLL SuQuad* CreateQuad(const SuString& rEffectFileName);

	/// Quad destructor
	SU_DLL ~SuQuad();

	/// Draws using default settings
	virtual SU_DLL bool Draw(uint32 nInstances = 1);

	/// Draws using a specific technique
	virtual SU_DLL bool Draw(const SuString& rEffectTechniqueName, uint32 nInstances = 1);

	/// Draws using a set of technique properties
	virtual SU_DLL bool Draw(uint32 nProperties, const SuString* pProperties, uint32 nInstances = 1);

	virtual SU_DLL bool HasMatchingTechnique(uint32 nProperties, const SuString* pProperties);

	// tolua_end

	  /// Handles an effect re-load
	virtual SU_DLL void OnEffectReload(SuEffectPtr pOldEffect, SuEffectPtr pNewEffect);


private:

	/// Constructor is private to restrict instantiation to CreateQuad() factory method
	SuQuad();

	/// Vertex structure for quads
	struct SuQuadVertex
	{
		SuPoint3  vPosition;
		SuVector2 vUV;
	};

	/// Vertex data used for quad rendering
	static SuQuadVertex QUAD_VERTEX_DATA[4];

	/// Index data used for quad rendering
	static uint16       QUAD_INDEX_DATA[4];

	/// Creates and initializes the vertex and index buffers for the quad
	bool CreateQuadGeometry();

	/// Tests whether or not an effect is a valid effect for an SuQuad
	static bool IsValidQuadEffect(SuEffectPtr pEffect);

	SuEffectPtr m_pEffect;        ///< The effect that is used by this quad
	SuGPUVertexBufferPtr m_pVB;   ///< Quad vertex buffer
	SuGPUIndexBufferPtr  m_pIB;   ///< Quad index buffer

	SuArray<SuSlotViewPair> m_renderTargetBindings; ///< Render target bindings

 // tolua_begin
};
// tolua_end


#endif // _SU_QUAD_H_
