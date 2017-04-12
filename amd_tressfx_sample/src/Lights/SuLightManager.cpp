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

#include "PCH.h"
#include "SuAlgorithm.h"
#include "SuAnimationSet.h"
#include "SuAreaLight.h"
#include "SuDirectionalLight.h"
#include "SuHashMap.h"
#include "SuLight.h"
#include "SuLightController.h"
#include "SuLightManager.h"
#include "SuPointLight.h"
#include "SuSpotLight.h"
#include "SuVolumeLight.h"

/// Light manager singleton instance
SuLightManager* SuLightManager::ms_pInstance = NULL;

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Default constructor
//=================================================================================================================================
SuLightManager::SuLightManager() {}


//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuLightManager::~SuLightManager()
{
    for (LightNameMap::iterator itr = m_lights.begin(); itr != m_lights.end(); ++itr)
    {
        SU_SAFE_DELETE(itr->second);
    }
}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// \return A pointer to the light manager
//=================================================================================================================================
SuLightManager* SuLightManager::GetPtr()
{
    if (ms_pInstance)
    {
        return ms_pInstance;
    }

    ms_pInstance = new SuLightManager();
    return ms_pInstance;
}


//=================================================================================================================================
/// \return A reference to the light manager
//=================================================================================================================================
SuLightManager& SuLightManager::GetRef() { return *GetPtr(); }

//=================================================================================================================================
/// Destroys the light manager and all managed lights
//=================================================================================================================================
void SuLightManager::ShutDown() { SU_SAFE_DELETE(ms_pInstance); }

//=================================================================================================================================
/// \param rLightName The name to assign to the point light
///
/// \return A point light.  Returns NULL if another light with the given name already exists. The
/// light manager retains ownership
///         of the returned camera.
//=================================================================================================================================
SuPointLight* SuLightManager::CreatePointLight(const SuString& rLightName)
{
    if (SuLight* pExisting = GetLightByName(rLightName))
    {
        if (pExisting->GetType() != SuLight::POINT_LIGHT)
        {
            SuLogError("Light \"%s\" already exists and is not a point light.",
                       static_cast<const char8*>(rLightName));
            return NULL;
        }
        else
        {
            SuLogWarning("Point light \"%s\" already exists.  Returning existing point light.",
                         static_cast<const char8*>(rLightName));
            return (SuPointLight*)pExisting;
        }
    }

    SuPointLight* pLight = new SuPointLight(rLightName);
    m_lights.insert(rLightName, pLight);
    m_lightArray.push_back(pLight);
    // pLight->SetIndex(m_lightArray.size()-1);
    return pLight;
}

//=================================================================================================================================
/// \param rLightName The name to assign to the directional light
///
/// \return A directional light.  Returns NULL if another light with the given name already exists.
/// The light manager retains
///         ownership of the returned camera.
//=================================================================================================================================
SuDirectionalLight* SuLightManager::CreateDirectionalLight(const SuString& rLightName)
{
    if (SuLight* pExisting = GetLightByName(rLightName))
    {
        if (pExisting->GetType() != SuLight::DIRECTIONAL_LIGHT)
        {
            SuLogError("Light \"%s\" already exists and is not a directional light.",
                       static_cast<const char8*>(rLightName));
            return NULL;
        }
        else
        {
            SuLogWarning(
                "Directional light \"%s\" already exists.  Returning existing directional light.",
                static_cast<const char8*>(rLightName));
            return (SuDirectionalLight*)pExisting;
        }
    }

    SuDirectionalLight* pLight = new SuDirectionalLight(rLightName);
    m_lights.insert(rLightName, pLight);
    m_lightArray.push_back(pLight);
    // pLight->SetIndex(m_lightArray.size()-1);
    return pLight;
}

