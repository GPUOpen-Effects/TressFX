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

#ifndef _SU_MODELCHUNK_H_
#define _SU_MODELCHUNK_H_

#include "SuRenderManager.h"
#include "SuDrawableObject.h"
#include "SuEffect.h"
#include "SuModelChunkDesc.h"
#include "SuEffectReloadListener.h"
#include "SuAutoDrawableObject.h"

class SuEffectParameter;
class SuParameterSetting;
class SuTextureSlot;
class SuGPUTexture;
class SuGPUResource;
class SuModelChunkDesc;
class SuMaterial;
class SuAnimationSet;
class SuModel;
struct SuSlotViewPair;

class SuSubChunkRenderer;
class SuSubChunkState;

class SuGPUResource;
class SuGPUVertexBuffer;
class SuGPUIndexBuffer;
typedef SuCountedPtr< SuGPUResource > SuGPUResourcePtr;
typedef SuCountedPtr< SuGPUIndexBuffer > SuGPUIndexBufferPtr;
typedef SuCountedPtr< SuGPUVertexBuffer > SuGPUVertexBufferPtr;
typedef SuArray<SuSlotViewPair> SuSlotViewArray;

//=========================================================================================================
/// \ingroup ObjectManager
/// \brief A class to represent a subset of a model which can be rendered in one draw call
/// 
///       A chunk is a piece of a model, with associated state settings, which can be rendered in one draw call.
///       Each will make any necessary changes to the effect state prior to rendering itself.  
/// 
///       Chunk order matters very greatly.  If a pair of chunks shares some common render state setting,
///       then it is assumed that the preprocessor will assign this state setting only to the first chunk in the sequence
///       and will order the chunks properly. 
///       For example, if chunks A and B both bind vertex buffer X to an effect stream, the preprocessor may choose
///       to list the stream binding only for chunk A, and assume that A will be drawn before B.
///       The point of all of this is that it is very important to draw chunks in the order they are defined in the model.
///       Chunks cannot easily be drawn in isolation (though it would be possible if we every want to)
///
//=========================================================================================================
class SuModelChunk : public SuDrawableObject, public SuAutoDrawableObject, public SuEffectReloadListener
{
public:

	/// Factory method which contructs a chunk for a model.
	static SuModelChunk* CreateChunk(SuModelChunkDescPtr pDesc, SuModel* pModel, SuModelChunk* pPredecessor);

	/// Destructor
	virtual ~SuModelChunk();

	/// Implements the SuDrawableObject interface.
	virtual SU_DLL bool Draw(uint32 nInstances);

	/// Implements the SuDrawableObject interface.
	virtual SU_DLL bool Draw(const SuString& rEffectTechniqueName, uint32 nInstances);

	/// Implements the SuDrawableObject interface.
	virtual SU_DLL bool Draw(uint32 nProperties, const SuString* pProperties, uint32 nInstances);

	/// Implements the SuDrawableObject interface.
	virtual SU_DLL bool HasMatchingTechnique(uint32 nProperties, const SuString* pProperties);

	/// Draws the model chunk using a particular technique of the object's effect using data written to a 
	/// streaming output buffer. 
	virtual SU_DLL bool DrawAuto(SuRenderManager::PrimType ePrimType, const SuString& rEffectTechniqueName);

	/// Draws the model chunk using a particular technique of the object's effect using data written to a 
	/// streaming output buffer. Note that this type of draw call will only work in DirectX10.
	virtual SU_DLL bool DrawAuto(const SuString& rEffectTechniqueName);

	/// Returns the chunk description for this chunk
	inline const SuModelChunkDesc* GetDesc() const { return m_pChunkDesc.get(); };

	/// Returns the subchunk renderer for this chunk, if one exists
	inline SuSubChunkRenderer* GetSubChunkRenderer() const { return m_pSubChunkRenderer; };

	/// Sets the animation set to use to retrieve curves for animated effect parameters.  This must be called before drawing
	void SetAnimationSet(SuAnimationSet* pAnimations);

	/// Sets the time value to use when sampling curves for animated effect parameters.  This must be called before drawing
	void SetAnimationTime(float fTime);

