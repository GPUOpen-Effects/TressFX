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

#ifndef _SUEFFECT_H
#define _SUEFFECT_H

#include "SuTypes.h"
#include "SuCommon.h"
#include "SuString.h"
#include "SuAnnotationMap.h"
#include "SuHashMap.h"
#include "SuCountedPtr.h"

// forward declarations
class SuEffectParameter;
class SuSampler;
class SuVertexStreamDecl;
class SuStreamMap;
class SuEffectTechnique;
class SuEffectPass;
class SuGPUVertexBuffer;
class SuGPUIndexBuffer;
class SuGPUSamplingResourceView;
class SuTextureSlot;
class SuUAVSlot;
class SuTexture;


/// String -> texture slot map
/// \ingroup EffectManager
typedef SuHashMap< SuString, SuTextureSlot*, SuStringHasher, SuHashComparer<SuString> > SuTextureSlotMap;

/// String -> texture slot map
/// \ingroup EffectManager
typedef SuHashMap< SuString, SuUAVSlot*, SuStringHasher, SuHashComparer<SuString> > SuUAVSlotMap;

/// String -> sampler declaration map
/// \ingroup EffectManager
typedef SuHashMap<SuString, SuSampler*, SuStringHasher, SuHashComparer<SuString> > SuSamplerMap;

/// String -> effect parameter map
/// \ingroup EffectManager
typedef SuHashMap<SuString, SuEffectParameter*, SuStringHasher, SuHashComparer<SuString> > SuEffectParameterMap;

/// String -> vertex stream map
/// \ingroup EffectManager
typedef SuHashMap<SuString, SuVertexStreamDecl*, SuStringHasher, SuHashComparer<SuString> > SuVertexStreamDeclMap;


/// String -> stream map map (stupid name, AMAZING results!)
/// \ingroup EffectManager
typedef SuHashMap<SuString, SuStreamMap*, SuStringHasher, SuHashComparer<SuString> > SuStreamMapMap;

/// Array with pointers to effect techniques
/// \ingroup EffectManager
typedef SuArray<SuEffectTechnique*> SuEffectTechniqueArray;


/// \brief Effect class
///
/// \ingroup EffectManager

// tolua_begin
class SuEffect
{
public:
   /// Constructor
   SU_DLL SuEffect();

   /// Destructor
   virtual SU_DLL ~SuEffect();

   /// Starts using this effect for rendering
   SU_DLL bool Begin( const SuString& rTechniqueName, uint32* pNumPasses );

   /// Starts using this effect for rendering
   SU_DLL bool Begin( const SuEffectTechnique* pTechnique, uint32* pNumPasses );

   /// Starts using a pass from the currently active technique
   SU_DLL bool BeginPass( uint32 nPassIndex );

   /// Ends using the currently active pass
   SU_DLL bool EndPass();

   /// Ends using this effect for rendering
   SU_DLL bool End();

   /// Retrieves the current pass
   inline const SuEffectPass* GetCurrentPass() const { return m_pCurrentPass; }

   /// Binds a vertex buffer to a stream
   SU_DLL bool BindVertexBuffer( const SuString& rVertexStreamName, const SuGPUVertexBuffer* pVB, uint32 nBaseVertexIndex = 0);

   /// Binds a vertex buffer to a stream without the overhead of a name lookup
   SU_DLL bool BindVertexBuffer( const SuVertexStreamDecl* pStream, const SuGPUVertexBuffer* pVB, uint32 nBaseVertexIndex = 0);

   /// Returns the currently bound vertex buffer.
   SU_DLL const SuGPUVertexBuffer* GetBoundVertexBuffer( const SuString& rVertexStreamName ) const;


   /// Binds an index buffer to use with this effect
   SU_DLL void BindIndexBuffer( const SuGPUIndexBuffer* pIB );

   /// Returns the index buffer currently bound to this effect
   SU_DLL const SuGPUIndexBuffer* GetBoundIndexBuffer() const;

