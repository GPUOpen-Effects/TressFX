//--------------------------------------------------------------------------------------
// File: TFXFileIO.h
//
// Reads and Writes TressFX viewer files.
//
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
////--------------------------------------------------------------------------------------



#include "TFXFileIO.h"
#include "AMD_TressFX.h"
#include "StringTokenizer.h"
#include "assert.h"


TFXProjectFile::TFXCapsule::TFXCapsule(float x_, float y_, float z_, float r_)
    : x0(x_), y0(y_), z0(z_), x1(x_), y1(y_), z1(z_), r(r_), shapeType(SPHERE) {}


TFXProjectFile::TFXCapsule::TFXCapsule(float x0_, float y0_, float z0_, float x1_, float y1_, float z1_, float r_)
    : x0(x0_), y0(y0_), z0(z0_), x1(x1_), y1(y1_), z1(z1_), r(r_), shapeType(CAPSULE) {}


    bool ReadTFXSimFile(const wchar_t* simFilepath, TressFXSimFileObject* sim)
    {
        std::wifstream inFile(simFilepath);

        if ( !inFile.is_open() )
            return false;

        std::wstring sLine;
        std::vector<std::wstring> sTokens;

        while (!inFile.eof())
        {
            getline(inFile, sLine);

            if ( sLine.length() == 0 )
                continue;

            sTokens.clear();
            int numFound = StringTokenizer(sLine, std::wstring(L" "), sTokens, false);

            if ( numFound < 2)
                continue;

            if ( sTokens[0] == L"version" )
            {
                sim->version = _wtoi(sTokens[1].c_str());
            }
            else if ( sTokens[0] == L"damping" )
            {
                sim->damping = (float)_wtof(sTokens[1].c_str());
            }
            else if ( sTokens[0] == L"stiffnessForLocalShapeMatching" )
            {
                sim->stiffnessForLocalShapeMatching = (float)_wtof(sTokens[1].c_str());
            }
            else if ( sTokens[0] == L"stiffnessForGlobalShapeMatching" )
            {
                sim->stiffnessForGlobalShapeMatching = (float)_wtof(sTokens[1].c_str());
            }
            else if ( sTokens[0] == L"globalShapeMatchingEffectiveRange" )
            {
                sim->globalShapeMatchingEffectiveRange = (float)_wtof(sTokens[1].c_str());
            }
        }

        inFile.close();
        return true;
    }

    bool WriteTFXSimFile(const wchar_t* simFilepath, const TressFXSimFileObject& sim)
    {
        std::wofstream outFile(simFilepath);

        if ( !outFile.is_open() )
            return false;

        outFile << L"version 3.0\n";
        outFile << L"damping " << sim.damping << L"\n";
        outFile << L"stiffnessForLocalShapeMatching " << sim.stiffnessForLocalShapeMatching << L"\n";
        outFile << L"stiffnessForGlobalShapeMatching " << sim.stiffnessForGlobalShapeMatching << L"\n";
        outFile << L"globalShapeMatchingEffectiveRange " << sim.globalShapeMatchingEffectiveRange << L"\n";

        outFile.close();
        return true;
    }

    TFXProjectFile::TFXProjectFile()
    {
        Defaults();
    }

    bool TFXProjectFile::Read(const wchar_t* projectFilepath)
    {
        Defaults();

        mProjectFile = projectFilepath;

        wchar_t drive[_MAX_DRIVE];
        wchar_t dir[_MAX_DIR];
        wchar_t fname[_MAX_FNAME];
        wchar_t ext[_MAX_EXT];

        _wsplitpath_s(projectFilepath, drive, dir, fname, ext);
        std::wifstream inFile(projectFilepath);
        mProjectDir = dir;

        if ( !inFile.is_open() )
            return false;

        std::wstring sLine;
        std::vector<std::wstring> sTokens;

        while (!inFile.eof())
        {
            getline(inFile, sLine);

            if ( sLine.length() == 0 )
                continue;

            // If # is in the very first column in the line, it is a comment.
            if (sLine[0] == L'#')
                continue;

            sTokens.clear();
            int numFound = StringTokenizer(sLine, std::wstring(L" "), sTokens, false);

            std::wstring token;

            if ( numFound > 0 )
                token = sTokens[0];
            else
                token = sLine;

            if ( token.find(L"project_name") != std::string::npos )
            {
                assert(numFound >= 2);
                mProjectName = sTokens[1];
            }
            else if ( token.find(L".tfx") != std::string::npos && token.find(L".tfxskin") == std::string::npos )
            {
                assert(numFound >= 4);

                TFXFormat tfx;

                std::wstring path(dir);
                path += sTokens[0];

                // tfx file path
                tfx.tfxFileName = path;

                // follow-to-guide ratio. It could be zero.
                tfx.numFollowHairsPerGuideHair = _wtoi(sTokens[1].c_str());

                // radius around guide strand to grow follow strands
                tfx.radiusForFollowHair = (float)_wtof(sTokens[2].c_str());

                // follow strand tip separation factor from guide strand.
                tfx.tipSeparationFactor = (float)_wtof(sTokens[3].c_str());

                // tfxsim file path
                path = dir;
                path += sTokens[4];
                TressFXSimFileObject tfxsim;
                if ( ReadTFXSimFile(path.c_str(), &tfxsim) )
                {
                    mTFXSimFile.push_back(tfxsim);
                    mTFXSimFilename.push_back( path );
                }
                else
                {
                    ::MessageBoxA(0, "Could not load tfxsim file", "tfxproj read error", 0);
                }


                // section name for diplaying on GUI
                if ( numFound > 4 )
                    tfx.sectionNameForGUI = sTokens[5];
                else
                    tfx.sectionNameForGUI = L"";

                mTFXFile.push_back(tfx);
            }
            else if ( token.find(L".tfxskin") != std::string::npos )
            {
                std::wstring path(dir);
                path += token;
                mTFXSkinFile.push_back(path);
            }
            else if ( token.find(L".dds") != std::string::npos )
            {
                mTFXTexFile = dir;
                mTFXTexFile += token;
            }
            else if ( token.find(L".sdkmesh") != std::string::npos && token.find(L".sdkmesh_anim") == std::string::npos )
            {
                mMeshFile = dir;
                mMeshFile += token;
            }
            else if ( token.find(L".sdkmesh_anim") != std::string::npos )
            {
                mAnimFile = dir;
                mAnimFile += token;
                bAnimation = true;
            }
            else if ( token.find(L"lengthConstraintIterations") != std::string::npos )
            {
                lengthConstraintIterations =  _wtoi(sTokens[1].c_str());
            }
            else if ( token.find(L"localShapeMatchingIterations") != std::string::npos )
            {
                localShapeMatchingIterations =  _wtoi(sTokens[1].c_str());
            }
            else if ( token.find(L"windMag") != std::string::npos )
            {
                windMag =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"thickness") != std::string::npos )
            {
                thickness =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"density") != std::string::npos )
            {
                density =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"alpha") != std::string::npos )
            {
                alpha =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"shadowMapAlpha") != std::string::npos )
            {
                shadowMapAlpha =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"Kd") != std::string::npos )
            {
                Kd =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"Ks1") != std::string::npos )
            {
                Ks1 =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"Ex1") != std::string::npos )
            {
                Ex1 =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"Ks2") != std::string::npos )
            {
                Ks2 =  (float)_wtof(sTokens[1].c_str());
            }
            else if ( token.find(L"Ex1") != std::string::npos )
            {
                Ex2 =  (float)_wtof(sTokens[1].c_str());
            }
            else if (token.find(L"hairColor") != std::string::npos)
            {
                hairColor[0] = (float)_wtof(sTokens[1].c_str());
                hairColor[1] = (float)_wtof(sTokens[2].c_str());
                hairColor[2] = (float)_wtof(sTokens[3].c_str());
            }
            else if (token.find(L"collision_sphere") != std::string::npos)
            {
                TFXCapsule sphere(
                    (float)_wtof(sTokens[1].c_str()),
                    (float)_wtof(sTokens[2].c_str()),
                    (float)_wtof(sTokens[3].c_str()),
                    (float)_wtof(sTokens[4].c_str())
                    );

                mCollisionCapsule.push_back(sphere);
            }
            else if (token.find(L"collision_capsule") != std::string::npos)
            {
                TFXCapsule capsule(
                    (float)_wtof(sTokens[1].c_str()),
                    (float)_wtof(sTokens[2].c_str()),
                    (float)_wtof(sTokens[3].c_str()),
                    (float)_wtof(sTokens[4].c_str()),
                    (float)_wtof(sTokens[5].c_str()),
                    (float)_wtof(sTokens[6].c_str()),
                    (float)_wtof(sTokens[7].c_str())
                    );

                mCollisionCapsule.push_back(capsule);
            }
        }

        inFile.close();
        return true;
    }

    // Helps with readability at the expense of estra copies.
    static std::wstring GetFilename(std::wstring path)
    {
        wchar_t drive[_MAX_DRIVE];
        wchar_t dir[_MAX_DIR];
        wchar_t fname[_MAX_FNAME];
        wchar_t ext[_MAX_EXT];
        _wsplitpath_s(path.c_str(), drive, dir, fname, ext);
        return std::wstring(fname) + std::wstring(ext);
    }

    bool TFXProjectFile::Write(const wchar_t* projectFilepath)
    {
        mProjectFile = projectFilepath;

        wchar_t drive[_MAX_DRIVE];
        wchar_t dir[_MAX_DIR];
        wchar_t fname[_MAX_FNAME];
        wchar_t ext[_MAX_EXT];

        _wsplitpath_s(projectFilepath, drive, dir, fname, ext);
        mProjectDir = dir;

        std::wofstream outFile(projectFilepath);


        if ( !outFile.is_open() )
            return false;


        outFile << L"version 3.0\n";
        if(!mProjectName.empty())
            outFile << L"project_name " << GetFilename(mProjectName) <<L"\n\n";

        // write tfx files.
        for( size_t i = 0; i < mTFXFile.size(); ++i)
        {
            const TFXFormat& part = mTFXFile[i];
            outFile << GetFilename(part.tfxFileName) << L" " << part.numFollowHairsPerGuideHair << L" " << part.radiusForFollowHair << L" " << part.tipSeparationFactor << L" " << GetFilename(mTFXSimFilename[i]) << L" " << part.sectionNameForGUI << L"\n";
        }
        outFile << L"\n"; // extra space after tfx file list.

        // write tfxskin files.
        if(HasAnimation())
        {
            for( size_t i = 0; i < mTFXFile.size(); ++i)
            {
                outFile << GetFilename(mTFXSkinFile[i]) << L"\n";
            }
        }
        outFile << L"\n"; // extra space after tfxskin file list.


        outFile << GetFilename(mMeshFile) << L"\n";
        if(HasAnimation())
            outFile << GetFilename(mAnimFile) << L"\n";
        outFile << L"\n";

        outFile << L"lengthConstraintIterations " << lengthConstraintIterations << L"\n";
        outFile << L"localShapeMatchingIterations " << localShapeMatchingIterations << L"\n";
        outFile << L"windMag " << windMag << L"\n\n";

        outFile << L"thickness " << thickness << L"\n";
        outFile << L"density " << density << L"\n";
        outFile << L"alpha " << alpha << L"\n";
        outFile << L"shadowMapAlpha " << shadowMapAlpha << L"\n\n";

        outFile << L"Kd " << Kd << L"\n";
        outFile << L"Ks1 " << Ks1 << L"\n";
        outFile << L"Ex1 " << Ex1 << L"\n";
        outFile << L"Ks2 " << Ks2 << L"\n";
        outFile << L"Ex2 " << Ex2 << L"\n";
        outFile << L"hairColor " << hairColor[0] << L" " << hairColor[1] << L" " << hairColor[2] << L"\n";

        if(!mTFXTexFile.empty())
        {
            outFile << GetFilename(mTFXTexFile) << L"\n\n";
        }

        for( size_t i = 0; i < mCollisionCapsule.size(); ++i)
        {
            const TFXCapsule& capsule = mCollisionCapsule[i];
            if (capsule.shapeType == TFXCapsule::SPHERE)
            {
                outFile << L"collision_sphere " << capsule.x0 << L" " << capsule.y0 << L" " << capsule.z0 << L" " << capsule.r << L"\n";
            }
            else
            {
                outFile << L"collision_capsule "
                    << capsule.x0 << L" " << capsule.y0 << L" " << capsule.z0 << L" "
                    << capsule.x1 << L" " << capsule.y1 << L" " << capsule.z1 << L" "
                    << capsule.r << L"\n";
            }
        }

        outFile << L"\n";

        outFile.close();
        return true;
    }