	/// Structure to hold information about skin targets used by this chunk
	struct SuSkinTargetInfo
	{
		SuString effectStreamName;
		uint32   nVertexSize;
		uint32   nVertexCount;
	};

	/// Returns information about the skin targets used by this chunk
	/// Skin targets must be created on a per-object basis, since multiple copies of the model may 
	/// exist and may be skinned differently.
	inline const SuArray<SuSkinTargetInfo>& GetSkinTargetInfo() const { return m_skinTargetInfo; };

	/// Sets the set of GPU vertex buffers to be used as skin targets. 
	/// This method must be called prior to rendering or skinning the chunk. 
	/// \param pBuffers Pointer to an array of vertex buffers to use as skin targets.  There should be 
	///   one buffer corresponding to each of the chunk's skin targets (as returned by GetSkinTargetInfo)
	inline void SetSkinTargets(const SuArray<SuGPUVertexBufferPtr>* pBuffers) { m_pSkinTargets = pBuffers; };

	/// Structure to hold information about the morph target buffers used by this chunk
	struct MorphDataBindingInfo
	{
		bool bIsVertexStream;         /// true if morph data is bound to vertex stream, false if it's bound to a texture slot
		SuString name;
		SuGPUVertexBufferPtr pVB;
		SuGPUSamplingResourceViewPtr pTexView;
	};

	/// Sets the set of morph target animators which contain the morph target VBs used by this chunk
	/// This method must be called prior to rendering the chunk. 
	inline void SetMorphTargetDataBindings(const SuArray<MorphDataBindingInfo>* pMorphTargetDataBindings)
	{
		m_pMorphTargetDataBindings = pMorphTargetDataBindings;
	}


	/// Sets the sub-chunk state to use for subchunk frustum culling.  This is necessary if the chunk has a subchunk renderer
	/// Passing NULL disables subchunk frustum culling
	inline void SetSubChunkState(const SuSubChunkState* pSubchunkState) { m_pSubChunkState = pSubchunkState; };

	/// Returns the effect used to render this chunk
	inline const SuEffectPtr GetEffect() const { return m_pEffect; };

	/// Performs stream-out skinning for this chunk's vertex buffers
	SU_DLL bool DoSOSkinning(const SuString& rTechnique);

	/// Draws the vertices from this chunk's vertex buffers as points.
	SU_DLL bool DrawVBsAsPoints(const SuString& rTechnique, uint32 nInstanceCount) const;

	/// Draws multiple instances of this chunk
	SU_DLL bool DrawInstances(const SuString& rTechnqiue, uint32 nInstances);

	/// Called on effect reload
	SU_DLL void OnEffectReload(SuEffectPtr pOldEffect, SuEffectPtr pNewEffect);

	/// Returns the chunk's visibility
	inline bool IsVisible() const { return m_bIsVisible; };

	/// Sets the visibility of this chunk.  Invisible chunks will still modify effect state when rendering, but will not draw themselves
	inline void SetVisible(bool bVisible) { m_bIsVisible = bVisible; };

	/// Returns the index buffer assigned to this chunk
	/// \return The index buffer that this chunk sets.  NULL if it uses a previous chunk's index buffer
	inline SuGPUIndexBufferPtr GetIB() const { return m_pIB; };

	/// Performs GPU cache warming
	SU_DLL void WarmGPUCaches() const;

	/// Returns the number of vertices referenced by this chunk
	inline uint32 GetVertexCount() const { return (m_nVBMax - m_nVBMin) + 1; };

	/// Returns the number of indices in this chunk
	inline uint32 GetIndexCount() const { return (m_nIBMax - m_nIBMin) + 1; };

	/// Returns the primitive type for this chunk
	inline SuRenderManager::PrimType GetPrimitiveType() const { return m_ePrimType; };

	/// Returns the name of this chunk
	inline const SuString& GetName() const { return m_name; };

private:

	/// Constructor is private to restrict instantiation to the CreateChunk factory method
	SuModelChunk();

