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

#ifndef _SU_FONT_H_
#define _SU_FONT_H_

#include "SuTypes.h"
#include "SuVector4.h"
#include "SuString.h"
#include "SuSearchPath.h"

// tolua_begin

// \defgroup Fonts Fonts

//=========================================================================================================
/// \ingroup Fonts
/// \brief   An abstract base class to represent a font.  
/// 
/// The SuFont class provides an interface for creating and rendering fonts
//=========================================================================================================
class SuFont
{
public:

   enum CustomFontFlags
   {
      CF_FIXED_WIDTH = 1,  ///< Render all fonts with a uniform width.  By default, the font's associated metrics are used instead
   };

   enum RenderFlags
   {
      CENTER_VERTICALLY = 1 ///< Center text vertically (for 'RenderTextCentered' method)
   };

   /// Adds an entry to the search path for loading custom fonts
   static SU_DLL void AddSearchPath( const SuString& rPath );

   /// Factory method which constructs an SuFont object for a system font.
   static SU_DLL SuFont* CreateSystemFont( const SuString& rTypeFace, uint32 nSize, bool bBold, bool bItalic );
   
   /// Factory method which loads a custom font
   static SU_DLL SuFont* CreateCustomFont( const SuString& rFontFileName, uint32 nPointSize, 
                                           uint32 nFlags = 0,
                                           const SuString& rEffectFileName = "Global/gCustomFont.sufx" );

   /// Destructor
   virtual SU_DLL ~SuFont() ;

   /// Method which renders horizontally centered text
   virtual bool RenderTextCentered( float32 fY, const SuString& rText, const SuVector4& rColor = SuVector4(1,1,1,1), uint32 nFlags = 0 ) ;

   /// Low-level interface for text rendering.  Text will be rendered into the specified bounding rectangle.
   virtual bool RenderText( uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, const SuString& rText,
                            const SuVector4& rColor = SuVector4(1.0f, 1.0f, 1.0f, 1.0f) ) ;

// tolua_end
protected:
   
   /// Default constructor
   SuFont();

private:

   static SuSearchPath ms_SearchPath;  ///< Search path for loading custom fonts

// tolua_begin
};
// tolua_end

#endif // _SU_FONT_H_
