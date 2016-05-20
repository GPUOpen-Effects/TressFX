#pragma once

// shader resource view slots
#define IDSRV_PPLL                              0
#define IDSRV_HEAD_PPLL                         1
#define IDSRV_SCENESM                           2
#define IDSRV_HAIRSM                            3
#define IDSRV_HAIR_COLOR_TEXTURE                4
#define IDSRV_NOISEMAP                          5
#define IDSRV_HAIR_THICKNESSES                  6
#define IDSRV_HAIR_VERTEX_POSITIONS             7
#define IDSRV_HAIR_TANGENTS                     8
#define IDSRV_HAIR_TRANSFORMS                   9
#define IDSRV_HAIR_STRAND_TEX_COORDS            10
#define IDSRV_HAIR_VERTEX_TEX_COORDS            11
#define IDSRV_HAIR_VERTEX_COLORS                12

// Shortcut resources.
#define IDSRV_HAIR_FRAGMENT_DEPTHS              13
#define IDSRV_HAIR_FRAGMENT_COLORS              14
#define IDSRV_HAIR_ACCUM_INV_ALPHA              15

#define IDSRV_CONSTANTS_BUFFER                  16
#define IDSRV_ATOMIC_COUNTER_BUFFER             17
#define IDSRV_NOISE_SAMPLER                     18
#define IDSRV_SHADOW_SAMPLER                    19

#define IDSRV_HAIR_PREVIOUS_VERTEX_POSITIONS    20
#define IDSRV_HAIR_VERTEX_INITIAL_POSITIONS     21
#define IDSRV_HAIR_STRAND_TYPE                  22
#define IDSRV_HAIR_GLOBAL_ROTATION              23
#define IDSRV_HAIR_LOCAL_REF_VEC                24
#define IDSRV_HAIR_ROOT_OFFSET                  25
#define IDSRV_HAIR_LENGTH                       26
#define IDSRV_HEAD_TRANSFORM                    27

#define SM_EPSILON 0.01
#define KERNEL_SIZE 5
#define PI 3.14
#define FLOAT_EPSILON 1e-7

#define SHORTCUT_MIN_ALPHA 0.02

// Number of depth layers to use.  2 or 3 supported.
#define SHORTCUT_NUM_DEPTHS 3

// Compute source color as weighted average of front fragments, vs blending in order.
#define SHORTCUT_WEIGHTED_AVERAGE 1

// Output color deterministically when fragments have the same depth.  Requires additional clear of colors resource.
#define SHORTCUT_DETERMINISTIC 1

layout(std140, set = 0, binding = IDSRV_CONSTANTS_BUFFER) uniform Constants
{
mat4 g_mWorld;
mat4 g_mViewProj;
mat4 g_mInvViewProj;
mat4 g_mViewProjLight;

vec3 g_vEye;
float g_fvFov;

vec4 g_AmbientLightColor;
vec4 g_PointLightColor;
vec4 g_PointLightPos;
vec4 g_MatBaseColor;
vec4 g_MatKValue; // Ka, Kd, Ks, Ex

float g_FiberAlpha;
float g_HairShadowAlpha;
float g_bExpandPixels;
float g_FiberRadius;

vec4 g_WinSize; // screen size

float g_FiberSpacing; // average spacing between fibers
float g_bThinTip;
float g_fNearLight;
float g_fFarLight;

int g_iTechSM;
int g_bUseCoverage;
int g_iStrandCopies; // strand copies that the transparency shader will produce
int g_iMaxFragments;

float g_alphaThreshold;
float g_fHairKs2; // for second highlight
float g_fHairEx2; // for second highlight

mat4 g_mInvViewProjViewport;
};