	/// Mapping of source stream names to skin target streams
	typedef SuHashMap< SuString, SuVertexStreamDecl*, SuStringHasher, SuHashComparer<SuString> > SuSkinTargetMap;

	/// Applies this chunk's changes to the effect state
	void ApplyChunkState(SuEffectPtr pEffect) const;

	/// Performs subchunk frustum culling
	bool DoSubChunkCulling();

	/// Renders the chunk using DrawIndexed
	void DrawChunk(uint32 nInstances);

	/// Renders the chunk using DrawAuto()
	void DrawChunkAuto(SuRenderManager::PrimType ePrimType);

	/// Applies this chunk's changes to the effect state
	void CleanupChunkState(SuEffectPtr pEffect) const;

	/// Locates the effect parameters for each parameter binding in the chunk
	void ResolveEffectParameterBindings(SuEffectPtr pEffect);

	/// Figures out which static vertex buffers to bind to which effect streams
	bool ResolveStaticVBBindings(SuEffectPtr pEffect);

	/// Locates effect texture slots for each static texture referenced by the chunk
	bool ResolveStaticTextureBindings(SuEffectPtr pEffect);

	struct SuStreamVBPair
	{
		const SuVertexStreamDecl* pStream;
		SuGPUVertexBufferPtr      pVB;
	};

	typedef SuArray<SuStreamVBPair> SuStreamVBArray;

	SuString m_name;                                ///< The name of this chunk

	SuArray<SuSkinTargetInfo> m_skinTargetInfo;     ///< Description of the skin targets needed by this chunk
	SuModel* m_pModel;                              ///< Pointer to the model that owns the chunk
	SuModelChunkDescPtr m_pChunkDesc;               ///< Pointer to chunk description
	SuEffectPtr m_pEffect;                          ///< Pointer to the effect used for rendering
	SuModelChunk* m_pPredecessor;                   ///< Pointer to the chunk that precedes this one in the draw order
	SuParameterBindingList m_paramSettings;         ///< Effect parameter settings for this chunk
	SuMaterial* m_pMaterial;						   ///< Pointer to material used by this chunk

	SuStreamVBArray m_vbBindings; ///< Mapping of vertex stream decls to vertex buffers for this chunk
	SuSlotViewArray m_textureBindings;   ///< Mapping of texture slots to sampling resource views for this chunk
	SuGPUIndexBufferPtr m_pIB;         ///< Index buffer for this chunk.  If NULL, it is assumed that the predecessor's index buffer is to be used

	uint32 m_nVBMin;              ///< Minimum vertex index
	uint32 m_nVBMax;              ///< Maximum vertex index
	uint32 m_nIBMin;              ///< Minimum index number
	uint32 m_nIBMax;              ///< Maximum index number
	uint8 m_nIndexSize;           ///< The index size (needed for rendering)
	uint32 m_nStaticVBVertices;   ///< Number of vertices contained in this chunk's static vertex buffers.  0 if the chunk does not bind static vertex buffers

	SuRenderManager::PrimType m_ePrimType; ///< Primitive type for this chunk

	SuSubChunkRenderer* m_pSubChunkRenderer;  ///< Subchunk renderer, if any.  NULL if chunk has no subchunks

	// ********************************************************************************************
	//    Per-object chunk state.  
	//    These members are set at draw time by the SuAnimatedModel class when rendering chunks
	// ********************************************************************************************

	const SuArray<SuGPUVertexBufferPtr>*  m_pSkinTargets;              ///< Current set of skin targets to use when rendering
	const SuArray<MorphDataBindingInfo>*  m_pMorphTargetDataBindings;  ///< Current set of morph target bindings

	SuAnimationSet*   m_pCurrentAnimationSet; ///< Animation set to use to retrieve curves for animated effect parameters
	float m_fAnimationTime;                   ///< Time value to use for sampling animation curves for effect parameters

	const SuSubChunkState* m_pSubChunkState;  ///< Current sub-chunk state.  NULL if subchunk culling is disabled or if chunk doesn't support it

	bool m_bIsVisible;   ///< Chunk's current visibility state (on or off)
};


#endif // _SU_MODELCHUNK_H_
