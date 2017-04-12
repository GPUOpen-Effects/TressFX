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

#ifndef _SUGPURESOUCEMANAGER_H
#define _SUGPURESOUCEMANAGER_H

#include "SuTypes.h"
#include "SuGPUResource.h"
#include "SuGPUTexture.h"
#include "SuMemoryBuffer.h"
#include "SuHashMap.h"
#include "SuMD5Hash.h"
#include "SuCountedPtr.h"
#include "SuGPURenderableResourceView.h"
#include "SuGPUDepthStencilResourceView.h"
#include "SuGPUResourceStats.h"

// forward declarations
class SuGPUVertexBuffer;
class SuGPUStructuredBuffer;
class SuGPUIndexBuffer;
class SuConstantBuffer;
class SuGPUTexture2DArray;
class SuGPUTexture3D;
class SuGPUTextureCubeArray;
class SuConstantBuffer;
class SuGPUTexture2DArrayClearInfo;

typedef SuCountedPtr< SuConstantBuffer > SuConstantBufferPtr;

// tolua_begin
typedef SuCountedPtr< SuGPUResource > SuGPUResourcePtr;
typedef SuCountedPtr< SuGPUVertexBuffer > SuGPUVertexBufferPtr;
typedef SuCountedPtr< SuGPUStructuredBuffer > SuGPUStructuredBufferPtr;
typedef SuCountedPtr< SuGPUIndexBuffer > SuGPUIndexBufferPtr;

typedef SuCountedPtr< SuGPUTexture > SuGPUTexturePtr;
typedef SuCountedPtr< SuGPUTexture2DArray > SuGPUTexture2DArrayPtr;
typedef SuCountedPtr< SuGPUTextureCubeArray > SuGPUTextureCubeArrayPtr;
typedef SuCountedPtr< SuGPUTexture3D > SuGPUTexture3DPtr;

typedef SuCountedPtr< SuGPURenderableResourceView > SuGPURenderableResourceViewPtr;
typedef SuCountedPtr< SuGPUDepthStencilResourceView > SuGPUDepthStencilResourceViewPtr;
typedef SuCountedPtr< SuMemoryBuffer > SuMemoryBufferPtr;
// tolua_end

/// \defgroup GPUResourceManager GPU Resource Manager

//=================================================================================================================================

/// \brief System-independent interface for GPU resouce management
/// \ingroup GPUResourceManager
/// SuGPUResourceManager provides an abstract interface for system-specific
/// GPU resource management tasks.  It follows the singleton pattern
/// and instantiates the appropriate SuGPUResourceManager subclass for the
/// platform.
//=================================================================================================================================

// tolua_begin
class SuGPUResourceManager : public SuSharedObjectOwner<SuGPUResource>
{
// tolua_end

protected:
	//=================================================================================================   
	/// Constructor
	//=================================================================================================   
	SU_DLL SuGPUResourceManager();

public:
// tolua_begin

	//=================================================================================================   
	/// Destructor
	//=================================================================================================   
	virtual SU_DLL ~SuGPUResourceManager();

	//=================================================================================================      
	/// Singleton access
	///
	/// \return Pointer to instance of GPU resource manager
	//=================================================================================================      
	static SU_DLL SuGPUResourceManager* GetPtr();

	//=================================================================================================      
	/// Singleton access
	///
	/// \return Reference to instance of GPU resource manager
	//=================================================================================================      
	static SU_DLL SuGPUResourceManager& GetRef();

	//=================================================================================================      
	/// Delete singleton instance
	//=================================================================================================      
	static SU_DLL void ShutDown();


	//=================================================================================================================================
	/// The initialize method performs API-specific initialization for the GPU resource manager.
	/// The main job of this method is to create resource objects to wrap the back buffer and Z buffer so that
	/// views can be created of them.
	///
	/// \param nWindowWidth The width of the backbuffer in pixels
	/// \param nWindowHeight The height of the backbuffer in pixels
	//=================================================================================================================================
	virtual SU_DLL void Initialize(uint32 nWindowWidth, uint32 nWindowHeight) {};

	//=================================================================================================================================
	/// Initialize() must be called before calling this method.
	/// \return A pointer to a GPUResource object which represents the back buffer.
	///   The GPU resource manager retains ownership of the returned object.
	//=================================================================================================================================
	virtual SU_DLL SuGPUTexture2DArray* GetBackBuffer();

