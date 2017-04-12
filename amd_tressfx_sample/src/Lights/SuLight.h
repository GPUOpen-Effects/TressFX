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

#ifndef _SU_LIGHT_H_
#define _SU_LIGHT_H_

#include "SuLightController.h"
#include "SuTypes.h"
#include "SuVector4.h"

class SuBoundingBox;

// tolua_begin
typedef SuCountedPtr<SuLightController> SuLightControllerPtr;
// tolua_end


//=================================================================================================================================
/// \ingroup Engine
/// \brief Base class for all lights
///
//=================================================================================================================================
// tolua_begin
class SuLight
{
public:
    /// Enum for all possible light types
    ///
    /// If you subclass SuLight, you need to add a new enum here.
    enum LightType
    {
        POINT_LIGHT       = 0,
        DIRECTIONAL_LIGHT = 1,
        SPOT_LIGHT        = 2,
        VOLUME_LIGHT      = 3,
        AREA_LIGHT        = 4
    };

    enum FalloffType
    {
        FALLOFF_NONE = 0,       // Note that these enum values are equal to the exponent of (1/r)
                                // required to compute the falloff.
        FALLOFF_LINEAR    = 1,  // This is by design and must not be changed.
        FALLOFF_QUADRATIC = 2
    };

    enum LightShapeType
    {
        VOLUMELIGHT_BOX      = 0,
        VOLUMELIGHT_SPHERE   = 1,
        VOLUMELIGHT_CYLINDER = 2,
        VOLUMELIGHT_CONE     = 3
    };

    /// Destructor
    virtual ~SuLight() {}

    /// Returns the light's type
    LightType GetType() const { return m_type; }

    /// Returns the light's current visibility state
    bool IsVisible() const { return m_bVisible; }

    /// Sets the light's visibility
    void SetVisible(bool bVisible) { m_bVisible = bVisible; }

    /// Sets the light intensity
    void SetIntensity(float32 fIntensity) { m_fIntensity = fIntensity; }

    /// Returns the light intensity
    float32 GetIntensity() const { return m_fIntensity; }

    /// Sets the light color
    void SetColor(const SuVector4& rColor) { m_color = rColor; }

    /// Returns the light color
    const SuVector4& GetColor() const { return m_color; }

    /// Turns diffuse lighting on or off
    void SetDiffuse(bool bEnable) { m_bDiffuse = bEnable; }

    /// Returns whether this light emits diffuse light
    bool IsDiffuse() const { return m_bDiffuse; }

    /// Turns specular lighting on or off
    void SetSpecular(bool bEnable) { m_bSpecular = bEnable; }

    /// Returns whether this light emits specular light
    bool IsSpecular() const { return m_bSpecular; }

    /// Sets the falloff type
    void SetFalloff(FalloffType type) { m_falloff = type; }

    /// Returns the falloff type
    FalloffType GetFalloff() const { return m_falloff; }

    /// Computes the distance-based light attenuation
    float32 ComputeDistanceAttenuation(float32 fDist) const;

    /// Tests whether the light casts any light on a bounding box
    virtual bool IlluminatesBoundingBox(const SuBoundingBox& rBB) const = 0;

    // tolua_end
    /// Sets a controller for the light
    void SetController(SuLightControllerPtr pController) { m_pController = pController; }
    // tolua_begin

    /// Returns the light's current controller
    SuLightController* GetController() const { return m_pController.get(); }

    /// Update the light using the current light controller
    void Update(SuTime fTime);

    /// Returns the name of the light
    const SuString& GetName() const { return m_name; };

    /// Assigns a property
    void AddProperty(const SuString& rStrProperty);

    /// Test if we have particular property
    bool HasProperty(const SuString& rStrProperty) const;

    uint32 GetNumProperties() const { return static_cast<uint32>(m_properties.size()); }

    const SuString& GetProperty(uint32 i) { return m_properties[i]; }


    /// Returns the entire list of properties
    inline const SuStringArray& GetProperties() const { return m_properties; }

    /// Sets an index (handle) for the light
    void SetIndex(int32 nIndex) { m_nIndex = nIndex; }

    /// Returns the index (handle) for the light
    int32 GetIndex() const { return m_nIndex; }

    // tolua_end

protected:
    /// Constructor
    SuLight(LightType type, const SuString& rName)
        : m_type(type)
        , m_name(rName)
        , m_bVisible(true)
        , m_bDiffuse(true)
        , m_bSpecular(true)
        , m_fIntensity(1.0f)
        , m_color(1.0f, 1.0f, 1.0f, 1.0f)
        , m_falloff(FALLOFF_NONE)
        , m_nIndex(-1)
    {
    }

    /// Returns the light's brightness (grayscale color * intensity)
    float32 GetBrightness() const;

private:
    LightType     m_type;
    SuString      m_name;
    SuStringArray m_properties;

    bool        m_bVisible;
    bool        m_bDiffuse;
    bool        m_bSpecular;
    float32     m_fIntensity;
    SuVector4   m_color;
    FalloffType m_falloff;
    int32       m_nIndex;

    static const SuVector4 COLOR_TO_GRAY_WEIGHTS;

    SuLightControllerPtr m_pController;

    // tolua_begin
};
// tolua_end


#endif  // _SU_LIGHT_H_
