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

#ifndef _SU_AUDIOSOURCE_H
#define _SU_AUDIOSOURCE_H

class SuFile;

//=========================================================================================================
/// \ingroup Audio
/// \brief A class representing a source of audio
/// 
//=========================================================================================================
class SuAudioSource
{
public:
   /// Reads samples from the source and places them in the output buffer.
   /// \param bytes     The number of bytes of audio data to read
   /// \param buffer    Address of the output buffer
   /// \return The number of bytes actually read (may be less than bytes)
   virtual int ReadSamples(int bytes, void *buffer) = 0;
};

//=========================================================================================================
/// \ingroup Audio
/// \brief A class representing an audio file (which can be used as a source of audio)
/// 
//=========================================================================================================
class SuAudioFileSource : public SuAudioSource
{
public:
   /// Attaches a the source to a file
   /// \param pFile     Pointer to a file to attach as a source
   virtual bool Attach(SuFile *pFile) = 0;
};

#endif /* _SU_AUDIOSOURCE_H */
