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

#ifndef _SU_FRAMESTATS_H_
#define _SU_FRAMESTATS_H_


// tolua_begin
/// \ingroup RenderManager
/// Enum of all of the statistics that the SuFrameStats class can track
enum SuFrameStatField
{
   SU_STAT_POINT_COUNT,
   SU_STAT_LINE_COUNT,
   SU_STAT_TRIANGLE_COUNT,
   SU_STAT_DRAWINDEXED_CALLS,
   SU_STAT_DRAWNONINDEXED_CALLS,
   SU_STAT_CLEAR_CALLS,

   SU_STAT_VB_LOCKS,
   SU_STAT_IB_LOCKS,
   SU_STAT_CB_LOCKS,

   SU_STAT_RT_SWITCHES,

   SU_STAT_PATCH_COUNT,

   SU_FRAME_STAT_COUNT
};
// tolua_end

// tolua_begin
//=================================================================================================================================
/// \ingroup RenderManager
/// \brief An object which keeps track of interesting per frame statistics
///  
//=================================================================================================================================
class SuFrameStats
{
public:

   SU_DLL SuFrameStats();

   /// Accessor for a field value
   SU_DLL uint32 GetValue( SuFrameStatField eField ) const;

   /// Changes a field value
   SU_DLL void IncrementValue( SuFrameStatField eField, uint32 nAmount=1 );

   /// Re-sets all field values to zero
   SU_DLL void Clear();

// tolua_end
private:

   uint32 m_nFrameStats[SU_FRAME_STAT_COUNT];

// tolua_begin
};
// tolua_end


#endif // _SU_FRAMESTATS_H_