//=================================================================================================================================
/// \param rLightName The name to assign to the spot light
///
/// \return A spot light.  Returns NULL if another light with the given name already exists. The
/// light manager retains ownership
///         of the returned camera.
//=================================================================================================================================
SuSpotLight* SuLightManager::CreateSpotLight(const SuString& rLightName)
{
    if (SuLight* pExisting = GetLightByName(rLightName))
    {
        if (pExisting->GetType() != SuLight::SPOT_LIGHT)
        {
            SuLogError("Light \"%s\" already exists and is not a spotlight.",
                       static_cast<const char8*>(rLightName));
            return NULL;
        }
        else
        {
            SuLogWarning("Spotlight \"%s\" already exists.  Returning existing spotlight.",
                         static_cast<const char8*>(rLightName));
            return (SuSpotLight*)pExisting;
        }
    }

    SuSpotLight* pLight = new SuSpotLight(rLightName);
    m_lights.insert(rLightName, pLight);
    m_lightArray.push_back(pLight);
    // pLight->SetIndex(m_lightArray.size()-1);
    return pLight;
}

//=================================================================================================================================
/// \param rLightName The name to assign to the volume light
///
/// \return A volume light.  Returns NULL or the original camera if another light with the given
/// name already exists.
//=================================================================================================================================
SuVolumeLight* SuLightManager::CreateVolumeLight(const SuString& rLightName)
{
    if (SuLight* pExisting = GetLightByName(rLightName))
    {
        if (pExisting->GetType() != SuLight::VOLUME_LIGHT)
        {
            SuLogError("Light \"%s\" already exists and is not a volume light.",
                       static_cast<const char8*>(rLightName));
            return NULL;
        }
        else
        {
            SuLogWarning("Volume light \"%s\" already exists.  Returning existing volume light.",
                         static_cast<const char8*>(rLightName));
            return (SuVolumeLight*)pExisting;
        }
    }

    SuVolumeLight* pLight = new SuVolumeLight(rLightName);
    m_lights.insert(rLightName, pLight);
    m_lightArray.push_back(pLight);
    // pLight->SetIndex(m_lightArray.size()-1);
    return pLight;
}

//=================================================================================================================================
/// \param rLightName The name to assign to the area light
///
/// \return A area light.  Returns NULL if another light with the given name already exists. The
/// light manager retains ownership
///         of the returned camera.
//=================================================================================================================================
SuAreaLight* SuLightManager::CreateAreaLight(const SuString& rLightName)
{
    if (SuLight* pExisting = GetLightByName(rLightName))
    {
        if (pExisting->GetType() != SuLight::AREA_LIGHT)
        {
            SuLogError("Light \"%s\" already exists and is not an area light.",
                       static_cast<const char8*>(rLightName));
            return NULL;
        }
        else
        {
            SuLogWarning("Area light \"%s\" already exists.  Returning existing area light.",
                         static_cast<const char8*>(rLightName));
            return (SuAreaLight*)pExisting;
        }
    }

    SuAreaLight* pLight = new SuAreaLight(rLightName);
    m_lights.insert(rLightName, pLight);
    m_lightArray.push_back(pLight);
    // pLight->SetIndex(m_lightArray.size()-1);
    return pLight;
}

//=================================================================================================================================
/// \param rLightName The name of the desired light
///
/// \return A pointer to the light with the given name.  NULL if no such light exists.
//=================================================================================================================================
SuLight* SuLightManager::GetLightByName(const SuString& rLightName) const
{
    LightNameMap::iterator itr = m_lights.find(rLightName);
    if (itr != m_lights.end())
    {
        return itr->second;
    }
    return NULL;
}

//=================================================================================================================================
/// Loads a light set file
///
/// \param rFileName The light set file name
///
/// \return true on success, false otherwise
//=================================================================================================================================
bool SuLightManager::LoadLightSet(const SuString& rFileName)
{
    return LoadLightSet(rFileName, 0, NULL);
}

