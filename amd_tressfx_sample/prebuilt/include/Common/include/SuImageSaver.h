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

#ifndef _SU_IMAGESAVER_H_
#define _SU_IMAGESAVER_H_

#include "SuTypes.h"
#include "SuImageData2D.h"

// tolua_begin
/// Enumeration of supported image file formats
enum SuImageFileFormat
{
   SU_IMAGE_PNG,
};


enum SuImageFlags
{
   // Disable saving out alpha to files for the time being for R600 performance testing
   SU_IMAGE_SAVEALPHA=0,
};
// tolua_end


// tolua_begin
// repeated counted pointer typedef for tolua's benefit
class SuImageData2D;
typedef SuCountedPtr<SuImageData2D> SuImageData2DPtr;
// tolua_end

//=================================================================================================================================
/// \ingroup Common
/// \brief  A class that knows how to export image data objects to files
///
//=================================================================================================================================
// tolua_begin
class SuImageSaver
{
public:

   SU_DLL SuImageSaver() {};

   /// Saves image data to a file
   SU_DLL bool SaveImage( const SuString& rFileName, SuImageFileFormat eFormat, SuImageData2DPtr pImageData, uint32 nFlags = 0 );

// tolua_end
private:

   /// PNG image saver helper
   static bool SaveImagePNG( const SuString& rFileName, SuImageData2DPtr pImageData, bool bSaveAlpha );

// tolua_begin
};
// tolua_end


#endif // _SU_IMAGESAVER_H_
