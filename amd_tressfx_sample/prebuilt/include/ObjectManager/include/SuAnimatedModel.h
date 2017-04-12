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

#ifndef _SU_ANIMATEDMODEL_H_
#define _SU_ANIMATEDMODEL_H_

#include "SuObject.h"
#include "SuArray.h"
#include "SuMatrix4.h"
#include "SuPoint3.h"
#include "SuBoundingBox.h"
#include "SuBoundingSphere.h"
#include "SuAnimationCurve.h"
#include "SuAutoDrawableObject.h"
#include "SuDispatchableObject.h"
#include "SuModel.h"
#include "SuModelChunk.h"
#include "SuEffectReloadListener.h"

class SuAnimationSet;
class SuModel;
class SuGPUVertexBuffer;
class SuVertexStreamDecl;
class SuMorphTargetBlender;

typedef SuCountedPtr<SuGPUVertexBuffer> SuGPUVertexBufferPtr;
typedef SuCountedPtr<SuMorphTargetBlender> SuMorphTargetBlenderPtr;

class SuBox;
class SuSphere;
class SuSkeleton;
class SuBoneAnimator;
class SuSubChunkState;

//=========================================================================================================
/// \ingroup ObjectManager 
/// \brief A subclass for an animated model, the most common type of Sushi object
/// 
/// The SuAnimatedModel object is responsible for tracking the animation state of a particular instance of
/// a model.  When the object is updated, new skinning matrices are computed for each of the model's bones
/// (if it has bones).  
///
/// In addition, if the effect file specifies it and the API supports it, the animated model can perform stream out skinning.
/// Stream out skinning is implemented by rendering the vertices of the object's vertex buffers as points and streaming out
/// 
/// The vertex buffer that is bound for the first chunk in each effect group is used as the source for stream out skinning.
/// Stream out skinning cannot be performed if the model uses bone re-mapping.
///
//=========================================================================================================
// tolua_begin
class SuAnimatedModel : public SuObject, public SuAutoDrawableObject, public SuDispatchableObject, public SuEffectReloadListener
{
public:
	/// Constructor for an animated model
	SU_DLL SuAnimatedModel(const SuString& rName, SuModel* pModel, SuAnimationSet* pAnimations);

	/// Destructor for an animated model
	SU_DLL ~SuAnimatedModel();

// tolua_end 
	/// Implements the SuDrawableObject interface.
	virtual SU_DLL bool Draw(uint32 nInstances);

	/// Implements the SuDrawableObject interface.
	virtual SU_DLL bool Draw(const SuString& rEffectTechniqueName, uint32 nInstances);

	/// Implements the SuDrawableObject interface.
	virtual SU_DLL bool Draw(uint32 nProperties, const SuString* pProperties, uint32 nInstances);

	/// Implements the SuDrawableObject interface
	virtual SU_DLL bool HasMatchingTechnique(uint32 nProperties, const SuString* pProperties);

	/// Implements the SuDispatchableObject interface
	virtual SU_DLL bool HasMatchingComputeTechnique(uint32 nProperties, const SuString* pProperties);

// tolua_begin

	/// Implements the SuDispatchableObject interface.
	virtual SU_DLL bool Dispatch(const SuString& rEffectTechniqueName, int nX, int nY, int nZ);

	/// Draws the animated model using a particular technique of the object's effect using 
	/// data written to a streaming output buffer. 
	virtual SU_DLL bool DrawAuto(SuRenderManager::PrimType ePrimType, const SuString& rEffectTechniqueName);

	/// Draws the animated model using a particular technique of the object's effect using data written to a 
	/// streaming output buffer. Note that this type of draw call will only work in DirectX10.
	virtual SU_DLL bool DrawAuto(const SuString& rEffectTechniqueName);

	/// Performs time-dependent updates for this object
	virtual SU_DLL bool UpdateObject(SuTime fTime);

	/// Draws the vertices of this model as points, using the specified technique
	SU_DLL bool DrawVerticesAsPoints(const SuString& rTechnique, uint32 nInstanceCount = 1);

	/// Draws multiple instances of this model using DrawInstanced() calls
	SU_DLL bool DrawInstances(const SuString& rTechnique, uint32 nInstances);

#if 0
	// tolua only understands this syntax, which is not legal C++
	SU_DLL bool DrawInstances(uint32 nProperties, const SuString pProperties[nProperties], uint32 nInstances);
#else
// tolua_end
	/// Draws multiple instances of this model using DrawInstanced() calls
	SU_DLL bool DrawInstances(uint32 nProperties, const SuString* pProperties, uint32 nInstances);
// tolua_begin
#endif