	//=================================================================================================================================
	/// Returns a renderable view of the back buffer (for convenience)
	//=================================================================================================================================
	virtual SU_DLL SuGPURenderableResourceViewPtr GetBackBufferView() const;

	//=================================================================================================================================
	/// Returns a pseudo-view of the left or right back buffer for stereo
	//=================================================================================================================================
	virtual SU_DLL SuGPURenderableResourceViewPtr GetStereoBackBufferView(SuGPUStereoBackBufferEye eye) const;

	//=================================================================================================================================
	/// Initialize() must be called before calling this method.
	/// \return A pointer to a GPU resource representing the "default" Z buffer.  
	///   The GPU resource manager retains ownership of the returned object
	//=================================================================================================================================
	virtual SU_DLL SuGPUTexture2DArray* GetDefaultZBuffer();

	//=================================================================================================================================
	/// Returns a depth-stencil view of the default Z buffer (for convenience)
	//=================================================================================================================================
	virtual SU_DLL SuGPUDepthStencilResourceViewPtr GetDefaultZBufferView() const;

	/// Create a renderable vertex buffer resource.   
	SU_DLL SuGPUVertexBufferPtr CreateRenderableVB(SuGPUResourceFormat eElementFormat,
		uint16 nRTWidth,
		uint16 nRTHeight,
		uint32 nNumTexelsPerVertex = 1,
		bool bAllowSampling = false,
		const SuString& rName = "",
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);



	/// Create vertex buffer resource        
	SU_DLL SuGPUVertexBufferPtr CreateResourceVertex(SuGPUResource::Behavior eBehavior,
		uint32                  nVertexSize,
		uint32                  nNumVerts,
		SuMemoryBufferPtr       pInitialData,
		uint32                  nBindFlags = SuGPUResource::BIND_VERTEX_BUFFER,
		const SuString& rName = "",
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);


	/// Create index buffer resource
	SU_DLL SuGPUIndexBufferPtr CreateResourceIndex(SuGPUResource::Behavior eBehavior,
		uint32                  nIndexSize,
		uint32                  nNumIndices,
		SuMemoryBufferPtr       pInitialData,
		uint32 nBindFlags = SuGPUResource::BIND_INDEX_BUFFER,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);


	/// Create a constant buffer
	SU_DLL SuConstantBufferPtr CreateConstantBuffer(SuGPUResource::Behavior eBehavior,
		uint32                  nSizeInBytes,
		SuMemoryBufferPtr       pInitialData,
		bool bUseShadowBuffers,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);

	/// Creates a 2D texture array
	SU_DLL SuGPUTexture2DArrayPtr CreateTexture2DArray(SuGPUResource::Behavior eBehavior,
		uint32 nBindFlags,
		uint32 nCPUAccessFlags,
		SuGPUResourceFormat eFormat,
		SuGPUTexture::MipType eMipType,
		uint8 nMipLevels,
		uint16 nWidth,
		uint16 nHeight,
		uint16 nArraySize,
		uint8  nSampleCount,
		uint8  nQualityLevel,
		SuMemoryBufferPtr pInitialData = SuMemoryBufferPtr(NULL),
		const SuString& rName = "",
		const SuMD5Hash* pDataHash = NULL,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE,
		const SuGPUTexture2DArrayClearInfo* pClearInfo = NULL);



	/// Creates a cube map
	SU_DLL SuGPUTextureCubeArrayPtr CreateCubemapArray(SuGPUResource::Behavior eBehavior,
		uint32 nBindFlags,
		uint32 nCPUAccessFlags,
		SuGPUResourceFormat eFormat,
		SuGPUTexture::MipType eMipType,
		uint8 nMipLevels,
		uint16 nEdgeWidth,
		uint16 nArraySize,
		uint8  nSampleCount,
		uint8  nQualityLevel,
		SuMemoryBufferPtr pInitialData = SuMemoryBufferPtr(NULL),
		const SuString& rName = "",
		const SuMD5Hash* pDataHash = NULL,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);