   /// Bind a texture to a texture declaration
   SU_DLL bool BindTexture(const SuString& rTexName, const SuCountedPtr<SuGPUSamplingResourceView>& pTex );

   /// Binds a texture to a texture declaration wtihout the overhead of a name lookup
   SU_DLL bool BindTexture(const SuTextureSlot* pSlot, const SuCountedPtr<SuGPUSamplingResourceView>& pTex );

   /// Returns the texture currently bound to a texture declaration.
   SU_DLL const SuCountedPtr<SuGPUSamplingResourceView> GetBoundTexture( const SuString& rTexName ) const;

   /// Returns the art notes.
   inline const SuString& GetArtNotes() const { return m_artNotes; }

   /// Returns a texture slot description for a texture with a particular name.
   SU_DLL const SuTextureSlot* GetTextureSlot( const SuString& rName ) const;

   /// Returns a key=value pair map of the textures declared in this effect
   ///
   /// \return a reference to the texture declaration map
   inline const SuTextureSlotMap& GetTextureSlots() const { return m_textureSlots; }


   /// Returns a UAV slot description for a texture with a particular name.
   SU_DLL const SuUAVSlot* GetUAVSlot( const SuString& rName ) const;

   /// Returns a key=value pair map of the textures declared in this effect
   ///
   /// \return a reference to the texture declaration map
   inline const SuUAVSlotMap& GetUAVSlots() const { return m_uavSlots; }

   /// Returns a sampler declaration for a sampler with a particular name.
   SU_DLL const SuSampler* GetSampler( const SuString& rName ) const;

   /// Returns a key=value pair map of the samplers declared in this effect
   ///
   /// \return a reference to the sampler declaration map
   inline const SuSamplerMap& GetSamplers() const { return m_samplers; }

   /// Returns an effect parameter with a particular name.
   SU_DLL SuEffectParameter* GetParameter( const SuString& rName );

   /// Returns an effect parameter with a particular name.
   SU_DLL const SuEffectParameter* GetParameter( const SuString& rName ) const;

   /// Returns a key=value pair map of the parameters declared in this effect
   ///
   /// \return a reference to the effect parameter map
   inline const SuEffectParameterMap& GetParameters() const { return m_parameters; }

   /// Returns a vertex stream declaration for a stream with a particular name.
   SU_DLL const SuVertexStreamDecl* GetVertexStreamDecl( const SuString& rName ) const;

   /// Returns a key=value pair map of the vertex streams declared in this effect
   ///
   /// \return a reference to the vertex stream declaration map
   inline const SuVertexStreamDeclMap& GetVertexStreamDecls() const { return m_streamDecls; }

   /// Returns a stream map for a map with a particular name.
   SU_DLL const SuStreamMap* GetStreamMap( const SuString& rName ) const;

   /// Returns a key=value pair map of the stream maps declared in this effect
   ///
   /// \return a reference to the map of stream maps
   inline const SuStreamMapMap& GetStreamMaps() const { return m_streamMaps; }

   /// Returns a technique with a particular name.
   SU_DLL const SuEffectTechnique* GetTechnique( const SuString& rName ) const;

   /// Returns an array with the techniques in this effect
   ///
   /// \return a reference to the technique array
   inline const SuEffectTechniqueArray& GetTechniques() const { return m_techniques; }

   /// Returns a key=value pair map of the global effect options
   ///
   /// \return a pointer to the global effect option map, or NULL if no options are set
   inline const SuAnnotationMap* GetOptions() const { return m_pGlobalOptions; }

   /// Returns an array of the files that are directly or indirectly included from this effect.
   ///
   /// \return A reference to the file name array.
   inline const SuStringArray& GetIncludedFileNames() const { return m_includedFileNames; }

   /// Checks whether a particular file is included directly or indirectly by this effect.
   SU_DLL bool IsFileIncluded( const SuString& rFileName ) const;

// tolua_end

   /// Locates a technique which possesses all of the specified properties and returns a pointer to its name
   SU_DLL const SuString* FindTechnique( uint32 nProperties, const SuString* pProperties ) const;

protected:
   friend class SuEffectBuilder;

