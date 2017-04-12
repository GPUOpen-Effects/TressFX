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

#include "Lights.h"

#include "SuBatchLineRenderer.h"
#include "SuLightManager.h"
#include "SuLightUpdateHandlerFactory.h"
#include "SuObjectLightTable.h"
#include "SuScriptManager.h"


bool Lights::Initialize()
{
    m_pObjToLightMapping = new SuObjectLightTable;
    SU_ASSERT(m_pObjToLightMapping != NULL);
    m_pLightUpdateHandlerFactory = new SuLightUpdateHandlerFactory(*m_pObjToLightMapping);
    SU_ASSERT(m_pLightUpdateHandlerFactory != NULL);

    m_pLightUpdateHandlerFactory->RegisterFactory();

    SuScriptManager::GetRef().SetVariable("Sushi.objToLightMapping", m_pObjToLightMapping, "SuObjectLightTable");

    return true;
}


void Lights::Shutdown()
{
    SuLightManager::ShutDown();
    SU_SAFE_DELETE(m_pLightUpdateHandlerFactory);
    SU_SAFE_DELETE(m_pObjToLightMapping);
}

void Lights::Update()
{
    // m_pObjToLightMapping->Clear();
}
