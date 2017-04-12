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

#ifndef _SU_LIGHTUPDATEHANDLERFACTORY_H_
#define _SU_LIGHTUPDATEHANDLERFACTORY_H_

#include "SuUpdateHandlerFactory.h"

class SuObjectLightTable;

//=================================================================================================================================
/// \ingroup Engine
/// \brief A factory to create light-related AppUpdate handlers
///
//=================================================================================================================================
class SuLightUpdateHandlerFactory : public SuUpdateHandlerFactory
{
public:
    /// Constructor
    SuLightUpdateHandlerFactory(const SuObjectLightTable& rObjectLightInteractions)
        : m_rObjectLightInteractions(rObjectLightInteractions)
    {
    }

    /// Destructor
    virtual ~SuLightUpdateHandlerFactory() {}

    /// Creates an update handler for an effect parameter
    virtual SuUpdateHandlerPtr GetUpdateHandler(const SuString&             rAppUpdateName,
                                                SuEffectParameter::DataType eType,
                                                uint32                      nArraySize,
                                                const SuAnnotationMap*      pAnnotations);

    /// Registers this factory with the effect manager
    void RegisterFactory();

private:
    /// Create a number of lights update handler
    SuUpdateHandlerPtr MakeNumLightsHandler(SuEffectParameter::DataType eType,
                                            uint32                      nArraySize,
                                            const SuAnnotationMap*      pAnnotations) const;

    /// Create a light position update handler
    SuUpdateHandlerPtr MakeLightPositionHandler(SuEffectParameter::DataType eType,
                                                uint32                      nArraySize,
                                                const SuAnnotationMap*      pAnnotations) const;

    /// Create a light cutoff update handler
    SuUpdateHandlerPtr MakeLightCutoffHandler(SuEffectParameter::DataType eType,
                                              uint32                      nArraySize,
                                              const SuAnnotationMap*      pAnnotations) const;

    /// Create a light color update handler
    SuUpdateHandlerPtr MakeLightColorHandler(SuEffectParameter::DataType eType,
                                             uint32                      nArraySize,
                                             const SuAnnotationMap*      pAnnotations) const;

    /// Create a misc. light parameter update handler
    SuUpdateHandlerPtr MakeLightParamsHandler(SuEffectParameter::DataType eType,
                                              uint32                      nArraySize,
                                              const SuAnnotationMap*      pAnnotations) const;

    /// Create a light direction update handler
    SuUpdateHandlerPtr MakeLightDirectionHandler(SuEffectParameter::DataType eType,
                                                 uint32                      nArraySize,
                                                 const SuAnnotationMap*      pAnnotations) const;

    /// Create a light cone angles update handler
    SuUpdateHandlerPtr MakeLightConeAnglesHandler(SuEffectParameter::DataType eType,
                                                  uint32                      nArraySize,
                                                  const SuAnnotationMap*      pAnnotations) const;

    /// Create a light view-projection matrix update handler
    SuUpdateHandlerPtr MakeLightViewProjectionMatrixHandler(
        SuEffectParameter::DataType eType,
        uint32                      nArraySize,
        const SuAnnotationMap*      pAnnotations) const;

    /// Create a light view-projection-texture matrix update handler
    SuUpdateHandlerPtr MakeLightViewProjTexMatrixHandler(
        SuEffectParameter::DataType eType,
        uint32                      nArraySize,
        const SuAnnotationMap*      pAnnotations) const;

    /// Create a light view matrix update handler
    SuUpdateHandlerPtr MakeLightViewMatrixHandler(SuEffectParameter::DataType eType,
                                                  uint32                      nArraySize,
                                                  const SuAnnotationMap*      pAnnotations) const;

    /// Create a light projection matrix update handler
    SuUpdateHandlerPtr MakeLightProjectionMatrixHandler(SuEffectParameter::DataType eType,
                                                        uint32                      nArraySize,
                                                        const SuAnnotationMap* pAnnotations) const;

    /// Create a light scale update handler
    SuUpdateHandlerPtr MakeLightScaleHandler(SuEffectParameter::DataType eType,
                                             uint32                      nArraySize,
                                             const SuAnnotationMap*      pAnnotations) const;

    /// Create a light orientation update handler
    SuUpdateHandlerPtr MakeLightOrientationHandler(SuEffectParameter::DataType eType,
                                                   uint32                      nArraySize,
                                                   const SuAnnotationMap*      pAnnotations) const;

    /// Create a light shape update handler
    SuUpdateHandlerPtr MakeLightShapeHandler(SuEffectParameter::DataType eType,
                                             uint32                      nArraySize,
                                             const SuAnnotationMap*      pAnnotations) const;

    /// Create a light intensity update handler
    SuUpdateHandlerPtr MakeLightIntensityHandler(SuEffectParameter::DataType eType,
                                                 uint32                      nArraySize,
                                                 const SuAnnotationMap*      pAnnotations) const;


    /// Create a light index update handler
    SuUpdateHandlerPtr MakeLightIndexHandler(SuEffectParameter::DataType eType,
                                             uint32                      nArraySize,
                                             const SuAnnotationMap*      pAnnotations) const;

    const SuObjectLightTable& m_rObjectLightInteractions;
};


#endif  // _SU_LIGHTUPDATEHANDLERFACTORY_H_