//=================================================================================================================================
/// Loads a light set file
///
/// \param rFileName The light set file name
/// \param nNumProperties Number of properties in properties array
/// \param pProperties Array of properties to assign to the lights in the set
///
/// \return true on success, false otherwise
//=================================================================================================================================
bool SuLightManager::LoadLightSet(const SuString& rFileName,
                                  uint32          nNumProperties,
                                  const SuString* pProperties)
{
    SuAnimationSet* pAnimSet = SuAnimationSet::CreateFromFile(rFileName);
    if (pAnimSet == NULL)
    {
        return false;
    }

    // extract light types and names from the animation set
    SuArray<SuString> lightNames;
    SuArray<uint8>    lightTypes;
    ExtractLightsFromLightSet(lightNames, lightTypes, pAnimSet);

    // create a LightSetInfo structure for this light set
    m_lightSets.push_back(SuLightSetInfo());
    SuLightSetInfo& rSetInfo = m_lightSets.back();
    rSetInfo.animationFile   = rFileName;
    rSetInfo.properties.reserve(nNumProperties);
    for (uint32 iProperty = 0; iProperty < nNumProperties; ++iProperty)
    {
        rSetInfo.properties.push_back(pProperties[iProperty]);
    }

    // create lights
    for (uint32 i = 0; i < lightNames.size(); i++)
    {
        SuLight* pLight = CreateLightFromLightSet(lightNames[i], lightTypes[i], pAnimSet);
        if (pLight)
        {
            rSetInfo.lights.insert(lightNames[i], pLight);
        }

        for (uint32 n = 0; n < nNumProperties; n++)
        {
            pLight->AddProperty(pProperties[n]);
        }
    }

    SU_SAFE_DELETE(pAnimSet);

    return true;
}

//=================================================================================================================================
/// Reloads all light sets that were loaded through a call to LoadLightSet()
///
/// This method cannot support changing light types.  If a light-set reload would cause light types
/// to change,
/// then re-loading of that particular light is skipped.
///
/// \return True if successful, false if any lightsets were skipped due to light type changes
//=================================================================================================================================
bool SuLightManager::ReloadLightSets()
{
    bool bErrors = false;

    // attempt to re-load each of the light sets that was loaded previously
    for (SuList<SuLightSetInfo>::iterator itr = m_lightSets.begin(); itr != m_lightSets.end();
         itr++)
    {
        SuLogInfo("Reloading light set from file: %s ", itr->animationFile.GetBuffer());

        // re-open animation set
        const SuAnimationSet* pSet = SuAnimationSet::CreateFromFile(itr->animationFile);
        if (!pSet)
        {
            SuLogError("Unable to create animation set from file: %s ",
                       itr->animationFile.GetBuffer());
            bErrors = true;
            continue;
        }

        // extract light information
        SuArray<SuString> lightNames;
        SuArray<uint8>    lightTypes;
        ExtractLightsFromLightSet(lightNames, lightTypes, pSet);

        // modify existing lights, and create new lights if specified
        for (uint32 i = 0; i < lightNames.size(); i++)
        {
            SuLight* pLight = GetLightByName(lightNames[i]);
            if (pLight)
            {
                // existing light.  Type specified in animation set must match actual type
                if (pLight->GetType() != lightTypes[i])
                {
                    SuLogError(
                        "Light type in lightset '%s' for light named: '%s' does not match type of existing light.  To change light types, you must restart the demo.",
                        itr->animationFile.GetBuffer(),
                        lightNames[i].GetBuffer());
                    continue;
                }

                // light types match for existing light, make a new controller
                SuLightControllerPtr pCtrl = CreateLightController(pLight, lightNames[i], *pSet);
                pLight->SetController(pCtrl);
            }
            else
            {
                // create new light for this light set
                SuLight* pLight = CreateLightFromLightSet(lightNames[i], lightTypes[i], pSet);
                if (pLight)
                {
                    size_t nProperties = itr->properties.size();
                    for (size_t iProperty = 0; iProperty < nProperties; ++iProperty)
                    {
                        pLight->AddProperty(itr->properties[iProperty]);
                    }
                    itr->lights.insert(lightNames[i], pLight);
                }
                else
                {
                    bErrors = true;
                }
            }
        }

        SU_SAFE_DELETE(pSet);
    }

    if (bErrors)
    {
        SuLogError("Reloaded lightsets with errors!!!");
    }
    else
    {
        SuLogInfo("Reloaded lightsets.");
    }

    return bErrors;
}


