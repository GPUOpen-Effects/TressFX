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

#ifndef _SU_AUDIOCLIP_H
#define _SU_AUDIOCLIP_H

#include "SuTypes.h"

//=========================================================================================================
/// \ingroup Audio
/// \brief A class representing an audio sample
/// 
//=========================================================================================================
// tolua_begin
class SuAudioClip
{
public:
   /// Destructor
   virtual ~SuAudioClip() {}

   enum PlayMode
   {
      PM_EXCLUSIVE,
      PM_MULTIPLE
   };

   /// Returns the play mode for the sound
   inline PlayMode GetPlayMode() const { return m_playMode; }
   
   /// Enables looping for the sound.  Must be called before playing
   inline void SetLoop( bool bLoop ) { m_bLoop = bLoop; }
   
   /// Queries whether or not the sound is looping
   inline bool IsLooping() const { return m_bLoop; }

   /// Seeks to a particular position in the clip.  This should only be called for EXCLUSIVE sounds, and only when the sound is playing
   /// \param fSeekTime    Time (in seconds) to seek to
   /// \param bAbsolute    If true, this is an absolute time (offset from beginning), otherwise, it is relative to the current position
   /// \return True if successful, false on failure
   virtual bool Seek( float32 fSeekTime, bool bAbsolute = true ) = 0;
   
   /// \param fStartSeekTime  Time at which to start playing, in seconds
   /// \param fFadeInTime     Amount of time over which to fade the sound in (in seconds)
   /// \return True if successful, false on error
   virtual bool Play( float32 fStartSeekTime = 0.0f, float32 fFadeInTime = 0.0f ) = 0;
   
   /// \param fFadeOutTime Length of time over which to fade out the sound, in seconds
   /// \return True if successful, false on error
   virtual bool Stop( float32 fFadeOutTime = 0.0f ) = 0;

   /// Returns the length of the clip, in seconds
   virtual float32 GetLength() const = 0;

   /// Returns the current position within the clip, in seconds
   virtual float32 GetCurrentPosition() const = 0;

   /// Returns the volume of the clip.  Volume is a normalized value (between 0 and 1)
   /// Returns a value of -1 if an error occurs
   virtual float32 GetVolume() const = 0;

   /// Sets the volume of the clip
   /// \return True if successful.  False if the sound is not playing, or if an error occurs
   virtual bool SetVolume( float32 fVolume ) = 0;

   /// Tests whether the sound is playing
   /// \return True if the sound is playing.  False otherwise
   virtual bool IsPlaying() const = 0;

// tolua_end

protected:
   /// Constructor
   SuAudioClip( PlayMode playMode = PM_MULTIPLE ) : m_playMode( playMode ), m_bLoop(false) {}

private:
   bool m_bLoop;
   PlayMode m_playMode;

// tolua_begin
};
// tolua_end

#endif // _SU_AUDIOCLIP_H
