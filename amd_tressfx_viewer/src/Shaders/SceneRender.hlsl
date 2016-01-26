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

//--------------------------------------------------------------------------------------
// File: RenderScene.hlsl
//
// Scene rendering (non-hair) shaders
//--------------------------------------------------------------------------------------

cbuffer cbPerFrame : register( b5 )
{
    matrix      g_mWorld            : packoffset( c0  );
    matrix      g_mViewProj         : packoffset( c4  );
    matrix      g_mInvViewProj      : packoffset( c8  );
    matrix      g_mViewProjLight    : packoffset( c12 );
    float4      g_vEye              : packoffset( c16 );
    float4      g_PointLightPos     : packoffset( c17 );
    float4      g_PointLightColor   : packoffset( c18 );
    float4      g_AmbientLightColor : packoffset( c19 );
};


cbuffer cbPerMaterial : register( b2 )
{
    float4      g_MatAmbient    : packoffset( c0 );
    float4      g_MatDiffuse    : packoffset( c1 );
    float4      g_MatSpecular   : packoffset( c2 );
    float4      g_ScalpColor    : packoffset( c3 );
};

struct VS_INPUT_SCENE
{
    float3 Position     : POSITION;     // vertex position
    float3 Normal       : NORMAL;       // this normal comes in per-vertex
    float2 Texcoord     : TEXCOORD0;    // vertex texture coords
};

struct VS_OUTPUT_SCENE
{
    float4 Position     : SV_POSITION; // vertex position
    float2 Texcoord     : TEXCOORD0;   // vertex texture coords
    float3 Normal       : TEXCOORD1;
    float3 WorldPos     : TEXCOORD2;
};

struct PS_SHADOW_OUTPUT
{
    float color : SV_TARGET;
    float depth : SV_DEPTH;
};

struct VS_OUTPUT_SM
{
    float4 Position     : SV_POSITION; // vertex position
    float3 WorldPos     : TEXCOORD0;
};


struct VSSceneIn
{
    float4 Pos  : POSITION;         //Position
    float3 Norm : NORMAL;           //Normal
    float2 Tex  : TEXCOORD;         //Texture coordinate
    float3 Tan : TANGENT;           //Normalized Tangent vector
    float3 Bin : BINORMAL;          //Normalized Binormal vector
};

struct VSSkinnedIn
{
    float3 Pos  : POSITION;         //Position
    float4 Weights : WEIGHTS;       //Bone weights
    uint4  Bones : BONES;           //Bone indices
    float3 Norm : NORMAL;           //Normal
    float2 Tex  : TEXCOORD;         //Texture coordinate
    float3 Tan : TANGENT;           //Normalized Tangent vector
    float3 Bin : BINORMAL;          //Normalized Binormal vector
};


struct PSSkinnedIn
{
    float4 Pos  : SV_POSITION;      //Position
    float3 vPos : POSWORLD;         //world space Pos
    float3 Norm : NORMAL;           //Normal
    float2 Tex  : TEXCOORD;         //Texture coordinate
    float3 Tangent : TANGENT;       //Normalized Tangent vector
};

struct SkinnedInfo
{
    float4 Pos;
    float3 Norm;
    float3 Tan;
};

struct VSPositionsOut
{
    float4 Pos  : POSITION;     //Position
};

//--------------------------------------------------------------------------------------
// Screen quad
//--------------------------------------------------------------------------------------
struct VS_INPUT_SCREENQUAD
{
    float3 Position     : POSITION;     // vertex position
    float3 Normal       : NORMAL;       // this normal comes in per-vertex
    float2 Texcoord     : TEXCOORD0;    // vertex texture coords
};

struct VS_OUTPUT_SCREENQUAD
{
    float4 vPosition : SV_POSITION;
    float2 vTex      : TEXCOORD;
};


Texture2D                               g_txShadow                  : register( t12 );
Buffer<float4>                          g_BufferBoneWorld           : register( t13 );
Texture2D                               g_txNorm                    : register( t14 );
Texture2D                               g_txSpec                    : register( t15 );
Texture2D                               g_txDiffuse                 : register( t16 );

