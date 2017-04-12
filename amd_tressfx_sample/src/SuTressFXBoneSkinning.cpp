// ----------------------------------------------------------------------------
// Compute-based skinning.
// ----------------------------------------------------------------------------
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

#include "SuTressFXBoneSkinning.h"

// Sushi
#include "SuObjectManager.h"

// TressFX
#include "TressFXHairObject.h"
#include "TressFXLayouts.h"

int static SuStringTokenizer(const SuString&    input,
                             const SuString&    delimiter,
                             SuArray<SuString>& results,
                             bool               includeEmpties)
{
    int iPos   = 0;
    int newPos = -1;
    int sizeS2 = (int)delimiter.GetLength();
    int isize  = (int)input.GetLength();

    if (isize == 0 || sizeS2 == 0)
        return 0;

    SuArray<int> positions;
    newPos = (int)input.Find(delimiter, 0);

    if (newPos < 0)
        return 0;

    int numFound = 0;

    while (newPos >= iPos)
    {
        numFound++;
        positions.push_back(newPos);
        iPos   = newPos;
        newPos = (int)input.Find(delimiter, iPos + sizeS2);
    }

    if (numFound == 0)
        return 0;

    for (int i = 0; i <= (int)positions.size(); ++i)
    {
        SuString s("");

        if (i == 0)
        {
            s.Copy(input, i, positions[i]);
        }
        else
        {
            int offset = positions[i - 1] + sizeS2;

            if (offset < isize)
            {
                if (i == (int)positions.size())
                    s.Copy(input, offset);
                else if (i > 0)
                    s.Copy(input,
                           positions[i - 1] + sizeS2,
                           positions[i] - positions[i - 1] - sizeS2);
            }
        }

        if (includeEmpties || (s.GetLength() > 0))
            results.push_back(s);
    }

    return ++numFound;
}

SuTressFXBoneSkinning::SuTressFXBoneSkinning() {}

SuTressFXBoneSkinning::~SuTressFXBoneSkinning() {}

