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
#include "SuEffectManager.h"
#include "SuLightUpdateHandler.h"
#include "SuLightUpdateHandlerFactory.h"

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================


//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Creates an update handler for an effect parameter
///
/// \param rAppUpdateName The name of the appupdate requested for the effect parameter
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::GetUpdateHandler(
    const SuString&             rAppUpdateName,
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations)
{
    SU_ASSERT(pAnnotations != NULL);

    if (rAppUpdateName == "NumLights")
    {
        return MakeNumLightsHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightPosition")
    {
        return MakeLightPositionHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightCutoff")
    {
        return MakeLightCutoffHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightColor")
    {
        return MakeLightColorHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightParams")
    {
        return MakeLightParamsHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightDirection")
    {
        return MakeLightDirectionHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightConeAngles")
    {
        return MakeLightConeAnglesHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightViewProjMatrix" ||
             rAppUpdateName == "LightViewProjectionMatrix")
    {
        return MakeLightViewProjectionMatrixHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightViewProjTexMatrix" ||
             rAppUpdateName == "LightViewProjTextureMatrix")
    {
        return MakeLightViewProjTexMatrixHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightViewMatrix")
    {
        return MakeLightViewMatrixHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightProjMatrix" || rAppUpdateName == "LightProjectionMatrix")
    {
        return MakeLightProjectionMatrixHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightScale")
    {
        return MakeLightScaleHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightOrientation")
    {
        return MakeLightOrientationHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightShape")
    {
        return MakeLightShapeHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightIntensity")
    {
        return MakeLightIntensityHandler(eType, nArraySize, pAnnotations);
    }
    else if (rAppUpdateName == "LightIndex")
    {
        return MakeLightIndexHandler(eType, nArraySize, pAnnotations);
    }
    else
    {
        SuLogError("Unsupported light Appupdate type");
        return SuUpdateHandlerPtr();
    }

    // NULL pointer
    return SuUpdateHandlerPtr();
}

//=================================================================================================================================
/// Registers this factory with the effect manager
//=================================================================================================================================
void SuLightUpdateHandlerFactory::RegisterFactory()
{
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("NumLights", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightPosition", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightCutoff", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightColor", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightParams", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightDirection", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightConeAngles", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightViewProjMatrix", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightViewProjectionMatrix", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightViewProjTexMatrix", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightViewProjTextureMatrix", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightViewMatrix", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightProjMatrix", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightProjectionMatrix", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightScale", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightOrientation", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightShape", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightIntensity", this);
    SuEffectManager::GetRef().RegisterUpdateHandlerFactory("LightIndex", this);
}

//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Creates a number of lights update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeNumLightsHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::INT)
    {
        SuLogError("Appupdate \"NumLights\" requires an integer effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    return SuUpdateHandlerPtr(new SuLightUpdateHandler(
        SuLightUpdateHandler::SU_NUM_LIGHTS, nArraySize, m_rObjectLightInteractions));
}

//=================================================================================================================================
/// Creates a light position update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightPositionHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::VECTOR)
    {
        SuLogError("Appupdate \"LightPosition\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_POSITION_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_POSITION,
                                                           nArraySize,
                                                           m_rObjectLightInteractions,
                                                           lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Creates a light cutoff update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightCutoffHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::VECTOR)
    {
        SuLogError("Appupdate \"LightCutoff\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of cutoffs
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(
            SuLightUpdateHandler::SU_LIGHT_CUTOFF_ARRAY, nArraySize, m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_CUTOFF,
                                                           nArraySize,
                                                           m_rObjectLightInteractions,
                                                           lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Creates a light color update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightColorHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::VECTOR)
    {
        SuLogError("Appupdate \"LightColor\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(
            SuLightUpdateHandler::SU_LIGHT_COLOR_ARRAY, nArraySize, m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_COLOR,
                                                           nArraySize,
                                                           m_rObjectLightInteractions,
                                                           lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}


//=================================================================================================================================
/// Create a misc. light parameter update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightParamsHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::VECTOR)
    {
        SuLogError("Appupdate \"LightParams\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(
            SuLightUpdateHandler::SU_LIGHT_PARAMS_ARRAY, nArraySize, m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_PARAMS,
                                                           nArraySize,
                                                           m_rObjectLightInteractions,
                                                           lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light direction update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightDirectionHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::VECTOR)
    {
        SuLogError("Appupdate \"LightDirection\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_DIRECTION_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_DIRECTION,
                                     nArraySize,
                                     m_rObjectLightInteractions,
                                     lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light cone angles update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightConeAnglesHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::VECTOR)
    {
        SuLogError("Appupdate \"LightConeAngles\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_CONE_ANGLES_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_CONE_ANGLES,
                                     nArraySize,
                                     m_rObjectLightInteractions,
                                     lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light view-projection matrix update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightViewProjectionMatrixHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::MATRIX)
    {
        SuLogError(
            "Appupdate \"LightViewProjectionMatrix\" requires a float matrix effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_VIEW_PROJ_MATRIX_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_VIEW_PROJ_MATRIX,
                                     nArraySize,
                                     m_rObjectLightInteractions,
                                     lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light view-projection-texture matrix update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightViewProjTexMatrixHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::MATRIX)
    {
        SuLogError(
            "Appupdate \"LightViewProjTextureMatrix\" requires a float matrix effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_VIEW_PROJ_TEX_MATRIX_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_VIEW_PROJ_TEX_MATRIX,
                                     nArraySize,
                                     m_rObjectLightInteractions,
                                     lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light view matrix update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightViewMatrixHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::MATRIX)
    {
        SuLogError("Appupdate \"LightViewMatrix\" requires a float matrix effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_VIEW_MATRIX_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_VIEW_MATRIX,
                                     nArraySize,
                                     m_rObjectLightInteractions,
                                     lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light projection matrix update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightProjectionMatrixHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::MATRIX)
    {
        SuLogError("Appupdate \"LightProjectionMatrix\" requires a float matrix effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_PROJ_MATRIX_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_PROJ_MATRIX,
                                     nArraySize,
                                     m_rObjectLightInteractions,
                                     lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light scale update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightScaleHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::VECTOR)
    {
        SuLogError("Appupdate \"LightScale\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(
            SuLightUpdateHandler::SU_LIGHT_SCALE_ARRAY, nArraySize, m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_SCALE,
                                                           nArraySize,
                                                           m_rObjectLightInteractions,
                                                           lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light orientation update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightOrientationHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::VECTOR)
    {
        SuLogError("Appupdate \"LightDirection\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_ORIENTATION_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_ORIENTATION,
                                     nArraySize,
                                     m_rObjectLightInteractions,
                                     lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light shape update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightShapeHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::INT)
    {
        SuLogError("Appupdate \"LightShape\" requires a int vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(
            SuLightUpdateHandler::SU_LIGHT_SHAPE_ARRAY, nArraySize, m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_SHAPE,
                                                           nArraySize,
                                                           m_rObjectLightInteractions,
                                                           lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light index update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightIndexHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::INT)
    {
        SuLogError("Appupdate \"LightShape\" requires a int vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(
            SuLightUpdateHandler::SU_LIGHT_INDEX_ARRAY, nArraySize, m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_INDEX,
                                                           nArraySize,
                                                           m_rObjectLightInteractions,
                                                           lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}

//=================================================================================================================================
/// Create a light intensity update handler
///
/// \param eType The effect parameter's data type
/// \param nArraySize The effect parameter's array size
/// \param pAnnotations The annotation for the effect parameter
///
/// \return Smart pointer to an update handler or NULL on failure
//=================================================================================================================================
SuUpdateHandlerPtr SuLightUpdateHandlerFactory::MakeLightIntensityHandler(
    SuEffectParameter::DataType eType,
    uint32                      nArraySize,
    const SuAnnotationMap*      pAnnotations) const
{
    if (eType != SuEffectParameter::FLOAT)
    {
        SuLogError("Appupdate \"LightIntensity\" requires a float vector effect parameter");
        return SuUpdateHandlerPtr(NULL);
    }

    SuString lightName = pAnnotations->LookupString("Name", "__NONE__");
    if (lightName == "__NONE__")
    {
        // make an appupdate handler for an array of positions
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_INTENSITY_ARRAY,
                                     nArraySize,
                                     m_rObjectLightInteractions));
    }
    else
    {
        // make an appupdate handler for a single named light source
        return SuUpdateHandlerPtr(
            new SuLightUpdateHandler(SuLightUpdateHandler::SU_LIGHT_INTENSITY,
                                     nArraySize,
                                     m_rObjectLightInteractions,
                                     lightName));
    }

    return SuUpdateHandlerPtr(NULL);
}