SamplerState                            g_samLinearWrap             : register( s0 );
SamplerState                            g_samPointClamp             : register( s1 );
SamplerComparisonState                  g_samShadow                 : register( s2 );


//--------------------------------------------------------------------------------------
// FetchBoneTransform fetches a bone transformation
//--------------------------------------------------------------------------------------
matrix FetchBoneTransform( uint iBone )
{
    matrix mret;

    iBone *= 4;
    float4 row1 = g_BufferBoneWorld.Load( iBone );
    float4 row2 = g_BufferBoneWorld.Load( iBone + 1 );
    float4 row3 = g_BufferBoneWorld.Load( iBone + 2 );
    float4 row4 = g_BufferBoneWorld.Load( iBone + 3 );
    mret = float4x4( row1, row2, row3, row4 );

    return mret;
}

//--------------------------------------------------------------------------------------
// SkinVert skins a single vertex
//--------------------------------------------------------------------------------------
SkinnedInfo SkinVert( VSSkinnedIn Input )
{
    SkinnedInfo Output = (SkinnedInfo)0;

    float4 Pos = float4(Input.Pos,1);
    float3 Norm = Input.Norm;
    float3 Tan = Input.Tan;

    //Bone0
    uint iBone = Input.Bones.x;
    float fWeight = Input.Weights.x;
    matrix m = FetchBoneTransform( iBone );
    Output.Pos += fWeight * mul( Pos, m );
    Output.Norm += fWeight * mul( Norm, (float3x3)m );
    Output.Tan += fWeight * mul( Tan, (float3x3)m );

    //Bone1
    iBone = Input.Bones.y;
    fWeight = Input.Weights.y;
    m = FetchBoneTransform( iBone );
    Output.Pos += fWeight * mul( Pos, m );
    Output.Norm += fWeight * mul( Norm, (float3x3)m );
    Output.Tan += fWeight * mul( Tan, (float3x3)m );

    //Bone2
    iBone = Input.Bones.z;
    fWeight = Input.Weights.z;
    m = FetchBoneTransform( iBone );
    Output.Pos += fWeight * mul( Pos, m );
    Output.Norm += fWeight * mul( Norm, (float3x3)m );
    Output.Tan += fWeight * mul( Tan, (float3x3)m );

    //Bone3
    iBone = Input.Bones.w;
    fWeight = Input.Weights.w;
    m = FetchBoneTransform( iBone );
    Output.Pos += fWeight * mul( Pos, m );
    Output.Norm += fWeight * mul( Norm, (float3x3)m );
    Output.Tan += fWeight * mul( Tan, (float3x3)m );

    return Output;
}

//--------------------------------------------------------------------------------------
// Vertex shader used for streaming out skinned vertices
//--------------------------------------------------------------------------------------
VSPositionsOut SkinStreamOutVS(VSSkinnedIn input )
{
    VSPositionsOut output = (VSPositionsOut)0;

    SkinnedInfo vSkinned = SkinVert( input );

    matrix scale = {1.0, 0, 0, 0,
                    0, 1.0, 0, 0,
                    0, 0, 1.0, 0,
                    0, 0, 0, 1.0};
    matrix tfm = mul(scale, g_mWorld);

    output.Pos = mul(vSkinned.Pos, tfm);

    return output;
}

//--------------------------------------------------------------------------------------
// Vertex shader used for streaming out vertices
//--------------------------------------------------------------------------------------
VSPositionsOut StreamOutVS(VSSkinnedIn input )
{
    VSPositionsOut output = (VSPositionsOut)0;
    float4 pos = float4(input.Pos, 1);
    matrix scale = {1.0, 0, 0, 0,
                    0, 1.0, 0, 0,
                    0, 0, 1.0, 0,
                    0, 0, 0, 1.0};
    matrix tfm = mul(scale, g_mWorld);

    output.Pos = mul(pos, tfm);


    return output;
}

