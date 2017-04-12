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

#ifndef _SU_GPU_RESOURCE_H_
#define _SU_GPU_RESOURCE_H_


#include "SuArray.h"
#include "SuCountedPtr.h"

class SuGPUResourceView;
class SuGPUResourceViewDescription;
class SuGPUSamplingResourceView;
class SuGPURenderableResourceView;
class SuGPUDepthStencilResourceView;
class SuGPUUnorderedAccessView;

class SuMD5Hash;

struct SuGPUResourceDescription;

// tolua_begin
typedef SuCountedPtr<SuGPUResourceView> SuGPUResourceViewPtr;
typedef SuCountedPtr<SuGPUSamplingResourceView> SuGPUSamplingResourceViewPtr;
typedef SuCountedPtr<SuGPUDepthStencilResourceView> SuGPUDepthStencilResourceViewPtr;
typedef SuCountedPtr<SuGPURenderableResourceView> SuGPURenderableResourceViewPtr;
typedef SuCountedPtr<SuGPUUnorderedAccessView> SuGPUUnorderedAccessViewPtr;

// tolua_end

// For now implementing as an array, when the map implementation will be resolved / finished / happy, I'll use that instead
typedef SuArray< SuGPUResourceViewPtr >     SuGPUResourceViewArray;
typedef SuGPUResourceViewArray::iterator  SuGPUResourceViewArrayIterator;


//---------------------------------------------------------------------------------------------------------------------------------
/// \brief System-independent representation of a generic GPU resource
///
/// \ingroup GPUResourceManager
/// SuGPUResource provides an abstract interface for system-specific GPU resources. Each GPU resource
/// can be either a static or a dynamic resource and its behavior is determined at creation time. The behavior
/// type of a GPU resource cannot be modified during its life space. Dynamic GPU resources are used for various
/// rendering content (dynamic vertex or data buffers / renderable textures / depth stencil textures)
//---------------------------------------------------------------------------------------------------------------------------------
//tolua_begin
class SuGPUResource
{

public:

	/// Resource behavior type
	enum Behavior
	{
		GPU_RESOURCE_STATIC,    ///< This resource's contents are never modified after creation
		GPU_RESOURCE_DYNAMIC,   ///< This resource will be used for rendering into and will be modifiable at run-time
		GPU_RESOURCE_UPLOAD,    ///< This resource is written by CPU for GPU to read once or copy from
		GPU_RESOURCE_READBACK,  ///< This resource is written once or copied into by GPU and read on CPU

	}; // End of enum Behavior declaration

	/// Resource type 
	enum Type
	{
		GPU_RESOURCE_UNKNOWN = 0,        ///< Unknown resource type, reserved
		GPU_RESOURCE_BUFFER,             ///< Buffer resource type, can be used for data buffer, vertex buffer or index buffer
		GPU_RESOURCE_TEXTURE_2D_ARRAY,   ///< Texture resource type, default array dimension is 1 slice for texture images
		GPU_RESOURCE_TEXTURE_CUBE_ARRAY, ///< Texture cube resource type, default array dimension is 1 slice for std. cube maps
		GPU_RESOURCE_TEXTURE_3D          ///< Volumetric texture resource type

	}; // End of enum Type declaration

	/// Describes how which pipeline stages this resource can be bound to. Note that these enums can be 
	/// bitwise ORed together.
	enum BindType
	{
		BIND_VERTEX_BUFFER = 0x1L,       ///< Resource can be used as a vertex buffer
		BIND_INDEX_BUFFER = 0x2L,        ///< Resource can be used as an index buffer
		BIND_CONSTANT_BUFFER = 0x4L,     ///< Resource can be used as a constant buffer
		BIND_SHADER_RESOURCE = 0x8L,     ///< Resource can be used as a shader resource
		BIND_STREAM_OUTPUT = 0x10L,      ///< Resource can be used as a streaming output buffer
		BIND_RENDER_TARGET = 0x20L,      ///< Resource can be used as a rendertarget
		BIND_DEPTH_STENCIL = 0x40L,      ///< Resource can be used as a depth stencil
		BIND_UNORDERED_ACCESS = 0x80L    ///< Resource can be used as a UAV
	}; // End of enum BindType declaration

	/// State of a resource, which includes view states as well as copy source or destination states.
	/// For use with Transition() to serialize two types of GPU access.
	enum StateType
	{
		STATE_COMMON = 0,                 ///< Generic initial state
		STATE_VERTEX_BUFFER,              ///< Resource is read/written as a vertex buffer
		STATE_INDEX_BUFFER,               ///< Resource is read/written as an index buffer
		STATE_CONSTANT_BUFFER,            ///< Resource is read/written as a constant buffer
		STATE_PIXEL_SHADER_RESOURCE,      ///< Resource is read as a shader resource in a pixel shader
		STATE_NON_PIXEL_SHADER_RESOURCE,  ///< Resource is read as a shader resource in a non-pixel shader
		STATE_DEPTH_STENCIL_READ,         ///< Resource is read as a depth stencil
		STATE_INDIRECT_ARGUMENT,          ///< Resource is read as an indirect argument
		STATE_GENERIC_READ,               ///< Resource is used in a read
		STATE_COPY_SOURCE,                ///< Resource is read as the source of a copy
		STATE_PRESENT,                    ///< Resource is presented
		STATE_MULTISAMPLE_RESOLVE_SOURCE, ///< Resource is source of multisample subresource resolve
		STATE_MULTISAMPLE_RESOLVE_DEST,   ///< Resource is destination of multisample subresource resolve
		STATE_STREAM_OUTPUT,              ///< Resource is read/written as a streaming output buffer
		STATE_RENDER_TARGET,              ///< Resource is written as a rendertarget
		STATE_DEPTH_STENCIL_WRITE,        ///< Resource is written as a depth stencil
		STATE_UNORDERED_ACCESS,           ///< Resource is read/written as a UAV
		STATE_COPY_DEST,                  ///< Resource is written as a copy destination
	}; // End of enum StateType declaration