	/// Creates a volume texture
	SU_DLL SuGPUTexture3DPtr CreateTexture3D(SuGPUResource::Behavior eBehavior,
		uint32 nBindFlags,
		uint32 nCPUAccessFlags,
		SuGPUResourceFormat eFormat,
		SuGPUTexture::MipType eMipType,
		uint8 nMipLevels,
		uint16 nWidth,
		uint16 nHeight,
		uint16 nDepth,
		SuMemoryBufferPtr pInitialData = SuMemoryBufferPtr(NULL),
		const SuString& rName = "",
		const SuMD5Hash* pDataHash = NULL,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);

	/// Creates a structured buffer
	SU_DLL SuGPUStructuredBufferPtr CreateStructuredBuffer(uint32 nStructSize,
		uint32 nNumStructs,
		uint32 nBindFlags,
		SuMemoryBufferPtr pInitialData,
		const SuString& rName,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE,
		uint32 nNumMaxStagingBuffers = 1);

	/// Returns the number of textures that are being managed by the GRM
	SU_DLL uint32 GetTextureCount() const;

	/// Iterates over the textures that are being managed by the GRM
	SU_DLL SuGPUTexturePtr GetTexture(uint32 nIndex) const;

	/// Returns the resource that is associated with a particular name
	SU_DLL SuGPUResourcePtr GetNamedResource(const SuString& rResourceName) const;

	/// Returns a statistics object that is maintained by the GRM
	inline SuGPUResourceStats& GetResourceStats() { return m_resourceStats; };

	/// Warms GPU caches by clearing all render targets
	virtual SU_DLL void WarmGPUCaches();


// tolua_end

	/// Method that is called by counted pointers to notify the GRM that it holds the last reference to a resource
	virtual SU_DLL void NotifyLastReference(SuGPUResource* pResource);

	/// Returns the resource (if any) which has a given MD5 hash
	SU_DLL SuGPUResourcePtr GetHashedResource(const SuMD5Hash& rResourceHash) const;

protected:


	/// Create an API-specific renderable vertex buffer
	virtual SU_DLL SuGPUVertexBuffer* APICreateRenderableVB(SuGPUResourceFormat eElementFormat, uint16 nRTWidth, uint16 nRTHeight,
		uint32 nNumTexelsPerVertex, bool bAllowSampling, uint32 nMiscHintFlags);

	/// Creates an API-specific vertex buffer
	virtual SU_DLL SuGPUVertexBuffer* APICreateResourceVertex(SuGPUResource::Behavior eBehavior,
		uint32                  nVertexSize,
		uint32                  nNumVerts,
		SuMemoryBufferPtr       pInitialData,
		uint32                  nBindFlags = SuGPUResource::BIND_VERTEX_BUFFER,
		uint32                  nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);

	/// Creates an API-specific structured buffer
	virtual SU_DLL SuGPUStructuredBuffer* APICreateStructuredBuffer(uint32                  nStructSize,
		uint32                  nNumStructs,
		uint32                  nBindFlags,
		SuMemoryBufferPtr       pInitialData,
		uint32                  nMiscHintFlags = SuGPUResource::MISC_HINT_NONE,
		uint32                  nNumMaxStagingBuffers = 1);

	/// Creates an API-specific index buffer
	virtual SU_DLL SuGPUIndexBuffer* APICreateResourceIndex(SuGPUResource::Behavior eBehavior,
		uint32                  nIndexSize,
		uint32                  nNumIndices,
		SuMemoryBufferPtr       pInitialData,
		uint32                  nBindFlags,
		uint32                  nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);


	/// Creates an API-specific constant buffer
	virtual SU_DLL SuConstantBuffer* APICreateConstantBuffer(SuGPUResource::Behavior eBehavior,
		uint32                  nSizeInBytes,
		SuMemoryBufferPtr       pInitialData,
		bool                    bShadowBuffers,
		uint32                  nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);


	/// Creates an API-specific texture array
	virtual SU_DLL SuGPUTexture2DArray* APICreateTexture2DArray(SuGPUResource::Behavior eBehavior,
		uint32 nBindFlags,
		uint32 nCPUAccessFlags,
		SuGPUResourceFormat eFormat,
		SuGPUTexture::MipType eMipType,
		uint8 nMipLevels,
		uint16 nWidth,
		uint16 nHeight,
		uint16 nArraySize,
		uint8  nSampleCount,
		uint8  nQualityLevel,
		SuMemoryBufferPtr pInitialData,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE,
		const SuGPUTexture2DArrayClearInfo* pClearInfo = NULL);