   /// Returns whether the object's visibility flag is currently on or off
	SU_DLL bool IsVisible() const { return !m_bIsHidden; }

	/// Set whether or not the object is currently visible
	SU_DLL void SetVisible(bool bVisible);

	/// Returns the length of the longest animation for this object.
	SU_DLL float GetAnimationLength() const;

	/// Set the world transform of the model
	SU_DLL void SetWorldTransform(const SuMatrix4& rWorldTransform);

	/// Get the world transform of the model
	virtual SU_DLL bool GetWorldTransform(SuMatrix4 *pMatrix) const { *pMatrix = m_mWorld; return true; }

	/// Returns the obejct's animation set
	inline const SuAnimationSet* GetAnimationSet() const { return m_pAnimationSet; };

	/// Returns the object's skeleton
	SU_DLL const SuSkeleton* GetSkeleton() const;

	/// Determines if view-dependent sorting is being performed by this object
	SU_DLL bool IsSortingEnabled() const { return m_bSortingEnabled; };

	/// Turns view-dependent sorting on or off.
	SU_DLL void SetSortingEnabled(bool bSorting) { m_bSortingEnabled = bSorting; };

	/// Returns the object's model
	inline const SuModel* GetModel() const { return m_pModel; };

	/// Toggles subchunking on or off.  
	/// \param bEnabled  If true, subchunking will be performed (if subchunk information is available).  If false, subchunking is not performed
	inline void SetSubChunking(bool bEnabled) { m_bSubChunkingEnabled = bEnabled; };

	inline const SuArray<SuMatrix4> GetSkinningMatrices()
	{
		SU_ASSERT(m_pSkinningMatrices);
		return *m_pSkinningMatrices;
	}

	/// Returns the current transformation matrix for a particular bone.  The transform is in world space
	inline const SuMatrix4& GetBoneTransform(uint32 nBoneIndex) const
	{
		SU_ASSERT(m_pSkinningMatrices);
		if (m_bWorldTransform)
			return m_pSkinningMatrices->at(nBoneIndex);
		else
			return m_pSkinningMatricesOS->at(nBoneIndex);
	};

	/// Returns number of morph blenders associated with the model.
	SU_DLL uint32 GetNumBlenders() const { return static_cast<uint32>(m_morphBlenders.size()); }

	/// Gets number of blend shapes in the indexed blender.  
	/// \note This is an accessor to the indexed SuMorphTargetBlender object, whose interface is not public.
	SU_DLL uint32 GetNumBlendShapes(uint32 nBlender) const;

	/// Zeros all blend weights in the indexed blender.
	/// \note This is an accessor to the indexed SuMorphTargetBlender object, whose interface is not public.
	SU_DLL void ZeroBlendWeights(uint32 nBlender);

	/// Gets the indexed blend weight of the indexed blender.
	/// \note This is an accessor to the indexed SuMorphTargetBlender object, whose interface is not public.
	SU_DLL float32 GetBlendWeight(uint32 nBlender, uint32 nIndex) const;

	/// Sets the indexed blend weight of the indexed blender.
	/// \note This is an accessor to the indexed SuMorphTargetBlender object, whose interface is not public.
	SU_DLL void SetBlendWeight(uint32 nBlender, uint32 nIndex, float32 fWeight);

	/// Accumulates all active blendshapes for a given time and updates the blendshape buffer for the indexed blender.
	/// \note This is an accessor to the indexed SuMorphTargetBlender object, whose interface is not public.
	SU_DLL void AccumulateBlendShapes(uint32 nBlender);


// tolua_end

	/// Returns the array of bone matrices to use to render the current chunk
	virtual bool HandleBoneAppUpdate(void* pBuffer, uint32 nBufferSize, bool bPrevFrame, bool bInverseTranspose, bool b4x3) const;

	/// Interface for handling a 'bounding box' app update
	virtual bool GetStaticBoundingBox(SuPoint3* pMin, SuPoint3* pMax) const;

	/// Interface for handling a 'bounding sphere' app update
	virtual bool GetStaticBoundingSphere(SuPoint3* pCenter, float* pRadius) const;

	/// Called on effect re-load
	virtual SU_DLL void OnEffectReload(SuEffectPtr pOldEffect, SuEffectPtr pNewEffect);

