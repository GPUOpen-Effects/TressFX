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

#ifndef _SUSPHERE_H
#define _SUSPHERE_H

#include "SuDrawableObject.h"
#include "SuVector3.h"
#include "SuMatrix4.h"
#include "SuEffect.h"
#include "SuEffectReloadListener.h"

// forward declarations
class SuGPUVertexBuffer;
class SuGPUIndexBuffer;

typedef SuCountedPtr< SuGPUIndexBuffer > SuGPUIndexBufferPtr;
typedef SuCountedPtr< SuGPUVertexBuffer > SuGPUVertexBufferPtr;

// tolua_begin
class SuSphere : public SuDrawableObject, public SuEffectReloadListener
{
public:
	/// Constructor
	SU_DLL SuSphere();

	/// Destructor
	virtual SU_DLL ~SuSphere();

	/// Sets the color of the box
	SU_DLL void SetColor(const SuVector3& rColor);

	/// Returns the color of the box
	inline const SuVector3& GetColor() const { return m_color; }

	/// Sets the model->world transform matrix
	SU_DLL void SetModelToWorldTransform(const SuMatrix4& rTransform);

	/// Sets the model-world transform such that the sphere is centered at the specified location and has the specified radius
	SU_DLL void SetBoundingSphereTransform(const SuPoint3& rCenter, float fRadius);

	/// Returns the model->world transform matrix
	inline const SuMatrix4& GetModelToWorldTransform() const { return m_transform; }

	/// Enable/disable drawing the box as wireframe
	SU_DLL void EnableWireFrame(bool bEnabled);

	/// Returns wireframe state
	SU_DLL bool IsWireFrameEnabled() const { return m_bDrawWireFrame; }

	/// Draws using default settings
	virtual SU_DLL bool Draw(uint32 nInstances = 1);

	/// Draws using a specific technique
	virtual SU_DLL bool Draw(const SuString& rEffectTechniqueName, uint32 nInstances = 1);

	/// Draws using a set of technique properties
	virtual SU_DLL bool Draw(uint32 nProperties, const SuString* pProperties, uint32 nInstances = 1);

	virtual SU_DLL bool HasMatchingTechnique(uint32 nProperties, const SuString* pProperties);


// tolua_end

	virtual SU_DLL void OnEffectReload(SuEffectPtr pOld, SuEffectPtr pNew);

private:
	/// Creates global instance of GPU resources for rendering boxe
	bool CreateResources();

	/// Destroys global box GPU resources
	void DestroyResources();

	/// Fills a buffer with sphere vertex data
	void InitVertexBuffer(float32* pfVerts, uint32 nSlices, uint32 nStacks) const;

	/// Fills a buffer with sphere index data
	void InitIndexBuffer(uint16* pnIndices, uint32 nSlices, uint32 nStacks) const;

	/// Fills a buffer with wireframe sphere index data
	void InitWireFrameIndexBuffer(uint16* pnIndices, uint32 nSlices, uint32 nStacks) const;

	/// Returns the number of vertices of a wireframe sphere
	uint32 NumSphereVertices(uint32 nSlices, uint32 nStacks) const;

	/// Returns the number of indices of a sphere
	uint32 NumSphereIndices(uint32 nSlices, uint32 nStacks) const;

	/// Returns the number of indices of a wireframe sphere
	uint32 NumWireFrameSphereIndices(uint32 nSlices, uint32 nStacks) const;

	SuVector3 m_color;
	SuMatrix4 m_transform;
	bool      m_bDrawWireFrame;

	// Static geometry and effect data shared by all instances.
	static SuEffectPtr ms_pEffect;
	static SuGPUVertexBufferPtr ms_pVB;
	static SuGPUIndexBufferPtr  ms_pIB;
	static SuGPUIndexBufferPtr  ms_pWireIB;

	static const uint32 ms_nNUM_SLICES;
	static const uint32 ms_nNUM_STACKS;
	static const uint32 ms_nNUM_FLOATS_PER_VERTEX;

	// number of spheres using the VB, IB and effect
	static uint32 ms_nResourceRefCount;

// tolua_begin
};
// tolua_end

#endif