//--------------------------------------------------------------------------------------
// Vertex shader used for rendering skinned vertices
//--------------------------------------------------------------------------------------
PSSkinnedIn SkinningVS(VSSkinnedIn input )
{
    PSSkinnedIn output = (PSSkinnedIn)0;

    SkinnedInfo vSkinned = SkinVert( input );

    float4 pos = mul(vSkinned.Pos, g_mWorld);
    output.Pos = mul(pos, g_mViewProj );
    output.vPos = mul( vSkinned.Pos, g_mWorld ).xyz;
    output.Norm = normalize( mul( vSkinned.Norm, (float3x3)g_mWorld ) );
    output.Tangent = normalize( mul( vSkinned.Tan, (float3x3)g_mWorld ) );
    output.Tex = input.Tex;

    return output;
}

//--------------------------------------------------------------------------------------
// Vertex shader used for rendering scene objects
//--------------------------------------------------------------------------------------
PSSkinnedIn SceneVS(VSSceneIn input )
{
    PSSkinnedIn output = (PSSkinnedIn)0;

    float4 pos = mul(input.Pos, g_mWorld);
    output.Pos = mul(pos, g_mViewProj );
    output.vPos = mul( input.Pos, g_mWorld ).xyz;
    output.Norm = normalize( mul( input.Norm, (float3x3)g_mWorld ) );
    output.Tangent = normalize( mul( input.Tan, (float3x3)g_mWorld ) );
    output.Tex = input.Tex;

    return output;
}

//--------------------------------------------------------------------------------------
// Simple vertex shader for rendering a full screen quad
//--------------------------------------------------------------------------------------
VS_OUTPUT_SCREENQUAD VS_ScreenQuad(VS_INPUT_SCREENQUAD input)
{
    VS_OUTPUT_SCREENQUAD output;

    output.vPosition = float4(input.Position.xyz, 1.0);
    output.vTex = input.Texcoord.xy;

    return output;
}

//--------------------------------------------------------------------------------------
// VS_GenerateSkinnedSM
//
// Renders the scene geometry into a shadow map
//--------------------------------------------------------------------------------------
VS_OUTPUT_SM VS_GenerateSkinnedSM(VSSkinnedIn input )
{
    VS_OUTPUT_SM output = (VS_OUTPUT_SM)0;

    // skin the vertex
    SkinnedInfo vSkinned = SkinVert( input );

    // Transform the position from object space to homogeneous projection space
    float4 pos = mul(vSkinned.Pos, g_mWorld);
    output.Position = mul(pos, g_mViewProjLight );

    // output position
    output.WorldPos = pos.xyz;

    return output;
}

//--------------------------------------------------------------------------------------
// VS_GenerateSceneSM
//
// Renders the scene geometry into a shadow map
//--------------------------------------------------------------------------------------
VS_OUTPUT_SM VS_GenerateSceneSM(VSSceneIn input )
{
    VS_OUTPUT_SM output = (VS_OUTPUT_SM)0;

    // Transform the position from object space to homogeneous projection space
    float4 pos = mul(input.Pos, g_mWorld);
    output.Position = mul(pos, g_mViewProjLight );

    // output position
    output.WorldPos = pos.xyz;

    return output;
}

//--------------------------------------------------------------------------------------
// DoLighting
//
// Computes Phong Illumination
//--------------------------------------------------------------------------------------
void DoLighting(in float3 vPosition, in float3 vNorm, in float3 vViewDir, out float3 LightColorDiffuseResult, out float3 LightColorSpecularResult)
{
    float3 vToLight = g_PointLightPos.xyz - vPosition;
    float3 vLightDir = normalize(vToLight);

    LightColorDiffuseResult = g_PointLightColor.rgb * saturate(dot(vLightDir,vNorm));

    float exponent = g_MatSpecular.w;
    float3 vHalfAngle = normalize( vViewDir + vLightDir );
    LightColorSpecularResult = pow( saturate(dot( vHalfAngle, vNorm )), exponent );
}