	/// Misc creation flags for hinting to driver about the usage of this resource
	enum MiscHint
	{
		MISC_HINT_NONE = 0x0L, ///< No hints
		MISC_HINT_SHARED_RESOURCE = 0x1L, ///< Enables the sharing of resource data between two or more devices
		MISC_HINT_DRAWINDIRECT_ARGS = 0x2L  ///< Buffer can be used as argument for indirect draws or dispatches
	}; // End of enum MiscHint

	/// Describes how this resource will be accessed by the CPU (write / read). Note that these flags can 
	/// be ORed. Also note that for certain resource behavior / type combinations only read-access is 
	/// allowed for CPU access and will be enforced.  CPU_ACCESS_PERFORMANT_READ is a hint that the resource will be
	/// read frequently, and that resource implementations should attempt to provide fast readback
	///  (for example, by caching any staging resources that are needed for the readback)
	enum CPUAccessType
	{
		CPU_ACCESS_WRITE = 0X10000L, ///< CPU has write access
		CPU_ACCESS_READ = 0x20000L, ///< CPU has read access
		CPU_ACCESS_PERFORMANT_READ = 0x40000L  ///< CPU has read access, and we would like it to be fast.

	};  // End of enum CPUAccessType declaration

	/// Options for read and write permissions for a mapped resource for access. Note that the resource
	/// must be first created with appropriate flags in order to use the right options.
	enum MapType
	{
		MAP_READ,                        ///< Map a resource and make it readable. Only allowed to be used with CPU_ACCESS_READ 
										 ///  specified resources.
		MAP_WRITE,                       ///< Map a resource and make it writeable. Can only be used if the resource is created
										 ///  with CPU_ACCESS_WRITE. Used for only overwriting parts of the buffer.
		MAP_READ_WRITE,                  ///< Map a resource and make it readable and writable. Both CPU_ACCESS_READ and 
										 ///  CPU_ACCESS_WRITE must be used to create resource in order to use this option
		MAP_WRITE_DISCARD,               ///< Map a resource, discard its contents and make it writable. Used for overwriting 
										 ///  the whole buffer. CPU_ACCESS_WRITE must be used during resource creation in order 
										 ///  to use this option
		MAP_WRITE_NO_OVERWRITE,          ///< Map a resource and make it writable.  The app promises to not overwrite any 
										 ///  previously written portions of the resource.  Only valid on resources created with 
										 ///  CPU_ACCESS_WRITE.  

	}; // End of enum MapType declaration

	/// Default constructor
	SU_DLL SuGPUResource();

	/// Constructor
	SU_DLL SuGPUResource(Behavior eBehavior, Type eType, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags);

	/// Destructor
	virtual SU_DLL ~SuGPUResource();

	/// Get API Handle to a GPU resource
	///
	/// \pre Resource has been successfully allocated
	/// \return An API-specific handle to the resource
	virtual SuHandle GetHandle() const = 0;

	/// Get the behavior type for this resource
	///
	/// \return Current behavior type
	inline const Behavior GetBehavior() const { return m_eBehavior; }

	/// Get the resource type of this resource (texture / buffer / etc)
	///
	/// \return Current resource type
	inline const Type GetType()   const { return m_eType; }

	/// Get bind flags for this resource
	inline const uint32 GetBindFlags() const { return m_nBindFlags; }

	/// Get CPU access flags for this resource
	inline const uint32 GetCPUAccessFlags() const { return m_nCPUAccessFlags; }

	/// Get Misc hint flags for this resource
	inline const uint32 GetMiscHintFlags() const { return m_nMiscHintFlags; }

	/// Returns true if this resource can be used according to a particular bind type option (i.e. usage type)
	SU_DLL bool QueryUsage(BindType eUsageOption) const;

	/// Helper method, allows querying whether a given bind type is found in bind flags
	static SU_DLL bool QueryBindUsage(BindType eUsageOption, uint32 nBindFlags);

	/// Returns true if this resource can be accessed according to particular CPU access option
	SU_DLL bool QueryCPUAccess(CPUAccessType eCPUAccessOption) const;

	/// Query Misc hint flag for the resource 
	SU_DLL bool QueryMiscHint(MiscHint eMiscHint) const;

