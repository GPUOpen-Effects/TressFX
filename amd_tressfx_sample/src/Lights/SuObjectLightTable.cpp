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

#include "PCH.h"
#include "SuObjectLightTable.h"

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuObjectLightTable::SuObjectLightTable() {}

//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuObjectLightTable::~SuObjectLightTable() {}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Adds a light influencing an object to the list
///
/// \param pObj The object the light influences
/// \param pLight The influencing light
//=================================================================================================================================
void SuObjectLightTable::AddActiveLightToObject(const SuObject* pObj, const SuLight* pLight)
{
    LightList& rList = GetLightList(pObj);
    rList.push_back(pLight);
}

//=================================================================================================================================
/// \return Reference to a list of all lights currently influencing the object
//=================================================================================================================================
const SuList<const SuLight*>& SuObjectLightTable::GetActiveLightsForObject(
    const SuObject* pObj) const
{
    return const_cast<SuObjectLightTable* const>(this)->GetLightList(pObj);
}

//=================================================================================================================================
/// Clears the list of lights for all objects
//=================================================================================================================================
void SuObjectLightTable::Clear()
{
    for (ObjectToLightMap::iterator it = m_objLightMap.begin(); it != m_objLightMap.end(); ++it)
    {
        it->second.clear();
    }
}


//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Returns the list of lights associated with an object. If the object isn't in the data
/// structure, it adds a new
/// empty list for it.
///
/// \param pObj The object for which to return the lights
///
/// \return A reference to the list of lights influencing the object
//=================================================================================================================================
SuObjectLightTable::LightList& SuObjectLightTable::GetLightList(const SuObject* pObj)
{
    ObjectToLightMap::iterator lightsIt = m_objLightMap.find(pObj);

    if (lightsIt == m_objLightMap.end())
    {
        // insert object for the first time
        m_objLightMap.insert(pObj, LightList());
        lightsIt = m_objLightMap.find(pObj);
        SU_ASSERT(lightsIt != m_objLightMap.end());
    }

    return lightsIt->second;
}
