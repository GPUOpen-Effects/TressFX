//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef _SU_OBJECTLIGHTTABLE_H_
#define _SU_OBJECTLIGHTTABLE_H_

#include "SuHashMap.h"
#include "SuList.h"
#include "SuTypes.h"

class SuObject;
class SuLight;

//=========================================================================================================
/// \ingroup Engine
/// \brief A table that keeps track of which lights currently illuminate an object
///
//=========================================================================================================
// tolua_begin
class SuObjectLightTable
{
public:
    /// Constructor
    SuObjectLightTable();

    /// Destructor
    virtual ~SuObjectLightTable();

    /// Clears the list of lights for all objects
    void Clear();

    /// Adds a light influencing an object to the list
    void AddActiveLightToObject(const SuObject* pObj, const SuLight* pLight);

    // tolua_end

    /// Returns a list of all lights currently influencing an object
    const SuList<const SuLight*>& GetActiveLightsForObject(const SuObject* pObj) const;

private:
    typedef SuList<const SuLight*> LightList;  // ideally I'd use a set, not a list
    typedef SuHashMap<const SuObject*,
                      LightList,
                      SuPointerHasher<SuObject>,
                      SuHashComparer<const SuObject*> >
        ObjectToLightMap;

    /// Returns the list of lights influencing an object
    LightList& GetLightList(const SuObject* pObj);

    ObjectToLightMap m_objLightMap;
    // tolua_begin
};
// tolua_end


#endif  // _SU_OBJECTLIGHTTABLE_H_