	/// Unmap (and update) the resource that was previously mapped
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
	///
	/// \return True if no errors occurred during unmapping, false if trying to unmap a resource that hasn't been created previously
	virtual bool Unmap() = 0;

	/// Ensure completion of start state by GPU before transition to end state
	virtual bool Transition(StateType startState, StateType endState);

	/// Ensure completion of all previous UAV reads or writes, before subsequent UAV reads or writes occur
	virtual bool UAVBarrier();

    /// Copy entire resource
    virtual bool CopyResource(const SuGPUResource& srcResource);

    /// A convenience function which returns a sampling view of the entire resource
	virtual SuGPUSamplingResourceViewPtr GetDefaultSamplingView() = 0;

	/// A convenience function which returns a sampling view description of the entire resource
	virtual SuGPUResourceDescription GetDefaultResourceDesc() = 0;

	/// Returns a sampling view for this resource
	SU_DLL SuGPUSamplingResourceViewPtr GetSamplingView(const SuGPUResourceViewDescription& rDesc);

	/// Returns a renderable view for this resource
	SU_DLL SuGPURenderableResourceViewPtr GetRenderableView(const SuGPUResourceViewDescription& rDesc);

	/// Retrieves a UAV for this resource
	SU_DLL SuGPUUnorderedAccessViewPtr GetUnorderedAccessView(const SuGPUResourceViewDescription& rDesc);

	/// Returns a reference to the array containing the existing views for this object
	inline const SuGPUResourceViewArray& GetExistingViews() const { return m_viewArray; };

	/// Returns the name assigned to this resource.  This is the name that is given when creating resource from scripts.
	inline const SuString& GetName() const { return m_name; };

//tolua_end

	/// Sets the name assigned to this resource.  This should only be called by the GRM at resource creation time
	/// \param rName  A name to assign to the resource
	inline void SetName(const SuString& rName) { m_name = rName; };

	/// Sets the MD5 hash assigned to this resource.  This should only be called by the GRM at resource creation time
	/// \param pHash  A pointer to an MD5 hash that is associated with this resource.  The resource does NOT assume
	///        ownership of the hash.  The hash is simply stored with the resource for simplicity's sake
	inline void SetHash(const SuMD5Hash* pHash) { m_pHash = pHash; };

	/// Returns the MD5 hash associated with this resource
	inline const SuMD5Hash* GetHash() const { return m_pHash; };

	/// Returns a counter identifying when this resource was created.  As they are created, resources are numbered with a sequential counter
	/// This counter can be used to track down resource leaks by breaking when a particular resource is created
	inline uint32 GetAllocID() const { return m_nAllocID; };

protected:

	/// Interface for creating an API-specific sampling view
	virtual SuGPUSamplingResourceView* CreateSamplingView(const SuGPUResourceViewDescription& rDesc) = 0;

	/// Interface for creating an API-specific renderable view
	virtual SuGPURenderableResourceView* CreateRenderableView(const SuGPUResourceViewDescription& rDesc) = 0;

	/// Interface for creating an API-SPECIFIC unordered access view
	virtual SuGPUUnorderedAccessView* CreateUnorderedAccessView(const SuGPUResourceViewDescription& rDesc);

	/// Sub-class specific method to determine whether the resource can support a particular view
	virtual SU_DLL bool IsViewValid(const SuGPUResourceViewDescription& rDesc) const;

	/// Locates an existing view on this resource whose description matches the given description
	SU_DLL SuGPUResourceViewPtr FindResourceView(const SuGPUResourceViewDescription& rDesc);

	/// Adds a resource view to this resource
	SU_DLL bool AddResourceView(SuGPUResourceViewPtr pResourceView);

	/// Removes a resource view from this resource
	SU_DLL bool RemoveResourceView(SuGPUResourceViewPtr pResourceView);

	/// Verifies that a particular mapping mode is legal for this resource
	SU_DLL bool ValidateMapType(MapType eMapOption) const;

private:

	/// Helper method that is called by constructors to assign an alloc ID to the resource
	void SetAllocID();



	Behavior       m_eBehavior;       ///< GPU resource usage behavior 

	Type           m_eType;           ///< GPU resource type         

	uint32         m_nBindFlags;       ///< Bitwise combination of GPU resource bind flags

	uint32         m_nCPUAccessFlags;  ///< Bitwise combination of CPU access specification

	uint32         m_nMiscHintFlags;   ///< Bitwise combination of Misc hint flags

	SuGPUResourceViewArray  m_viewArray;   ///< An array of views that this resource uses for rendering

	bool           m_bIsMapped;       ///< This specified whether the resource is being mapped or not

	SuString       m_name;            ///< A globally unique name that can be used to identify this resource (e.g. in app-updates)

	const SuMD5Hash*     m_pHash;     ///< An MD5 hash that can be optionally assigned to a resource

	uint32         m_nAllocID;     ///< Numeric ID used to track resource leaks.  This is automatically assigned by the resource constructor

	/// Global resource counter
	static uint32 s_nAllocIDCounter;


//tolua_begin
}; // End of class SuGPUResource declaration
//tolua_end


#endif // End of _SU_GPU_RESOURCE_H_ definition