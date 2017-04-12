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

#ifndef _SU_AUDIOMANAGER_H
#define _SU_AUDIOMANAGER_H

#include "SuTypes.h"
#include "SuSearchPath.h"
#include "SuAudioClip.h"

class SuSpectrum;

/// \defgroup Audio Audio stuff

//=========================================================================================================
/// \ingroup Audio
/// \brief A flash animation that can be rendered in a 3D window
/// 
//=========================================================================================================
// tolua_begin
class SuAudioManager
{
// tolua_end

protected:
   /// Constructor
   SuAudioManager();

public:
// tolua_begin

   /// Destructor
   virtual SU_DLL ~SuAudioManager();

   /// Singleton access
   ///
   /// \return Pointer to instance of audio manager
   static SU_DLL SuAudioManager* GetPtr();

   /// Singleton access
   ///
   /// \return Reference to instance of audio manager
   static SU_DLL SuAudioManager& GetRef();

   /// Delete singleton instance
   static SU_DLL void ShutDown();
  
   inline bool Initialize() { return true; }

   virtual SuAudioClip* LoadTrack( const SuString& rFilename, SuAudioClip::PlayMode pm = SuAudioClip::PM_EXCLUSIVE ) = 0;

   /// Return the current audio spectrum
   virtual SuSpectrum* GetSpectrum() = 0;

   /// Get the number of frequency samples of the spectrum returned by the audio manager
   virtual int32 GetSpectrumSize() = 0;

   SU_DLL void AddSearchPath( const SuString& rPath );
   SU_DLL bool FadeOutMasterVolume( float32 fFadeOutTime = 0.0f );

   /// Updates the audio
   SU_DLL void Update();

   // tolua_end
protected:
   /// Returns the current search path for audio files
   const SuSearchPath& GetSearchPath() const { return m_searchPath; }

   /// To be implemented by subclasses. This is called every frame to update the underlying audio layer
   virtual bool UpdateAudio() = 0;

   enum AudioCommandType
   {
      SU_AM_CMD_NONE = 0,
      SU_AM_CMD_FADE_OUT,
      SU_AM_CMD_FADE_IN,
      SU_AM_CMD_PLAY_NEXT,
   };

   struct AudioClip
   {
      uint32         channel;       ///< only used for sounds
      void*          pClipData;     ///< API dependant data
   };

   struct AudioCommand
   {
      SuHandle         handle;
      AudioCommandType cmd;
      
      // params
      bool            b[2];
      uint32          i[2];
      float32         f[4];
   };

   SuList<AudioCommand> m_commandList;

   SuList<AudioClip*> m_activeTracks;

   bool CommandExists( SuHandle handle, AudioCommandType cmd ) const;

   bool ClearCommands( SuHandle handle );

   void FreeAllTracks();

   static const float32 AUDIO_EPSILON;

private:
   static SuAudioManager* ms_pInstance;

   /// The path in which to search for audio files
   SuSearchPath m_searchPath;

// tolua_begin
};
// tolua_end

#endif // _SU_AUDIOMANAGER_H
