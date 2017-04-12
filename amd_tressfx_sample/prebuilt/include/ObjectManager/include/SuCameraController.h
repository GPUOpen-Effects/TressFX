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

#ifndef _SU_CAMERACONTROLLER_H_
#define _SU_CAMERACONTROLLER_H_

#include "SuTypes.h"

class SuCamera;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A camera controller is responsible for moving the camera around in interesting ways.
/// 
//=================================================================================================================================

// tolua_begin
class SuCameraController
{
public:
// tolua_end

   /// Default constructor for camera controllers
   SU_DLL SuCameraController();

   /// Destructor for camera controllers
   virtual SU_DLL ~SuCameraController() ;

// tolua_begin
   /// Returns the camera that is attached to this camera controller
   inline SuCamera* GetCamera() const { return m_pCamera; };

   /// Attaches a camera to this camera controller
   inline void SetCamera( SuCamera* pCam ) { m_pCamera = pCam; };

   /// Updates the camera position
   virtual void Update( SuTime t ) = 0;

// tolua_end
private:

   SuCamera* m_pCamera; ///< The camera that is attached to this controller

// tolua_begin
};
// tolua_end



#endif // _SU_CAMERACONTROLLER_H_
