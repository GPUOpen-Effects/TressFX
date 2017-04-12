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

#ifndef _SU_CUSTOMFONTMETRICS_H_
#define _SU_CUSTOMFONTMETRICS_H_

#include "SuVector2.h"
#include "SuCountedPtr.h"

class SuModelADB;
typedef SuCountedPtr<SuModelADB> SuModelADBPtr;

//=================================================================================================================================
/// \ingroup Fonts
/// \brief This class encapsulates font properties that are used for rendering
///
///  This class is meant to serve as a thin wrapper for a table of font properties (size, UVs, etc) that are used to render fonts.
///  There is not really any reason why anyone outside of the custom font code base would want to manipulate these objects.
/// 
//=================================================================================================================================
class SuCustomFontMetrics
{
public:

   /// Default constructor
   SuCustomFontMetrics();

   /// Returns the bounding rectangle of a particular character, in texture space
   bool GetCharacterUVs( SuChar nCharacter, SuVector2* pTopLeft, SuVector2* pBottomRight ) const;

   /// Returns the logical width of a particular character. 
   bool GetCharacterWidth( SuChar nCharacter, float32* pfWidth ) const;

   /// Sets the font metrics for a character.  This should only be used when constructing font metrics in the font preprocessor
   bool SetCharacterMetrics( SuChar nCharacter, const SuVector2& rTopLeft, const SuVector2& rBottomRight, float32 fWidth ) ;

   /// Determines whether or not the given character is supported by the font
   bool IsCharacterSupported( SuChar nCharacter ) const;

   /// Encodes the font metrics into an ADB
   SuModelADBPtr Serialize() const;

   /// Extracts font metrics from an ADB
   static SuCustomFontMetrics* Deserialize( SuModelADBPtr pADB ) ;

private:

   // I'm assuming only non-extended ASCII characters right now (char<=127).  I'm doing this mainly for speed, so that we can just have a lookup table.
   // When we decide to support unicode properly, we're going to need to re-consider this implementation
   static const uint32 MAX_FONTS = 128;

   struct Metric
   {
      SuVector2 vMinUV;       ///< Font UV (top-left)
      SuVector2 vMaxUV;       ///< Font UV (bottom-right)
      float32   fFontWidth;   ///< Width of the character in 'logical font units'.  1.0 is the height of one line of text 
   };

   /// A header struct that is written to the ADB
   struct ADBHeader
   {
      uint32 nOffset;   ///< Byte offset from the start of the header to the start of the font metric data
      uint16 nNumFonts; ///< Number of fonts for which metrics are provided.  ATM this is always equal to MAX_FONTS.  Eventually it may vary.
   };


   /// Helper method which looks up font metrics for a character
   const Metric* LookupMetric( SuChar nCharacter ) const;

   /// Helper method which looks up font metrics for a character
   Metric* LookupMetric( SuChar nCharacter ) ;


   Metric m_metrics[ MAX_FONTS ];
};


#endif // _SU_CUSTOMFONTMETRICS_H_