	/// Called to signal the start of an object re-load.  The object will clear all of its internal state.
	SU_DLL void StartObjectReload();

	/// Called to signal the end of an object re-load.  The object will re-initialize its internal state
	SU_DLL void EndObjectReload();

	/// Samples bounding volume animations for the given time value
	SU_DLL void UpdateBoundingVolume(float fTime);


	/// \brief Performs cache warming for this object
	/// Cache warming is the process of drawing one triangle for every effect pass in order to avoid things like draw-time shader compiles and resource swapping
	virtual SU_DLL void WarmGPUCaches();

	/// Renders the object's bounding volume
	virtual SU_DLL void DrawBoundingVolumes(const SuBoundingVolumeVisualizer& rVis) const;

	/// Fills an array with the object's current bone transformations, in world space
	SU_DLL void GetBonesColumnMajor(void* pBuffer);

	/// Control whether or not enable to update bone animation and morphing 
	SU_DLL void ToggleAnimation(bool bAnimation);

	// Query if updating animation is enabled or not. 
	SU_DLL bool IsAnimationEnabled() const;

private:

	/// Time threshold for extrapolating bones
	static const SuTime EXTRAPOLATE_TIME_THRESHOLD;

	/// Helper method which initializes bounding volume information for this object
	void InitBoundingVolume();

	/// Helper method which locates bone animation curves in the object's animation set
	void FindBoneAnimationCurves();

	/// Helper method to locate the visibility animation curves for this object
	void FindVisibilityCurves();

	/// Helper method which sets up the data structures used for stream out skinning
	void SetupStreamOutSkinning();

	/// Helper method which sets up the data structures used for morph target animation
	void SetupMorphTargetAnimation();

	/// Creates a vertex buffer to be used as a stream-output skinning target
	SuGPUVertexBufferPtr CreateSkinTarget(uint32 nVertexCount, uint32 nVertexSize);

	/// Samples visibility curves for object and chunks, and updates the 'm_bHidden' flag
	void UpdateVisibility(SuTime fTime);

	/// Updates the bones transforms for the object
	void UpdateSkinningMatrices(SuTime fTime, bool bForceBoneExtrapolation);

	/// Samples bone animation curves for the object, and either stores previous frame bones or extrapolates them
	void UpdateObjectSpaceBones(SuTime fTime, bool bForceBoneExtrapolation);

	/// Applies the object's world transform to the sampled object space bones, and re-computes inverse-transpose bones as needed
	void UpdateWorldSpaceBones();

	/// Helper method that computes the inverse-transpose for an array of matrices
	void ComputeInverseTranspose(const SuArray<SuMatrix4>& rXForms, SuArray<SuMatrix4>& rXFormsOut);

	/// Helper method that tests whether the bones for the previous frame need to be extrapolated
	bool NeedBoneExtrapolation(SuTime fPrevTime, SuTime fTime);

	/// Method that extrapolates bones for the previous time
	void ExtrapolateBones(SuTime fPrevTime, SuTime fCurrTime, const SuArray<SuMatrix4>& rCurrentBones, SuArray<SuMatrix4>& rBonesOut);

	/// Helper method which sets per-object chunk state.  
	void UpdatePerObjectChunkState();

	/// Sets an array of bones to identity
	void SetBonesToIdentity(void* pBuffer, uint32 nBufferSize, bool b4x3) const;

	/// Checks if a vertex stream decl is for morph data
	bool IsMorphDataVertexStream(const SuVertexStreamDecl& rStream) const;

	/// Searches for a texture slot that is supposed to have morph data and returns its name
	bool GetMorphTextureSlotName(SuEffectPtr pEffect, const SuString& rMorphStreamName, SuString* pMorphTextureSlotName) const;

	/// Finds the morph target info object for a particular vertex stream of an effect
	SuMorphTargetInfoPtr FindMorphTargetInfo(const SuString& rEffectFileName, const SuString& rVertexStreamName) const;

	/// Performs stream out skinning.  Does nothing if the object does not have stream-out skinning turned on
	void DoStreamOutSkinning();

	/// Calculates blend shape animation for a particular time
	void DoMorphing(SuTime fTime);

	/// Locates morph target animation curves from an animation set
	void FindMorphAnimationCurves(SuArray<SuAnimationCurvePtr>& rCurvesOut, const SuMorphTargetInfo& rMTI, SuAnimationSet* pAnimSet);

	/// Destroys the bone animators
	void DestroyBoneAnimators();

