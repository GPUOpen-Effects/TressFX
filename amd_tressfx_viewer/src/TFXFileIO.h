//--------------------------------------------------------------------------------------
// File: TFXProject.h
//
// Reads the tfxproj file.
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

#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>

///TODO:  There's a lot of redundancy between this and "DemoModel"

namespace AMD
{
    struct TressFX_HairParams;
    struct TressFX_SimulationParams;
}

struct TressFXSimFileObject
{
    int version;                                  // .tfxsim file version number (unsigned integer)
    float damping;                                // Floating point value for damping, stored as a string.
    float stiffnessForLocalShapeMatching;         // Floating point local stiffness, stored as a string.
    float stiffnessForGlobalShapeMatching;        // Floating point global stiffness, stored as a string.
    float globalShapeMatchingEffectiveRange;      // Floating point effective range of shape matching within a strand, stored as a string
    float reserved[32];
};

bool ReadTFXSimFile(const wchar_t* simFilepath, TressFXSimFileObject* sim);
bool WriteTFXSimFile(const wchar_t* simFilepath, const TressFXSimFileObject& sim);

struct TFXProjectFile
{
    TFXProjectFile();

    struct TFXFormat
    {
        std::wstring tfxFileName;
        int numFollowHairsPerGuideHair;
        float radiusForFollowHair;
        float tipSeparationFactor;
        std::wstring sectionNameForGUI;
    };

    struct TFXCapsule
    {
        // Two center points and a radius
        float x0, y0, z0, x1, y1, z1, r;
        enum {CAPSULE, SPHERE} shapeType;
        TFXCapsule(float x_, float y_, float z_, float r_);
        TFXCapsule(float x0_, float y0_, float z0_, float x1_, float y1_, float z1_, float r_);
    };


    // "File" is actually a path.
    std::wstring mProjectName;
    std::wstring mMeshFile;
    std::wstring mAnimFile;
    std::wstring mTFXTexFile;
    std::wstring mProjectDir;
    std::wstring mProjectFile;
    std::vector<TFXFormat> mTFXFile;
    std::vector<std::wstring> mTFXSkinFile;
    std::vector<TressFXSimFileObject> mTFXSimFile;
    std::vector<std::wstring> mTFXSimFilename;
    std::vector<TFXCapsule> mCollisionCapsule;

    // project level simulation and rendering settings
    int lengthConstraintIterations;
    int localShapeMatchingIterations;
    float windMag;
    float thickness;
    float density;
    float alpha;
    float shadowMapAlpha;
    float Ka;
    float Kd;
    float Ks1;
    float Ex1;
    float Ks2;
    float Ex2;
    float hairColor[3];
    bool customLightPos;
    float lightPos[3];

    void SetHairParams(const AMD::TressFX_HairParams* params);
    void SetHairSimulationParams(const AMD::TressFX_SimulationParams* params);


    int CountTFXFiles() const { return (int) mTFXFile.size(); }
    bool HasAnimation() const { return bAnimation; }

    bool Read(const wchar_t* projectFilepath);
    bool Write(const wchar_t* projectFilepath);


private:
    void Defaults();
    bool bAnimation;

};
