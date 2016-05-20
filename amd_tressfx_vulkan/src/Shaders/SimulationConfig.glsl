// If you change the value below, you must change it in TressFXAsset.h as well.
#define THREAD_GROUP_SIZE 64

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

#define MAX_VERTS_PER_STRAND 16

layout(std140, set = 0, binding = IDSRV_CONSTANTS_BUFFER) buffer Constants
{
	vec4 g_Wind;
	vec4 g_Wind1;
	vec4 g_Wind2;
	vec4 g_Wind3;

	int g_NumLengthConstraintIterations;
	int g_bCollision;

	float g_GravityMagnitude;
	float g_TimeStep;

	float g_Damping0;
	float g_StiffnessForLocalShapeMatching0;
	float g_StiffnessForGlobalShapeMatching0;
	float g_GlobalShapeMatchingEffectiveRange0;

	float g_Damping1;
	float g_StiffnessForLocalShapeMatching1;
	float g_StiffnessForGlobalShapeMatching1;
	float g_GlobalShapeMatchingEffectiveRange1;

	float g_Damping2;
	float g_StiffnessForLocalShapeMatching2;
	float g_StiffnessForGlobalShapeMatching2;
	float g_GlobalShapeMatchingEffectiveRange2;

	float g_Damping3;
	float g_StiffnessForLocalShapeMatching3;
	float g_StiffnessForGlobalShapeMatching3;
	float g_GlobalShapeMatchingEffectiveRange3;

	uint g_NumOfStrandsPerThreadGroup;
	uint g_NumFollowHairsPerGuideHair;
	float g_TipSeparationFactor;

	int g_bWarp;
	int g_NumLocalShapeMatchingIterations;
};

layout(std140, set = 0, binding = IDSRV_HEAD_TRANSFORM) buffer HeadTransform
{
	mat4 g_ModelTransformForHead;
	vec4 g_ModelRotateForHead; // quaternion
	int bSingleHeadTransform;
	float padding[3];
};

bool IsMovable(vec4 particle)
{
	return particle.w > 0;
}