bool SuTressFXBoneSkinning::LoadTressFXCollisionMeshData(
    TressFXSkeletonInterface* skeletonInterface, const char* filePath)
{
    m_pTempIndices.clear();
    m_pTempNormals.clear();
    m_pTempVertices.clear();

    SuFile* inFile = SuFile::Open(filePath, "r");

    if (inFile->IsValid() == false)
        return false;

    SuString          sLine;
    SuArray<SuString> sTokens;

    int numOfBones;

    SuStringList  lines;
    SuStringArray boneNames;
    inFile->ReadTextLines(lines);

    for (SuStringList::iterator itr = lines.begin(); itr != lines.end(); itr++)
    {
        SuString& sLine = *itr;

        if (sLine.GetLength() == 0)
            continue;

        // If # is in the very first column in the line, it is a comment.
        if (sLine[0] == '#')
            continue;

        sTokens.clear();
        int numFound = SuStringTokenizer(sLine, " ", sTokens, false);

        SuString token;

        if (numFound > 0)
        {
            token = sTokens[0];
        }
        else
        {
            token = sLine;
        }

        // load bone names.
        if (token.Find("numOfBones") >= 0)
        {
            numOfBones    = atoi(sTokens[1]);
            int countBone = 0;

            while (1)
            {
                itr++;
                sLine = *itr;

                if (sLine.GetLength() == 0)
                    continue;

                // If # is in the very first column in the line, it is a comment.
                if (sLine[0] == '#')
                    continue;

                sTokens.clear();
                int numFound = SuStringTokenizer(sLine, " ", sTokens, false);
                boneNames.push_back(sTokens[1]);
                countBone++;

                if (countBone == numOfBones)
                    break;
            }
        }
        if (token.Find("numOfVertices") >= 0)  // load bone indices and weights for each strand
        {
            m_NumVertices    = (uint32)atoi(sTokens[1]);
            boneSkinningData = new AMD::TressFXBoneSkinningData[m_NumVertices];
            m_pTempVertices.resize(m_NumVertices);
            m_pTempNormals.resize(m_NumVertices);
            memset(boneSkinningData, 0, sizeof(AMD::TressFXBoneSkinningData) * m_NumVertices);

            int index = 0;

            while (1)
            {
                itr++;
                sLine = *itr;

                if (sLine.GetLength() == 0)
                    continue;

                // If # is in the very first column in the line, it is a comment.
                if (sLine[0] == '#')
                    continue;

                sTokens.clear();
                int numFound = SuStringTokenizer(sLine, " ", sTokens, false);
                assert(numFound == 15);

                int vertexIndex = atoi(sTokens[0]);
                assert(vertexIndex == index);

                AMD::TRESSFX::float3& pos = m_pTempVertices[index];
                pos.x                     = (float)atof(sTokens[1]);
                pos.y                     = (float)atof(sTokens[2]);
                pos.z                     = (float)atof(sTokens[3]);

                AMD::TRESSFX::float3& normal = m_pTempNormals[index];
                normal.x                     = (float)atof(sTokens[4]);
                normal.y                     = (float)atof(sTokens[5]);
                normal.z                     = (float)atof(sTokens[6]);

                AMD::TressFXBoneSkinningData skinData;

                int boneIndex   = atoi(sTokens[7]);
                int engineIndex = skeletonInterface->GetBoneIndexByName(boneNames[boneIndex]);
                skinData.boneIndex[0] = (float)engineIndex;

                boneIndex   = atoi(sTokens[8]);
                engineIndex = skeletonInterface->GetBoneIndexByName(boneNames[boneIndex]);
                skinData.boneIndex[1] = (float)engineIndex;

                boneIndex   = atoi(sTokens[9]);
                engineIndex = skeletonInterface->GetBoneIndexByName(boneNames[boneIndex]);
                skinData.boneIndex[2] = (float)engineIndex;

                boneIndex   = atoi(sTokens[10]);
                engineIndex = skeletonInterface->GetBoneIndexByName(boneNames[boneIndex]);
                skinData.boneIndex[3] = (float)engineIndex;

                skinData.weight[0] = (float)atof(sTokens[11]);
                skinData.weight[1] = (float)atof(sTokens[12]);
                skinData.weight[2] = (float)atof(sTokens[13]);
                skinData.weight[3] = (float)atof(sTokens[14]);

                boneSkinningData[index] = skinData;

                ++index;

                if (index == m_NumVertices)
                    break;
            }
        }
        else if (token.Find("numOfTriangles") >= 0)  // triangle indices
        {
            m_NumTriangles = atoi(sTokens[1]);
            int numIndices = m_NumTriangles * 3;
            m_pTempIndices.resize(numIndices);
            int index = 0;

            while (1)
            {
                itr++;
                sLine = *itr;

                if (sLine.GetLength() == 0)
                    continue;

                // If # is in the very first column in the line, it is a comment.
                if (sLine[0] == '#')
                    continue;

                sTokens.clear();
                int numFound = SuStringTokenizer(sLine, " ", sTokens, false);
                assert(numFound == 4);

                int triangleIndex = atoi(sTokens[0]);
                assert(triangleIndex == index);

                m_pTempIndices[index * 3 + 0] = atoi(sTokens[1]);
                m_pTempIndices[index * 3 + 1] = atoi(sTokens[2]);
                m_pTempIndices[index * 3 + 2] = atoi(sTokens[3]);


                ++index;

                if (index == m_NumTriangles)
                    break;
            }
        }
    }

    inFile->Close();
    delete inFile;

    return true;
}