//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================


//=================================================================================================================================
/// \param rLightName  Name of light to create
/// \param nLightType  Type of light to create
/// \param pAnimSet    Animation set containing light parameter curves
/// \return A pointer to the created light, or NULL
//=================================================================================================================================
SuLight* SuLightManager::CreateLightFromLightSet(const SuString&       rLightName,
                                                 uint8                 nLightType,
                                                 const SuAnimationSet* pAnimSet)
{
    SuLight* pLight = NULL;
    switch (nLightType)
    {
    case SuLight::POINT_LIGHT:
    {
        pLight = CreatePointLight(rLightName);
        if (!pLight)
        {
            SuLogWarning("Unable to create point light!");
        }
    }
    break;

    case SuLight::DIRECTIONAL_LIGHT:
    {
        pLight = CreateDirectionalLight(rLightName);
        if (!pLight)
        {
            SuLogWarning("Unable to create directional light!");
        }
    }
    break;

    case SuLight::SPOT_LIGHT:
    {
        pLight = CreateSpotLight(rLightName);
        if (!pLight)
        {
            SuLogWarning("Unable to create spot light!");
        }
    }
    break;

    case SuLight::VOLUME_LIGHT:
    {
        pLight = CreateVolumeLight(rLightName);
        if (!pLight)
        {
            SuLogWarning("Unable to create volume light!");
        }
    }
    break;

    case SuLight::AREA_LIGHT:
    {
        pLight = CreateAreaLight(rLightName);
        if (!pLight)
        {
            SuLogWarning(" Unable to create area light!");
        }
    }
    break;

    default:
    {
        SuLogError("Unidentified light type!");
        return NULL;
    }
    }

    if (!pLight)
    {
        return NULL;
    }

    SuLightControllerPtr pCtrl = CreateLightController(pLight, rLightName, *pAnimSet);
    pLight->SetController(pCtrl);
    return pLight;
}


