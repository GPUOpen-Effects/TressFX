//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef _SU_LIGHTMANAGER_H_
#define _SU_LIGHTMANAGER_H_

#include "SuArray.h"
#include "SuHashMap.h"
#include "SuLightController.h"
#include "SuTypes.h"

class SuLight;
class SuPointLight;
class SuSpotLight;
class SuDirectionalLight;
class SuVolumeLight;
class SuAreaLight;
class SuAnimationSet;

typedef SuCountedPtr<SuLightController> SuLightControllerPtr;


//=================================================================================================================================
/// \ingroup Engine
/// \brief  A class which creates and manages light objects
///
//=================================================================================================================================

// tolua_begin
class SuLightManager
{
public:
    typedef SuHashMap<SuString, SuLight*, SuStringHasher, SuHashComparer<SuString> > LightNameMap;

    /// Destructor
    virtual ~SuLightManager();

    /// Singleton access
    static SuLightManager* GetPtr();

    /// Singleton access
    static SuLightManager& GetRef();

    /// Destroys the light manager and all managed lights
    static void ShutDown();

    /// Creates a point light source
    SuPointLight* CreatePointLight(const SuString& rLightName);

    /// Creates a directional light source
    SuDirectionalLight* CreateDirectionalLight(const SuString& rLightName);

    /// Creates a spot light source
    SuSpotLight* CreateSpotLight(const SuString& rLightName);

    /// Creates a volume light source
    SuVolumeLight* CreateVolumeLight(const SuString& rLightName);

    /// Creates a area light source
    SuAreaLight* CreateAreaLight(const SuString& rLightName);

    /// Returns a pointer to the light with the given name
    SuLight* GetLightByName(const SuString& rLightName) const;

    /// Returns a pointer to a light with the given index
    SuLight* GetLightByIndex(uint32 nIndex) const { return m_lightArray[nIndex]; }

    /// Returns the number of lights the manager contains
    uint32 GetNumLights() const { return m_lights.size(); }

    /// Loads a light set file
    bool LoadLightSet(const SuString& rFileName);

/// Loads a light set and adds properties to the lights
#if 0
   // tolua only understands this syntax, which is not legal C++
   bool LoadLightSet( const SuString& rFileName, uint32 nProperties, const SuString pProperties[nProperties] );
#else
    // tolua_end
    bool LoadLightSet(const SuString& rFileName,
                      uint32          nNumProperties,
                      const SuString* pProperties);
// tolua_begin
#endif

    /// Reloads all light sets that were previously loaded through a call to LoadLightSet
    bool ReloadLightSets();

    // tolua_end

    /// Returns a map of all lights
    const LightNameMap& GetAllLights() const { return m_lights; }

private:
    /// Struct containing information about loaded light sets
    struct SuLightSetInfo
    {
        LightNameMap      lights;         ///< Map of light names to lights in the set
        SuString          animationFile;  ///< Animation file which contains light curves
        SuArray<SuString> properties;
    };

    /// Extracts light names and types based on the curves in an animation set
    void ExtractLightsFromLightSet(SuArray<SuString>&    rLightNames,
                                   SuArray<uint8>&       rLightTypes,
                                   const SuAnimationSet* pAnimSet);

    /// Creates a light with a particular name and type, and builds a controller for it by
    /// extracting animation curves from a light set
    SuLight* CreateLightFromLightSet(const SuString&       rLightName,
                                     uint8                 nLightType,
                                     const SuAnimationSet* pAnimSet);

    /// Creates a light controller by extracting the relevant animations from an animation set
    SuLightControllerPtr CreateLightController(SuLight*              pLight,
                                               const SuString&       rLightName,
                                               const SuAnimationSet& rAnimSet);

    /// Default constructor.  Private to restrict instantiation to singleton access method
    SuLightManager();

    static SuLightManager* ms_pInstance;

    /// Map of light names to lights
    LightNameMap m_lights;

    /// Array that stores the same lights as m_lights, but allows for easily retrieving lights
    /// using an index.
    SuArray<SuLight*> m_lightArray;

    SuList<SuLightSetInfo>
        m_lightSets;  ///< Information about loaded light sets, to support reloading

    // tolua_begin
};
// tolua_end

#endif  // _SU_LIGHTMANAGER_H_