void TFXProjectFile::SetHairParams(const AMD::TressFX_HairParams* params)
{
    thickness = params->thickness;
    density = params->density;
    alpha = params->alpha;
    shadowMapAlpha = params->shadowMapAlpha;
    hairColor[0] = params->color.x;
    hairColor[1] = params->color.y;
    hairColor[2] = params->color.z;
}

void TFXProjectFile::SetHairSimulationParams(const AMD::TressFX_SimulationParams* params)
{
    lengthConstraintIterations = params->numLengthConstraintIterations;
    localShapeMatchingIterations = params->numLocalShapeMatchingIterations;
    windMag = params->windMag;
}

void TFXProjectFile::Defaults()
{
        bAnimation = false;
        lengthConstraintIterations = 3;
        localShapeMatchingIterations = 3;
        windMag = 0;

        alpha = 0.5f;
        shadowMapAlpha = 0.004f;
        thickness = 0.15f;
        density = 1.0f;

        Kd = 0.4f;
        Ks1 = 0.04f;
        Ex1 = 80.0f;
        Ks2 = 0.5f;
        Ex2 = 8.0f;

        hairColor[0] = 115.f / 255.f;
        hairColor[1] = 84.f / 255.f;
        hairColor[2] = 56.f / 255.f;

        mTFXFile.clear();
        mTFXSimFile.clear();
        mTFXSimFilename.clear();
        mTFXSkinFile.clear();
        mTFXTexFile.clear();
        mMeshFile.clear();
        mAnimFile.clear();
        mProjectName.clear();
        mProjectDir.clear();
        mProjectFile.clear();
        mCollisionCapsule.clear();
}