//=================================================================================================================================
/// Extracts an array of light names and types from an animation set
/// This is done by parsing the curve names and looking for the prefixes 'point', 'directional' or
/// 'spot'
/// \param rLightNames    An array that will be filled with light names
/// \param rLightTypes    An array that will be filled with light types
/// \param pAnimSet       The animation set to parse
//=================================================================================================================================
void SuLightManager::ExtractLightsFromLightSet(SuArray<SuString>&    rLightNames,
                                               SuArray<uint8>&       rLightTypes,
                                               const SuAnimationSet* pAnimSet)
{
    SuStringList animNames = pAnimSet->GetAllCurveNames();

    typedef SuHashMap<SuString, bool, SuStringHasher, SuHashComparer<SuString> > LightNameSet;
    LightNameSet pointLightNames;
    LightNameSet dirLightNames;
    LightNameSet spotLightNames;
    LightNameSet volumeLightNames;
    LightNameSet areaLightNames;

    // get all unique lights in the set
    for (SuStringList::iterator it = animNames.begin(); it != animNames.end(); ++it)
    {
        SuStringList tokens = it->Split(".");
        if (tokens.size() != 3)
        {
            SuLogWarning("Unrecognized animation in light set: " + *it);
            continue;
        }

        if (tokens.front() == "point")
        {
            tokens.pop_front();
            pointLightNames.insert(tokens.front(), true);
        }
        else if (tokens.front() == "directional")
        {
            tokens.pop_front();
            dirLightNames.insert(tokens.front(), true);
        }
        else if (tokens.front() == "spot")
        {
            tokens.pop_front();
            spotLightNames.insert(tokens.front(), true);
        }
        else if (tokens.front() == "volume")
        {
            tokens.pop_front();
            volumeLightNames.insert(tokens.front(), true);
        }
        else if (tokens.front() == "area")
        {
            tokens.pop_front();
            areaLightNames.insert(tokens.front(), true);
        }
        else
        {
            SuLogWarning("Unrecognized light type in light set: " + tokens.front());
            continue;
        }
    }

    // count lights
    uint32 nLights = pointLightNames.size() + dirLightNames.size() + spotLightNames.size() +
                     volumeLightNames.size() + areaLightNames.size();

    // initialize array of light types
    rLightTypes.reserve(nLights);
    rLightTypes.insert(rLightTypes.end(), pointLightNames.size(), SuLight::POINT_LIGHT);
    rLightTypes.insert(rLightTypes.end(), dirLightNames.size(), SuLight::DIRECTIONAL_LIGHT);
    rLightTypes.insert(rLightTypes.end(), spotLightNames.size(), SuLight::SPOT_LIGHT);
    rLightTypes.insert(rLightTypes.end(), volumeLightNames.size(), SuLight::VOLUME_LIGHT);
    rLightTypes.insert(rLightTypes.end(), areaLightNames.size(), SuLight::AREA_LIGHT);

    // initialize array of light names
    rLightNames.reserve(nLights);
    for (LightNameSet::iterator it = pointLightNames.begin(); it != pointLightNames.end(); ++it)
    {
        rLightNames.push_back(it->first);
    }
    for (LightNameSet::iterator it = dirLightNames.begin(); it != dirLightNames.end(); ++it)
    {
        rLightNames.push_back(it->first);
    }
    for (LightNameSet::iterator it = spotLightNames.begin(); it != spotLightNames.end(); ++it)
    {
        rLightNames.push_back(it->first);
    }
    for (LightNameSet::iterator it = volumeLightNames.begin(); it != volumeLightNames.end(); ++it)
    {
        rLightNames.push_back(it->first);
    }
    for (LightNameSet::iterator it = areaLightNames.begin(); it != areaLightNames.end(); ++it)
    {
        rLightNames.push_back(it->first);
    }
}