//--------------------------------------------------------------------------------------
// ComputeSceneShading
//
// Computes Phong Illumination and does texturing (including normal map and specular mask)
//--------------------------------------------------------------------------------------
float4 ComputeSceneShading(float3 iPos, float3 iNormal, float3 iTangent, float2 tex, float amountLight)
{
    float3 texColor = g_txDiffuse.Sample( g_samLinearWrap, tex ).rgb;
    float specMask = g_txSpec.Sample( g_samLinearWrap, tex ).x;

    // get normal from normal map
    float3 vNorm = g_txNorm.Sample( g_samLinearWrap, tex ).xyz;
    vNorm *= 2;
    vNorm -= float3(1,1,1);

    // transform normal into world space
    float3 vBinorm = normalize( cross( iNormal, iTangent ) );
    float3x3 BTNMatrix = float3x3( vBinorm, iTangent, iNormal );
    vNorm = normalize(mul( vNorm, BTNMatrix ));

    float3 LightDiffuse, LightSpecular;
    float3 vEyeDir = normalize(g_vEye.xyz - iPos);
    DoLighting( iPos, vNorm, vEyeDir, LightDiffuse, LightSpecular );

    // define ambient, diffuse and specular term for material
    float3 matAmbient = g_MatAmbient.rgb;
    float3 matDiffuse = g_MatDiffuse.rgb;
    float3 matSpecular = g_MatSpecular.rgb;

    // vary the ambient slightly, just so it isn't so flat
    float fAmbientBlend = 0.5f * vNorm.y + 0.5f;
    float3 LightAmbient = 1.25f * g_AmbientLightColor.rgb * fAmbientBlend + 0.75f * g_AmbientLightColor.rgb * (1-fAmbientBlend);

    float3 vColor = texColor * ( matAmbient * LightAmbient + amountLight * matDiffuse * LightDiffuse ) + amountLight*specMask*(matSpecular * LightSpecular);

    return float4(vColor,1);
}


//--------------------------------------------------------------------------------------
// PS_RenderSceneShadow
//
// Renders the hair shadowmap into a color buffer to create a scene shadow (hair casting
// shadows on the scene). Since the shadow map is rendered as a textured full screen quad
// it needs to output depth so that depth compares can be made with the current depth
// buffer. In this case, the current depth buffer is the scene model. Depth compares are
// necessary because the hair wraps around the head model, and you don't want shadows cast
// from hair behind the head.
//--------------------------------------------------------------------------------------
PS_SHADOW_OUTPUT PS_RenderSceneShadow(VS_OUTPUT_SCREENQUAD input)
{
    PS_SHADOW_OUTPUT Output;

    float shadowMapVal = g_txShadow.Sample(g_samPointClamp,input.vTex).x;
    // Areas in shadow will have a value < 1. Output 0.5 (the darkness of the shadow) if in shadow, otherwise output 1
    Output.color = saturate(step(1, shadowMapVal) + 0.5);
    Output.depth = shadowMapVal;
    return Output;
}


//--------------------------------------------------------------------------------------
// PS_RenderScene
//
// Renders the scene geometry (everything except the hair). Use the shadow texture created
// by PS_RenderSceneShadow to modulate the light intensity on the model.
//--------------------------------------------------------------------------------------
[earlydepthstencil]
float4 PS_RenderScene(PSSkinnedIn input):SV_Target
{
    // Use light projected texture coordinates (same as shadowmap) since the shadow buffer was
    // generated from the hair shadow map.
    float4 projPosLight = mul(float4(input.vPos,1), g_mViewProjLight);
    float2 texSM = float2(projPosLight.x/projPosLight.w+1, -projPosLight.y/projPosLight.w+1)*0.5;
    float shadow = g_txShadow.SampleLevel(g_samLinearWrap, texSM, 0).x;
    return ComputeSceneShading(input.vPos, input.Norm, input.Tangent, input.Tex, shadow);
}