	/// Sets up subchunk states for this object
	void SetupSubChunkState();

	/// Updates subchunk state based on current bones
	void UpdateSubChunkState();

	/// Destroyus subchunk states for this object
	void DestroySubChunkState();

	/// Cleans up morphing state
	void DestroyMorphState();

	SuModel* m_pModel;

	SuArray<SuMatrix4>* m_pSkinningMatricesOS;            ///< Object-space skinning matrices, sampled from bone animation curves
	SuArray<SuMatrix4>* m_pSkinningMatricesPrevOS;        ///< Last frame's object-space skinning matrices, sampled from bone animatino curves
	SuArray<SuMatrix4>* m_pSkinningMatrices;              ///< Current skinning matrix for each bone in the model's skeleton (world space)
	SuArray<SuMatrix4>* m_pInverseSkinningMatrices;       ///< Current inverse skinning matrices, if they are needed by the shader (world-space)
	SuArray<SuMatrix4>* m_pSkinningMatricesPrev;          ///< Previous frame skinning matrices, if needed (world-space)
	SuArray<SuMatrix4>* m_pInverseSkinningMatricesPrev;   ///< Previous frame inverse skinning matrices, if needed (world-space)

	SuMatrix4 m_mWorld;      ///< World transform applied after skinning
	bool m_bWorldTransform;  ///< Is there a world transform?

	///The chunk that is currently being drawn.  This is needed in order to implement the bone app-update
	SuModelChunk* m_pDrawingChunk;


	/// A struct to hold stream-out skinning data for a particular chunk
	struct SoSkinningInfo
	{
		SuModelChunk* pChunk;                        ///< The chunk to perform SO Skinning on
		SuString skinningTechnique;                  ///< The name of skinning technique in the effect file
		SuArray<SuGPUVertexBufferPtr> skinTargets;   ///< Array of vertex buffers used as skinning targets for this object
	};

	SuArray<SoSkinningInfo> m_soSkinningInfo;    ///< Set of chunks that should be processed during stream-out skinning

	/// A struct to hold morph target animation data for a particular chunk
	struct MorphInfo
	{
		SuModelChunk* pChunk;                                       ///< The chunk to perform morph target animation on
		SuArray<SuModelChunk::MorphDataBindingInfo> morphBindings;  ///< Information about how to bind morph data to the chunk
	};

	SuArray<MorphInfo> m_morphTargetInfo;                          ///< Set of chunks that have morph target animation
	SuArray<SuMorphTargetBlenderPtr> m_morphBlenders;              ///< Array of morph target blenders.  There is one for each SuMorphTargetInfo in the model.

	SuAnimationSet* m_pAnimationSet;                               ///< Animation set used by this object
	SuArray< SuArray<SuAnimationCurvePtr> > m_morphAnimationCurves;///< Sets of animation curves for the blend shapes, one for each morph target blender
	SuArray<SuBoneAnimator*> m_boneAnimators;                      ///< Array of animation curves for each bone in the model's skeleton
	SuAnimationCurvePtr m_pBBMinCurve;                             ///< Animation curve for bounding box extents
	SuAnimationCurvePtr m_pBBMaxCurve;                             ///< Animation curve for bounding box extents

	SuAnimationCurvePtr m_pVisibilityCurve;                        ///< Animation curve for object visibility
	SuArray<SuAnimationCurvePtr> m_chunkVisibilityCurves;          ///< Visibility curves for chunks
	SuArray<bool> m_chunkVisibility;                               ///< Current chunk visibility values

	SuArray< SuPair<uint32, SuSubChunkState*> > m_subChunkStates;   ///< Subchunk states for each chunk in the model that has subchunks.  This is a pair mapping chunk index to subchunk state

	float m_fAnimationLength;
	SuTime m_fTime;               ///< Time of last update
	bool m_bIsHidden;             ///< Is the object currently visible or not (according to the visibility curve)
	mutable bool m_bShowedBoneWarning;    ///< Have we shown the warning about not having bones for a bone app-update???
	bool m_bSortingEnabled;       ///< Flag to control whether or not view-dependent sorting is performed
	bool m_bSubChunkingEnabled;   ///< Flag to control whether or not sub-chunking is turned on
	bool m_bEnableAnimation;      ///< Flage to control whether or not enable to update bone animation and morphing. 

// tolua_begin
};
// tolua_end

#endif // _SU_ANIMATEDMODEL_H_