//=================================================================================================================================
/// \param pLight Light to create a controller for
/// \param rLightName Light name
/// \param rAnimSet Animation set that contains the animation curves for the light
///
/// \return Smart pointer to a light controller with the relevant animation curves from the
/// animation set
//=================================================================================================================================
SuLightControllerPtr SuLightManager::CreateLightController(SuLight*              pLight,
                                                           const SuString&       rLightName,
                                                           const SuAnimationSet& rAnimSet)
{
    SU_ASSERT(pLight != NULL);

    SuString curveNamePrefix;

    switch (pLight->GetType())
    {
    case SuLight::POINT_LIGHT:
        curveNamePrefix = "point.";
        break;

    case SuLight::DIRECTIONAL_LIGHT:
        curveNamePrefix = "directional.";
        break;

    case SuLight::SPOT_LIGHT:
        curveNamePrefix = "spot.";
        break;

    case SuLight::VOLUME_LIGHT:
        curveNamePrefix = "volume.";
        break;

    case SuLight::AREA_LIGHT:
        curveNamePrefix = "area.";
        break;

    default:
        SU_ASSERT(!"Unsupported light type");
        break;
    }

    curveNamePrefix += rLightName;

    // Common attributes

    SuString            curveName   = curveNamePrefix + ".vColor";
    SuAnimationCurvePtr pColorCurve = rAnimSet.GetCurve(curveName);

    curveName                           = curveNamePrefix + ".fIntensity";
    SuAnimationCurvePtr pIntensityCurve = rAnimSet.GetCurve(curveName);

    curveName                         = curveNamePrefix + ".bEmitDiffuse";
    SuAnimationCurvePtr pDiffuseCurve = rAnimSet.GetCurve(curveName);

    curveName                          = curveNamePrefix + ".bEmitSpecular";
    SuAnimationCurvePtr pSpecularCurve = rAnimSet.GetCurve(curveName);

    curveName                         = curveNamePrefix + ".nDecayRate";
    SuAnimationCurvePtr pFalloffCurve = rAnimSet.GetCurve(curveName);

    curveName                            = curveNamePrefix + ".visibility";
    SuAnimationCurvePtr pVisibilityCurve = rAnimSet.GetCurve(curveName);

    SuAnimationCurvePtr pRadiusCurve;
    SuAnimationCurvePtr pPosCurve;
    SuAnimationCurvePtr pOrientationCurve;
    SuAnimationCurvePtr pScaleCurve;
    SuAnimationCurvePtr pConeAngleCurve;
    SuAnimationCurvePtr pPenumbraAngleCurve;
    SuAnimationCurvePtr pDropOffCurve;
    SuAnimationCurvePtr pLightShapeCurve;
    SuAnimationCurvePtr pSushiLightIndirectCurve;

    if (pLight->GetType() == SuLight::POINT_LIGHT)
    {
        // point light
        curveName    = curveNamePrefix + ".fRadius";
        pRadiusCurve = rAnimSet.GetCurve(curveName);
    }

    if (pLight->GetType() == SuLight::POINT_LIGHT || pLight->GetType() == SuLight::SPOT_LIGHT ||
        pLight->GetType() == SuLight::VOLUME_LIGHT || pLight->GetType() == SuLight::AREA_LIGHT)
    {
        // point and spot light
        curveName = curveNamePrefix + ".vPosition";
        pPosCurve = rAnimSet.GetCurve(curveName);
    }

    if (pLight->GetType() == SuLight::DIRECTIONAL_LIGHT ||
        pLight->GetType() == SuLight::SPOT_LIGHT || pLight->GetType() == SuLight::VOLUME_LIGHT ||
        pLight->GetType() == SuLight::AREA_LIGHT)
    {
        // directional, splot light and vollume light
        curveName         = curveNamePrefix + ".qOrientation";
        pOrientationCurve = rAnimSet.GetCurve(curveName);
    }

    if (pLight->GetType() == SuLight::SPOT_LIGHT)
    {
        // spotlight
        curveName       = curveNamePrefix + ".fConeAngle";
        pConeAngleCurve = rAnimSet.GetCurve(curveName);

        curveName           = curveNamePrefix + ".fPenumbraAngle";
        pPenumbraAngleCurve = rAnimSet.GetCurve(curveName);

        curveName     = curveNamePrefix + ".fDropOff";
        pDropOffCurve = rAnimSet.GetCurve(curveName);
    }

    if (pLight->GetType() == SuLight::VOLUME_LIGHT || pLight->GetType() == SuLight::AREA_LIGHT)
    {
        curveName   = curveNamePrefix + ".vScale";
        pScaleCurve = rAnimSet.GetCurve(curveName);
    }

    if (pLight->GetType() == SuLight::VOLUME_LIGHT)
    {
        curveName        = curveNamePrefix + ".nLightShape";
        pLightShapeCurve = rAnimSet.GetCurve(curveName);

        curveName                = curveNamePrefix + ".bSushiLightIndirect";
        pSushiLightIndirectCurve = rAnimSet.GetCurve(curveName);
    }

    // create controller
    SuLightControllerPtr pController =
        SuLightControllerPtr(new SuLightController(pColorCurve,
                                                   pIntensityCurve,
                                                   pDiffuseCurve,
                                                   pSpecularCurve,
                                                   pFalloffCurve,
                                                   pRadiusCurve,
                                                   pPosCurve,
                                                   pOrientationCurve,
                                                   pScaleCurve,
                                                   pConeAngleCurve,
                                                   pPenumbraAngleCurve,
                                                   pDropOffCurve,
                                                   pVisibilityCurve,
                                                   pLightShapeCurve,
                                                   pSushiLightIndirectCurve));
    SU_ASSERT(pController != NULL);

    return pController;
}
