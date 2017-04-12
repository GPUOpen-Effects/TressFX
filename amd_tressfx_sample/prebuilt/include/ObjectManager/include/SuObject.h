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

#ifndef _SU_OBJECT_H_
#define _SU_OBJECT_H_

#include "SuDrawableObject.h"
#include "SuBoundingBox.h"
#include "SuBoundingSphere.h"
#include "SuFrustum.h"


class SuFrustum;
class SuBoundingBox;
class SuBoundingSphere;
class SuBoundingVolumeVisualizer;
class SuObjectListener;

//=========================================================================================================
/// \ingroup ObjectManager
/// \brief A base class for every object which is owned by the object manager.
/// 
//=========================================================================================================
// tolua_begin
class SuObject : public SuDrawableObject
{
public:
// tolua_end

	/// Default constructor
	SU_DLL SuObject(const SuString& rName);

	/// Destructor
	virtual SU_DLL  ~SuObject();

// tolua_begin

	/// Implements the SuDrawableObject interface.  \sa SuDrawableObject
	virtual bool Draw(uint32 nInstances = 1) = 0;

	/// Implements the SuDrawableObject interface.  \sa SuDrawableObject
	virtual bool Draw(const SuString& rEffectTechniqueName, uint32 nInstances = 1) = 0;

	/// Implements the SuDrawableObject interface.
#if 0
	// tolua only understands this syntax, which is not legal C++
	virtual bool Draw(uint32 nProperties, const SuString pProperties[nProperties], uint32 nInstances = 1) = 0;
	virtual bool HasMatchingTechnique(uint32 nProperties, const SuString pProperties[nProperties]) = 0;
#else
// tolua_end
	virtual bool Draw(uint32 nProperties, const SuString* pProperties, uint32 nInstances = 1) = 0;
	virtual bool HasMatchingTechnique(uint32 nProperties, const SuString* pProperties) = 0;
// tolua_begin
#endif


	/// Accessor for object name
	SU_DLL const SuString& GetName() const;

	/// Assigns a property
	SU_DLL void AddProperty(const SuString& rStrProperty);

	/// Test if we have particular property
	SU_DLL bool HasProperty(const SuString& rStrProperty) const;

	/// Returns the entire list of properties
	SU_DLL inline const SuStringArray& GetProperties() const { return m_properties; }

	// Returns a bool indicating if the object's bounding volume intersects the frustum
	SU_DLL bool IntersectsFrustum(const SuFrustum& rFrustum) const;

	/// Returns the object's current bounding sphere
	inline const SuBoundingSphere& GetBoundingSphere() const { return m_boundingSphere; }

	/// Returns the object's current bounding box
	inline const SuBoundingBox& GetBoundingBox() const { return m_boundingBox; }

	/// Performs time-dependent updates for this object
	/// \param fTime
	///   The current time setting
	virtual bool UpdateObject(SuTime fTime) = 0;

	/// Renders the object's bounding volume
	virtual SU_DLL void DrawBoundingVolumes(const SuBoundingVolumeVisualizer& rVis) const;

	/// Determines whether or not the object is currently visible
	virtual SU_DLL bool IsVisible() const;

	/// Set whether or not the object is currently visible
	virtual SU_DLL void SetVisible(bool bVisible);

	/// Update the object's bounding volume
	virtual SU_DLL void SetBoundingVolume(const SuBoundingBox& rBox);

// tolua_end

	// the following methods are an interface for supporting object-specific app-updates (bones, bounding volumes, etc)
	//  The default implementations will, for the most part, error out (animated bounding volumes are an aspect of the SuObject class and can be supported for all objects)
	//  Subclasses should override these methods if they want to support any of the object manager app-updates

	/// Interface for handling a 'bones' app update
	virtual SU_DLL bool HandleBoneAppUpdate(void* pBuffer, uint32 nBufferSize, bool bPrevFrame, bool bInverseTranspose, bool b4x3) const;

	/// Interface for handling a 'bounding box' app update
	virtual SU_DLL bool GetStaticBoundingBox(SuPoint3* pMin, SuPoint3* pMax) const;

	/// Interface for handling a 'bounding sphere' app update
	virtual SU_DLL bool GetStaticBoundingSphere(SuPoint3* pCenter, float* pRadius) const;

	virtual SU_DLL bool GetWorldTransform(SuMatrix4* pMatrix) const;

	/// Attaches a listener to this object
	SU_DLL void AttachListener(SuObjectListener* pListener);

	/// Detaches a listener from this object
	SU_DLL void DetachListener(SuObjectListener* pListener);


protected:

	/// Notifies all object listeners of an object update
	SU_DLL void NotifyListenersOfUpdate(SuTime fTime);

	/// Notifies all object listeners of an object reload
	SU_DLL void NotifyListenersOfReload();

private:

	/// Updates the object's bounding sphere information from the bounding box extents
	///  Should only be called from SetBoundingVolume
	void UpdateBoundingSphereFromBoundingBox();

	SuStringArray     m_properties;
	SuString          m_name;
	SuBoundingBox     m_boundingBox;
	SuBoundingSphere  m_boundingSphere;

	SuList< SuObjectListener* > m_listeners;

// tolua_begin
};
// tolua_end


#endif // _SU_OBJECT_H_
