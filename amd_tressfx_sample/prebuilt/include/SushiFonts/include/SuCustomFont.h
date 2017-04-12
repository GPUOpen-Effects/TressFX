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

#ifndef _SU_CUSTOMFONT_H_
#define _SU_CUSTOMFONT_H_

#include "SuFont.h"
#include "SuCountedPtr.h"
#include "SuVector2.h"
#include "SuEffectReloadListener.h"

class SuEffect;
class SuGPUSamplingResourceView;
class SuCustomFontMetrics;
class SuGPUVertexBuffer;
class SuGPUIndexBuffer;
class SuGPUTexture;

typedef SuCountedPtr<SuEffect> SuEffectPtr;
typedef SuCountedPtr<SuGPUSamplingResourceView> SuGPUSamplingResourceViewPtr;
typedef SuCountedPtr<SuCustomFontMetrics> SuCustomFontMetricsPtr;
typedef SuCountedPtr<SuGPUVertexBuffer> SuGPUVertexBufferPtr;
typedef SuCountedPtr<SuGPUIndexBuffer> SuGPUIndexBufferPtr;
typedef SuCountedPtr<SuGPUTexture> SuGPUTexturePtr;

//=================================================================================================================================
/// \ingroup Fonts
/// \brief A font class which implements a custom font renderer for Sushi3.
///  
///  SuCustomFont is a class which implements a custom font renderer.  This renderer is more efficient than the D3DX font renderer, and
///  provides customizable fonts.  
///  
///  In order to use custom fonts, it is necessary to generate a preprocessed font file (suf2) by using the SuMakeFont tool.
///
///  Preprocessed fonts can be then loaded into the engine by using the factory method that is exposed by the SuFont base class.
///
///  Font sizes are specified in points, and conversion to screen-space pixel sizes is done on the fly, based on the current viewport width
///
///  There are a few implementation details to be aware of when using custom fonts:
///    1.  Tabs are always replaced emulated with four spaces
///    2.  Font positions and sizes are always calculated by using the current viewport size as the window size
///    3.  Font colors may not behave like you expect.  Coloring is implemented by fetching a texture and modulating by the given font color
///        If the font texture is red, and you try to draw it blue, you'll get black
///
///  By default, whitespace characters are slightly smaller than other text.  This makes for prettier text, but is problematic
///     when printing formatted tables.  To override this behavior, the FIXED_WIDTH flag can be used
///
//=================================================================================================================================
class SuCustomFont : public SuFont, virtual public SuEffectReloadListener
{
public:

   /// Default constructor for a custom font
   SuCustomFont( uint32 nPointSize, SuEffectPtr pEffect, SuGPUTexturePtr pTexture, SuCustomFontMetricsPtr pFontMetrics, uint32 nFlags );

   /// Destructor
   ~SuCustomFont( );

   /// Renders horizontally centered text
   virtual bool RenderTextCentered( float32 fY, const SuString& rText, const SuVector4& rColor = SuVector4(1,1,1,1), uint32 nFlags = 0 );

   /// Low-level interface for text rendering.  Text will be rendered into the specified bounding rectangle.
   virtual bool RenderText( uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, const SuString& rText,
                            const SuVector4& rColor = SuVector4(1.0f, 1.0f, 1.0f, 1.0f)) ;

   /// Called during effect reload
   virtual void OnEffectReload( SuEffectPtr pOldEffect, SuEffectPtr pNewEffect ) ;

private:


   static const uint32 PIXELS_PER_INCH;      ///< Number of pixels per inch to assume when converting font sizes from points to pixels
   static const float  POINTS_TO_INCHES;     ///< Scale factor for converting from points to inches

   /// Struct to represent the vertices used in font rendering
   struct SuCustomFontVertex
   {
      SuVector2 vXY; ///< X,Y position of the vertex, in logical font units.  The shader will transform this appropriately
      SuVector2 vUV; ///< UVs of the font vertex, in font texture space
   };

   /// Fills a font vertex buffer with font vertices for the specified text, in logical space
   uint32 GenerateFontBillboards( const SuString& rText, SuCustomFontVertex* pVertexBuffer );

   /// Center-aligns a set of font billboards
   void CenterFontBillboards( SuCustomFontVertex* pVertexBuffer, uint32 nCharacters, float32* pfWidth, float32* pfHeight );

   /// Helper method that converts logical font space to pixel space
   float32 ConvertLogicalToPixels( float32 fValue );

   /// Issues draw calls to render the specified number of characters
   bool RenderCharacters( uint32 nX, uint32 nY, uint32 nChars );

   /// Resizes the font vertex and index buffers so that they will hold the specified number of characters
   bool ResizeBuffers( uint32 nMaxCharacters );

   uint32  m_nPointSize;      ///< Vertical size of this font, in points.
   uint32  m_nFlags;          ///< Flags which control font rendering behavior

   SuGPUVertexBufferPtr          m_pVB;            ///< Vertex buffer used for rendering this font
   SuGPUIndexBufferPtr           m_pIB;            ///< Index buffer used for rendering this font

   SuEffectPtr                   m_pEffect;        ///< The effect to use to render this font 
   SuGPUTexturePtr               m_pFontTexture;   ///< The font texture
   SuGPUSamplingResourceViewPtr  m_pView;          ///< A sampling view of the font texture
   SuCustomFontMetricsPtr        m_pFontMetrics;   ///< Font metrics for this custom font
};


#endif // _SU_CUSTOMFONT_H_