   /// Sets the art notes
   inline void SetArtNotes( const SuString& rArtNotes ) { m_artNotes = rArtNotes; }

   /// Sets a texture declaration. The caller gives up ownership of the SuTextureSlot object passed in.
   SU_DLL void SetTextureSlot( SuTextureSlot* pTextureSlot );

   /// Sets a UAV declaration. The caller gives up ownership of the SuTextureSlot object passed in.
   SU_DLL void SetUAVSlot( SuUAVSlot* pUAVSlot );

   /// Sets a sampler declaration. The caller gives up ownership of the SuSampler object passed in.
   SU_DLL void SetSampler( SuSampler* pSampler );

   /// Sets an effect parameter. The caller gives up ownership of the SuEffectParameter object passed in.
   SU_DLL void SetParameter( const SuString& rName, SuEffectParameter* pParam );

   /// Sets a vertex stream declaration. The caller gives up ownership of the SuVertexStreamDecl object passed in.
   SU_DLL void SetVertexStreamDecl( const SuString& rName, SuVertexStreamDecl* pStream );

   /// Sets a stream map declaration. The caller gives up ownership of the SuStreamMap object passed in.
   SU_DLL void SetStreamMap( const SuString& rName, SuStreamMap* pStreamMap );

   /// Sets an effect technique. The caller gives up ownership of the SuEffectTechnique object passed in.
   SU_DLL void SetTechnique( const SuString& rName, SuEffectTechnique* pTechnique );

   /// Sets a set of global effect options. The caller gives up ownership of the SuAnnotationMap object passed in.
   SU_DLL void SetOptions( SuAnnotationMap* pOptions );

   /// Adds a filename to the list of files directly or indirectly included in this effect.
   SU_DLL void AddIncludedFile( const SuString& rIncludeFileName );

   /// Returns a non-const technique with a particular name.
   SU_DLL SuEffectTechnique* GetChangableTechnique( const SuString& rName );

   /// Returns a non-const vertex stream with a particular name.
   SU_DLL SuVertexStreamDecl* GetNonConstVertexStreamDecl( const SuString& rName );

   /// Optimizes the effect by releasing intermediate memory
   SU_DLL void Finalize();

private:

   /// Invokes the texture slot binders or stream binders for any slots or streams in the effect which have them
   void ExecuteBinders();

   /// Updates any 'mimic' streams
   void UpdateMimicStreams();

   
   SuString m_artNotes;

   SuUAVSlotMap m_uavSlots;
   SuTextureSlotMap m_textureSlots;
   SuSamplerMap m_samplers;
   SuEffectParameterMap m_parameters;
   SuVertexStreamDeclMap m_streamDecls;

   /// The index buffer bound to the effect. I'm not sure yet if SuEffect is
   /// the right place to bind VBs and IBs, but that's what it is for now.
   const SuGPUIndexBuffer* m_pBoundIB;

   /// Map of technique property strings to techniques which have that property. 
   typedef SuHashMap< SuString, SuList<SuEffectTechnique*>, SuStringHasher, SuHashComparer<SuString> > TechniquePropertyMap;

   TechniquePropertyMap m_techniqueProperties;  ///< Techniques, grouped by property.  Does NOT take into account abstract techniques

   typedef SuArray< SuPair<const SuVertexStreamDecl*, const SuVertexStreamDecl*> > MimicStreamList;
   MimicStreamList m_mimicStreamPairs;  ///< Set of 'mimic' streams, which mirror streams bound to other streams


   SuStreamMapMap m_streamMaps;
   SuAnnotationMap* m_pGlobalOptions;
   SuEffectTechniqueArray m_techniques;
   SuStringArray m_includedFileNames;

   const SuEffectTechnique* m_pCurrentTechnique;
   const SuEffectPass* m_pCurrentPass;

// tolua_begin
};
// tolua_end


typedef SuCountedPtr<SuEffect> SuEffectPtr;


#endif
