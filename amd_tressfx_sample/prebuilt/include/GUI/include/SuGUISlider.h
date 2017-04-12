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

#ifndef _SUGUISLIDER_H_
#define _SUGUISLIDER_H_

#include "SuGUIElement.h"

// tolua_begin
class SuGUISlider : public SuGUIElement
{

public:

    /// Deconstructor
    ~SuGUISlider();
// tolua_end
    /// ***************************************************************
    /// Static member used to create a SuGUISlider
    ///
    /// \param name Name Of this Element, Can not be NULL.
	/// \param width The width of the slider, 0 will automatically resize based off window width.
    /// \param startingValue float used to set an intial value for the slider. Cannot be bigger then max or lower then min. default = 0.5.
    /// \param min Minimum value of the slider, default = 0.0.
    /// \param max Maximum value of the slider, defualt = 1.0.
    /// \param isSameLine Whether this element is on the same line as the previous element.
    /// \return SuGUISlider*
    /// ***************************************************************
	static SuGUISlider* CreateSliderFloat(SuString name, float width, float startingValue, float min, float max, bool isSameLineAsLastElement = false);

	/// ***************************************************************
	/// Static member used to create a SuGUISlider with integer 
	///
	/// \param name Name Of this Element, Can not be NULL.
	/// \param width The width of the slider, 0 will automatically resize based off window width.
	/// \param startingValue float used to set an intial value for the slider. Cannot be bigger then max or lower then min. default = 0.5.
	/// \param min Minimum value of the slider, default = 0.0.
	/// \param max Maximum value of the slider, defualt = 1.0.
	/// \param isSameLine Whether this element is on the same line as the previous element.
	/// \return SuGUISlider*
	/// ***************************************************************
	static SuGUISlider* CreateSliderInt(SuString name, float width, int startingValue, int min, int max, bool isSameLineAsLastElement = false);

// tolua_begin
    /// Returns the value of the slider
    SU_DLL float GetValue();
// tolua_end

    /// Overrides the base Draw Function.
    virtual void Draw();

private:
    /// Constructor
    SuGUISlider();

	enum SliderValueType { FLOAT, INT };

	SliderValueType m_ValueType;

	/// The Width of the slider, 0 will auto resize based on window size.
	float m_nWidth;

    /// The Value of the slider
    float m_nValue;

    /// The max value the slider can have.
    float m_nMin;

    /// The min value a slider can have.
    float m_nMax;
// tolua_begin
};
// tolua_end
#endif