	/// Creates an API-specific cube texture
	virtual SU_DLL SuGPUTextureCubeArray* APICreateCubemapArray(SuGPUResource::Behavior eBehavior,
		uint32 nBindFlags,
		uint32 nCPUAccessFlags,
		SuGPUResourceFormat eFormat,
		SuGPUTexture::MipType eMipType,
		uint8 nMipLevels,
		uint16 nEdgeWidth,
		uint16 nArraySize,
		uint8  nSampleCount,
		uint8  nQualityLevel,
		SuMemoryBufferPtr pInitialData,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);

	/// Creates an API-specific 3D texture
	virtual SU_DLL SuGPUTexture3D* APICreateTexture3D(SuGPUResource::Behavior eBehavior,
		uint32 nBindFlags,
		uint32 nCPUAccessFlags,
		SuGPUResourceFormat eFormat,
		SuGPUTexture::MipType eMipType,
		uint8 nMipLevels,
		uint16 nWidth,
		uint16 nHeight,
		uint16 nDepth,
		SuMemoryBufferPtr pInitialData,
		uint32 nMiscHintFlags = SuGPUResource::MISC_HINT_NONE);


	/// Helper method to verify that the bind flags passed for resource creation are supported
	bool SU_DLL ValidateBindFlags(uint32 nBindFlags, uint32 nSupportedFlags) const;

   /// Sets the resource objects that represent the default back-buffer and Z-buffer
   void SU_DLL SetDefaultResources( SuGPUTexture2DArray* pBackBuffer, SuGPUTexture2DArray* pZBuffer, 
                             SuGPUResourceFormat eColorViewFormat, SuGPUResourceFormat eDepthViewFormat );


private:

	/// Helper function to update the GRM's internal data structures when a new resource is created
	SuGPUResourcePtr AddResourceToGRM(SuGPUResource* pResource, const SuString& rName, const SuMD5Hash* pHash);

	static SuGPUResourceManager* m_pInstance;       ///< Singleton GPU manager instance

	typedef SuList<SuGPUResourcePtr>::iterator ResourceListIterator;

	SuList< SuGPUResourcePtr > m_resources; ///< List of user-created resources which are managed by the GRM

	/// An array pointers to the textures in the resource list.  This is used for iterating over textures
	SuArray< ResourceListIterator > m_textures;

	/// A hash map of named resources for fast name lookup.  This maps the name of a resource to its counted pointer
	typedef SuHashMap< SuString, ResourceListIterator, SuStringHasher, SuHashComparer<SuString> > SuNamedResourceMap;
	SuNamedResourceMap m_namedResourceMap;

	/// MD5 hashes for resources that have them.  The resource objects contain pointers back to the hashes in this list
	SuList< SuMD5Hash > m_resourceHashes;

	/// A hash map for resources which have an associated hash.  This maps MD5 hashes to the resources which use a particular hash
	typedef SuHashMap< SuMD5Hash, ResourceListIterator, SuMD5Hash::Hasher, SuHashComparer<SuMD5Hash> > SuHashedResourceMap;
	SuHashedResourceMap m_hashedResourceMap;



	SuGPUTexture2DArray* m_pBackBuffer;       ///< A GPUResource object that represents the back buffer
	SuGPUTexture2DArray* m_pDefaultZBuffer;   ///< A GPUResource object that represents the 'default' Z buffer

	SuGPURenderableResourceViewPtr m_pBackBufferView;        ///< A "default" renderable view of the back buffer
	SuGPUDepthStencilResourceViewPtr m_pDefaultZBufferView;  ///< A view of the "default" depth-stencil view
	SuGPURenderableResourceViewPtr m_pBackLeftBufferView;    ///< A "default" renderable view of the back left buffer
	SuGPURenderableResourceViewPtr m_pBackRightBufferView;    ///< A "default" renderable view of the back left buffer

	SuGPUResourceStats m_resourceStats; ///< Tracks resource statistics


	bool m_bShutdownInProgress;   ///< Flag to signal that the GRM is shutting down

// tolua_begin
};
// tolua_end

#endif // _SUGPURESOUCEMANAGER_H