void SuTressFXBoneSkinning::Initialize(EI_Device*  pDevice,
                                       EI_CommandContextRef commandContext, EI_StringHash name)
{
    // load an effect for rendering
    m_pRenderEffect = SuEffectManager::GetRef().LoadEffect(_T("oSDF.sufx"));

    // create a vertex and normal buffer
    size_t vertexBlockSize = GetSizeOfMeshElement();
    SuMemoryBufferPtr pVertexBuffer(SuMemoryBuffer::Allocate((uint32)(vertexBlockSize * m_NumVertices)));  // position + normal
    uint8* pDataVB = pVertexBuffer->GetData();

    for (size_t i = 0; i < m_NumVertices; ++i)
    {
        // position
        memcpy(&pDataVB[i * vertexBlockSize], &m_pTempVertices[i], sizeof(SuVector3));

        // normal
        memcpy(&pDataVB[i * vertexBlockSize + sizeof(SuVector3)], &m_pTempNormals[i], sizeof(SuVector3));
    }

    // create an index buffer
    SuMemoryBufferPtr pIndexBuffer(
        SuMemoryBuffer::Allocate((uint32)(sizeof(int32) * m_pTempIndices.size())));
    int32* pDataIB = (int32*)pIndexBuffer->GetData();
    memcpy(pDataIB, &m_pTempIndices[0], sizeof(int32) * m_pTempIndices.size());
    m_pIndexBuffer =
        SuGPUResourceManager::GetRef().CreateResourceIndex(SuGPUResource::GPU_RESOURCE_STATIC,
                                                           (uint32)sizeof(int32),
                                                           (uint32)m_pTempIndices.size(),
                                                           pIndexBuffer);

    // UAV
    m_CollMeshVertPositionsUAV = EI_CreateReadWriteSB(
        pDevice, (uint32)vertexBlockSize, m_NumVertices, TRESSFX_STRING_HASH("CollMesh"), name);

    AMD::EI_Barrier prepareMeshVertPositions[] = 
    {
        { m_CollMeshVertPositionsUAV, AMD::EI_STATE_UAV, AMD::EI_STATE_COPY_DEST }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(prepareMeshVertPositions), prepareMeshVertPositions);

    {
        void* pDest = EI_SB_Map(commandContext, *m_CollMeshVertPositionsUAV);
        TRESSFX_ASSERT(pDest);
        memcpy(pDest, pDataVB, vertexBlockSize * m_NumVertices);
        EI_SB_Unmap(commandContext, *m_CollMeshVertPositionsUAV);
    }

    // SRV
    {
        m_BoneSkinningDataSRV = EI_CreateReadOnlySB(pDevice,
                                                    sizeof(AMD::TressFXBoneSkinningData),
                                                    m_NumVertices,
                                                    TRESSFX_STRING_HASH("CSSkinningData"), name);

        void* pDest = EI_SB_Map(commandContext, *m_BoneSkinningDataSRV);
        TRESSFX_ASSERT(pDest);
        memcpy(pDest, boneSkinningData, sizeof(AMD::TressFXBoneSkinningData) * m_NumVertices);
        EI_SB_Unmap(commandContext, *m_BoneSkinningDataSRV);

        delete[] boneSkinningData;
    }

    {
        m_InitialVertexPositionsSRV = EI_CreateReadOnlySB(
            pDevice, (AMD::uint32)vertexBlockSize, m_NumVertices, TRESSFX_STRING_HASH("CSSkinningInitialVertPos"), name);

        void* pDest = EI_SB_Map(commandContext, *m_InitialVertexPositionsSRV);
        TRESSFX_ASSERT(pDest);
        memcpy(pDest, pVertexBuffer->GetData(), vertexBlockSize * m_NumVertices);
        EI_SB_Unmap(commandContext, *m_InitialVertexPositionsSRV);
    }

    {
        m_TrimeshVertexIndicesSRV = EI_CreateReadOnlySB(pDevice,
                                                        (uint32)sizeof(int32),
                                                        (uint32)m_pTempIndices.size(),
                                                        TRESSFX_STRING_HASH("CSSkinningMeshIndices"), name);

        void* pDest = EI_SB_Map(commandContext, *m_TrimeshVertexIndicesSRV);
        TRESSFX_ASSERT(pDataIB);
        memcpy(pDest, pDataIB, sizeof(int32) * m_pTempIndices.size());
        EI_SB_Unmap(commandContext, *m_TrimeshVertexIndicesSRV);
    }


    AMD::EI_Barrier finishUpload[] =
    {
        { m_CollMeshVertPositionsUAV, AMD::EI_STATE_COPY_DEST, AMD::EI_STATE_UAV },
        { m_BoneSkinningDataSRV, AMD::EI_STATE_COPY_DEST, AMD::EI_STATE_NON_PS_SRV },
        { m_InitialVertexPositionsSRV, AMD::EI_STATE_COPY_DEST, AMD::EI_STATE_NON_PS_SRV },
        { m_TrimeshVertexIndicesSRV, AMD::EI_STATE_COPY_DEST, AMD::EI_STATE_NON_PS_SRV }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(finishUpload), finishUpload);


    EI_UAV      UAVs[1];
    EI_SRV      SRVs[2];

    // Bind set
    UAVs[0] = EI_GetUAV(m_CollMeshVertPositionsUAV);

    SRVs[0] = EI_GetSRV(m_BoneSkinningDataSRV);
    SRVs[1] = EI_GetSRV(m_InitialVertexPositionsSRV);

    AMD::TressFXBindSet bindSet;
    bindSet.nSRVs = AMD_ARRAY_SIZE(SRVs);
    bindSet.nUAVs = AMD_ARRAY_SIZE(UAVs);
    bindSet.uavs  = UAVs;
    bindSet.srvs  = SRVs;
    m_pBindSet = EI_CreateBindSet(pDevice, bindSet);

    // release vertex and index arrays on CPU.
    m_pTempVertices.clear();
    m_pTempNormals.clear();
    m_pTempIndices.clear();

    // set color
    m_MeshColor = SuVector3(1.f, 0, 0);

    // load a compute shader
    m_pComputeEffectSkinning = SuEffectManager::GetRef().LoadEffect("cTressFXBoneSkinning.sufx");

    // TODO: Following code should be moved to TransarencyPlugin.cpp.
    EI_LayoutManagerRef layoutManager = GetLayoutManagerRef(m_pComputeEffectSkinning);
    CreateBoneSkinningLayout2(pDevice, layoutManager);
}

