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

#ifndef _SUEFFECTPASS_H
#define _SUEFFECTPASS_H

#include "SuTypes.h"
#include "SuRenderStateEnums.h"
#include "SuVector4.h"


// forward declarations
class SuEffectTechnique;
class SuStreamMap;
class SuGPUResource;
class SuTextureSlot;
class SuGPUResource;

/// \brief A render pass of an effect technique
///
/// \ingroup EffectManager
class SuEffectPass
{
public:

	/// The maximum number of stream output streams allowed for an effect pass
	static const uint32 MAX_SO_STREAM_MAPS = 4;

	/// Constructor
	SU_DLL SuEffectPass(const SuString& rName, const SuEffectTechnique* pParent);

	/// Destructor
	virtual SU_DLL ~SuEffectPass();

	/// Returns the pass name
	inline const SuString &GetName() const { return m_name; }

	/// Sets the stream map to use for this pass
	virtual SU_DLL void SetStreamMap(const SuStreamMap* pSMap) { SU_ASSERT(pSMap != NULL); m_pStreamMap = pSMap; }

	/// Sets the stream output stream map to use for this pass
	virtual SU_DLL void SetStreamOutMap(uint32 nStreamIdx, const SuStreamMap* pSMap);

	/// Returns a pointer to the stream map this pass uses
	inline const SuStreamMap* GetStreamMap() const { return m_pStreamMap; }

	/// Returns a pointer to the stream out map this pass uses for a particular SO stream
	inline const SuStreamMap* GetStreamOutMap(uint32 nStreamIdx = 0) const
	{
		SU_ASSERT(nStreamIdx < MAX_SO_STREAM_MAPS);
		return m_pStreamOutMap[nStreamIdx];
	};

	/// Returns the parent technique this pass belongs to
	inline const SuEffectTechnique *GetParent() const { return m_pParent; }

	/// Creates a copy of this effect pass with a new parent
	SU_DLL SuEffectPass* Clone(const SuString& rNewName, const SuEffectTechnique* pParentOfClone,
		bool bAllowIncompleteState = false) const;

	/// Begins using this pass for rendering. Sets all render state.
	virtual bool Begin() const = 0;

	/// Ends using this pass for rendering. Restores all render state.
	virtual bool End() const = 0;

	/// Sets a render state that takes a bool parameter
	virtual bool AddRenderState(SuRenderStateId stateID, bool enabled) = 0;

	/// Sets a render state that takes an int parameter
	virtual bool AddRenderState(SuRenderStateId stateID, int32 value) = 0;

	/// Sets a render state that takes a float parameter
	virtual bool AddRenderState(SuRenderStateId stateID, float32 value) = 0;

	/// Sets a render state that takes a vector parameter
	virtual bool AddRenderState(SuRenderStateId stateID, const SuVector4& rValue) = 0;

	/// Sets a render state that takes an enumerated parameter
	virtual bool AddRenderState(SuRenderStateId stateID, SuRenderStateValue value) = 0;

	/// Checks whether this effect pass is allowed to contain only an incomplete subset of renderstate
	///
	/// This is the case if the pass is part of a technique that's only used as a base class to other techniques.
	/// If this flag is set, the pass must not be used for rendering.
	inline bool AllowsIncompleteState() const { return m_bAllowIncompleteState; }

protected:
	/// Subclass-specific implementation for the Clone() method
	virtual SuEffectPass* CloneImpl(const SuEffectTechnique* pParentOfClone) const = 0;

private:
	/// Disable copy constructor
	SuEffectPass(const SuEffectPass &);

	bool m_bAllowIncompleteState;
	SuString m_name;
	const SuEffectTechnique* m_pParent;
	const SuStreamMap* m_pStreamMap;
	const SuStreamMap* m_pStreamOutMap[MAX_SO_STREAM_MAPS];
};

#endif