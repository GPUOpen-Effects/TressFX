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

#ifndef _SU_TEXTURE_SLOT_H_
#define _SU_TEXTURE_SLOT_H_

#include "SuCountedPtr.h"
#include "SuEffectManagerCommon.h"
#include "SuTextureSlotBinder.h"
#include "SuGPUSamplingResourceView.h"

class SuAnnotationMap;

/// \brief Texture slot description
///
/// \ingroup EffectManager
class SuTextureSlot
{
public:
	enum Type
	{
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBE,
		TEXTURE_2D_ARRAY,
		TEXTURE_CUBE_ARRAY,
		TEXTURE_BUFFER,
		TEXTURE_BUFFER_STRUCTURED,
		TEXTURE_BUFFER_RAW
	};

	/// Constructor
	SU_DLL SuTextureSlot(const SuString& rName, SuTextureSlot::Type type, SuAnnotationMap *annotations);

	/// Destructor
	virtual SU_DLL ~SuTextureSlot();

	/// Returns the texture type
	inline const Type& GetType() const { return m_type; }

	/// Returns the texture name
	inline const SuString& GetName() const { return m_name; }

	/// Returns the texture annotations
	inline const SuAnnotationMap* GetAnnotations() const { return m_annotations; }

	/// Binds a sampling resource view to the slot
	inline void BindResource(const SuCountedPtr< SuGPUSamplingResourceView >& rpTexture) const { m_pTexture = rpTexture; };

	/// Returns the sampling view that is bound to the slot.
	inline const SuCountedPtr< SuGPUSamplingResourceView >& GetBoundResource() const { return m_pTexture; };

	/// Returns the binder that is associated with this slot
	inline const SuTextureSlotBinderPtr GetBinder() const { return m_pBinder; };

	/// Removes the binder that is associated with this slot
	inline void RemoveBinder() { m_pBinder = SuTextureSlotBinderPtr(0); };

private:

	friend class SuEffectBuilder; // So that SuEffectBuilder may call 'SetBinder()'

	/// Sets the binder that is associated with this slot
	inline void SetBinder(SuTextureSlotBinderPtr pBinder) { m_pBinder = pBinder; };

	/// Texture name
	SuString m_name;

	/// Texture type
	Type m_type;

	/// Texture annotations
	SuAnnotationMap *m_annotations;

	/// Currently bound texture
	mutable SuCountedPtr<SuGPUSamplingResourceView> m_pTexture;

	/// Associated binder object
	SuTextureSlotBinderPtr m_pBinder;

};


#endif   // End of _SU_TEXTURE_SLOT_H_