void SuTressFXBoneSkinning::Destroy(EI_Device* pDevice)
{
    EI_DestroyBindSet(pDevice, m_pBindSet);

    AMD_SAFE_RESOURCE_DELETE(pDevice, m_CollMeshVertPositionsUAV);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_TrimeshVertexIndicesSRV);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_BoneSkinningDataSRV);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_InitialVertexPositionsSRV);


    m_pRenderEffect          = SuEffectPtr(0);
    m_pComputeEffectSkinning = SuEffectPtr(0);

    m_pIndexBuffer  = SuGPUIndexBufferPtr(0);
}

void SuTressFXBoneSkinning::SetModel(const SuString& modelname)
{
    // Get a model by its name
    SuObject* pObject = SuObjectManager::GetRef().GetObjectByName(modelname);

    if (pObject == nullptr)
    {
        SuLogError("Could not find object called %s", modelname);
    }
    else
        m_pModel = dynamic_cast<SuAnimatedModel*>(pObject);

    SU_ASSERT(m_pModel);
}

void SuTressFXBoneSkinning::GetBoundingBox(AMD::tressfx_vec3& min, AMD::tressfx_vec3& max)
{
    SU_ASSERT(m_pModel);

    const SuBoundingBox& bbox = m_pModel->GetBoundingBox();
    SuPoint3             bmin = bbox.GetMinCorner();
    SuPoint3             bmax = bbox.GetMaxCorner();

    memcpy(&min.x, &bmin.x, sizeof(float) * 3);
    memcpy(&max.x, &bmax.x, sizeof(float) * 3);
}

void SuTressFXBoneSkinning::Update(EI_CommandContextRef commandContext, SuTime fTime)
{
    if (!m_pComputeEffectSkinning || !m_pModel)
        return;

    // update animation model before getting the skinning matrices.
    m_pModel->UpdateObject(fTime);

    //-----------------------------
    // 1. BoneSkinning
    //-----------------------------
    const SuArray<SuMatrix4>& m_BoneMatricesPerFrame = m_pModel->GetSkinningMatrices();

    // update constant buffer with bone skinning matrices
    m_pComputeEffectSkinning->GetParameter("g_BoneSkinningMatrix")
        ->SetRawValue(&m_BoneMatricesPerFrame[0],
                      SuMin((int)m_BoneMatricesPerFrame.size(), AMD_TRESSFX_MAX_NUM_BONES) *
                          sizeof(SuMatrix4));
    m_pComputeEffectSkinning->GetParameter("g_NumMeshVertices")->SetInt(m_NumVertices);

    // Binding UAVs and SRVs
    EI_Bind(commandContext, GetBoneSkinningMeshLayout(), *m_pBindSet);

    // Run BoneSkinning
    {
        int numDispatchSize =
            (int)ceil((float)m_NumVertices / (float)TRESSFX_SIM_THREAD_GROUP_SIZE);
        uint32   numPasses;
        SuString techniqueName = "TressFX_BoneSkinning";
        m_pComputeEffectSkinning->Begin(techniqueName, &numPasses);
        m_pComputeEffectSkinning->BeginPass(0);
        SuRenderManager::GetRef().Dispatch(numDispatchSize, 1, 1);
        m_pComputeEffectSkinning->EndPass();
        m_pComputeEffectSkinning->End();
    }

    // State transition for DX12
    AMD::EI_Barrier flushSkinnedVerts[] = { { m_CollMeshVertPositionsUAV, AMD::EI_STATE_UAV, AMD::EI_STATE_UAV } };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(flushSkinnedVerts), flushSkinnedVerts);
}

void SuTressFXBoneSkinning::DrawMesh(EI_CommandContextRef commandContext)
{
    // draw the collsion mesh
    if (m_NumVertices > 0)
    {
        bool ok = true;

        m_pRenderEffect->BindIndexBuffer(m_pIndexBuffer.get());
        m_pRenderEffect->GetParameter("cColor")->SetFloatVector(m_MeshColor);
        m_pRenderEffect->GetParameter("vLightDir")->SetFloatVector(SuVector3(1.0f, 1.0f, 1.0f));

        m_pRenderEffect->GetTextureSlot("vertices")->BindResource(m_CollMeshVertPositionsUAV->srv);

        uint32 nNumPasses;
        if (m_pRenderEffect->Begin("MeshUAV", &nNumPasses))
        {
            SU_ASSERT(nNumPasses == 1);

            if (m_pRenderEffect->BeginPass(0))
            {
                SuRenderManager::GetRef().DrawIndexed(SuRenderManager::TRIANGLE_LIST,
                                                      0,
                                                      m_NumVertices,
                                                      0,
                                                      m_pIndexBuffer->GetNumIndices(),
                                                      0,
                                                      m_pIndexBuffer->GetIndexSize());

                if (m_pRenderEffect->EndPass())
                    ok = true;
            }

            ok = m_pRenderEffect->End() && ok;  // always call end if begin succeeded.
        }
    }
}
