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
// File: TresFXSimulation.hlsl
//
// Physics simulation of hair using compute shaders
//--------------------------------------------------------------------------------------

//constants that change frame to frame
cbuffer ConstBufferCS_Per_Frame : register(b0)
{
	float4 g_Wind;
	float4 g_Wind1;
	float4 g_Wind2;
	float4 g_Wind3;

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

	int g_NumLocalShapeMatchingIterations;

	int g_NumVerticesPerStrand; // should be 2^n (n is integer and greater and 3) and less than or equal to THREAD_GROUP_SIZE. i.e. 8, 16, 32 or 64
	float padding_0[3];
}

cbuffer ConstBufferCS_Collision_Capsule : register(b3)
{
	float4  g_cc0_center1AndRadius;
	float4  g_cc0_center2AndRadiusSquared;
	float4  g_cc1_center1AndRadius;
	float4  g_cc1_center2AndRadiusSquared;
	float4  g_cc2_center1AndRadius;
	float4  g_cc2_center2AndRadiusSquared;
}

cbuffer ConstBufferCS_GenerateTransforms : register(b4)
{
	row_major float4x4 g_ModelTransformForHeada;
}

cbuffer ConstBufferCS_HeadTransform : register(b5)
{
	row_major float4x4 g_ModelTransformForHead;
	float4 g_ModelRotateForHead; // quaternion
	int g_bSingleHeadTransform; // If true, then simulation is for hair and all hair strands will be affected by a single head transformation
	// If false, then simulation is for fur and each hair strand will have its own transformation which comes from associated skinned triangle
	float padding[3];
}

struct Transforms
{
	row_major float4x4  tfm;
	float4              quat;
};

struct HairToTriangleMapping
{
	uint    meshIndex;          // index to the mesh
	uint    triangleIndex;      // index to triangle in the skinned mesh that contains this hair
	float3  barycentricCoord;   // barycentric coordinate of the hair within the triangle
	uint    reserved;           // for future use
};

// UAVs
RWStructuredBuffer<float4>      g_HairVertexPositions       : register(u0);
RWStructuredBuffer<float4>      g_HairVertexPositionsPrev   : register(u1);
RWStructuredBuffer<float4>      g_HairVertexTangents        : register(u2);
RWStructuredBuffer<float4>      g_InitialHairPositions      : register(u3);
RWStructuredBuffer<float4>      g_GlobalRotations           : register(u4);
RWStructuredBuffer<float4>      g_LocalRotations            : register(u5);
RWStructuredBuffer<Transforms>  g_Transforms                : register(u6);

// SRVs
Buffer<float>                           g_HairRestLengthSRV         : register(t0);
Buffer<int>                             g_HairStrandType            : register(t1);
Buffer<float4>                          g_HairRefVecsInLocalFrame   : register(t2);
Buffer<float4>                          g_FollowHairRootOffset      : register(t3);
Buffer<float4>                          g_MeshVertices              : register(t4);
Buffer<float4>                          g_TransformedVerts          : register(t5);
StructuredBuffer<HairToTriangleMapping> g_HairToMeshMapping         : register(t6);


// If you change the value below, you must change it in TressFXAsset.h as well.
#define THREAD_GROUP_SIZE 64

groupshared float4 sharedPos[THREAD_GROUP_SIZE];
groupshared float  sharedLength[THREAD_GROUP_SIZE];

//--------------------------------------------------------------------------------------
//
//  Helper Functions for the main simulation shaders
//
//--------------------------------------------------------------------------------------
bool IsMovable(float4 particle)
{
	if ( particle.w > 0 )
		return true;
	return false;
}

float2 ConstraintMultiplier(float4 particle0, float4 particle1)
{
	if ( IsMovable(particle0) )
	{
		if ( IsMovable(particle1) )
			return float2(0.5, 0.5);
		else
			return float2(1, 0);
	}
	else
	{
		if ( IsMovable(particle1) )
			return float2(0, 1);
		else
			return float2(0, 0);
	}
}

float4 MakeQuaternion(float angle_radian, float3 axis)
{
	// create quaternion using angle and rotation axis
	float4 quaternion;
	float halfAngle = 0.5f * angle_radian;
	float sinHalf = sin(halfAngle);

	quaternion.w = cos(halfAngle);
	quaternion.xyz = sinHalf * axis.xyz;

	return quaternion;
}

float4 InverseQuaternion(float4 q)
{
	float lengthSqr = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;

	if ( lengthSqr < 0.001 )
		return float4(0, 0, 0, 1.0f);

	q.x = -q.x / lengthSqr;
	q.y = -q.y / lengthSqr;
	q.z = -q.z / lengthSqr;
	q.w = q.w / lengthSqr;

	return q;
}

float3 MultQuaternionAndVector(float4 q, float3 v)
{
	float3 uv, uuv;
	float3 qvec = float3(q.x, q.y, q.z);
		uv = cross(qvec, v);
	uuv = cross(qvec, uv);
	uv *= (2.0f * q.w);
	uuv *= 2.0f;

	return v + uv + uuv;
}

float4 MultQuaternionAndQuaternion(float4 qA, float4 qB)
{
	float4 q;

	q.w = qA.w * qB.w - qA.x * qB.x - qA.y * qB.y - qA.z * qB.z;
	q.x = qA.w * qB.x + qA.x * qB.w + qA.y * qB.z - qA.z * qB.y;
	q.y = qA.w * qB.y + qA.y * qB.w + qA.z * qB.x - qA.x * qB.z;
	q.z = qA.w * qB.z + qA.z * qB.w + qA.x * qB.y - qA.y * qB.x;

	return q;
}

void ApplyDistanceConstraint(inout float4 pos0, inout float4 pos1, float targetDistance, float stiffness = 1.0)
{
	float3 delta = pos1.xyz - pos0.xyz;
		float distance = max(length(delta), 1e-7);
	float stretching = 1 - targetDistance / distance;
	delta = stretching * delta;
	float2 multiplier = ConstraintMultiplier(pos0, pos1);

		pos0.xyz += multiplier[0] * delta * stiffness;
	pos1.xyz -= multiplier[1] * delta * stiffness;
}

void CalcIndicesInVertexLevelTotal(uint local_id, uint group_id, inout uint globalStrandIndex, inout uint localStrandIndex, inout uint globalVertexIndex, inout uint localVertexIndex, inout uint numVerticesInTheStrand, inout uint indexForSharedMem, inout uint strandType)
{
	indexForSharedMem = local_id;
	numVerticesInTheStrand = (THREAD_GROUP_SIZE / g_NumOfStrandsPerThreadGroup);

	localStrandIndex = local_id % g_NumOfStrandsPerThreadGroup;
	globalStrandIndex = group_id * g_NumOfStrandsPerThreadGroup + localStrandIndex;
	localVertexIndex = (local_id - localStrandIndex) / g_NumOfStrandsPerThreadGroup;

	strandType = g_HairStrandType.Load(globalStrandIndex);
	globalVertexIndex = globalStrandIndex * numVerticesInTheStrand + localVertexIndex;
}

void CalcIndicesInVertexLevelMaster(uint local_id, uint group_id, inout uint globalStrandIndex, inout uint localStrandIndex, inout uint globalVertexIndex, inout uint localVertexIndex, inout uint numVerticesInTheStrand, inout uint indexForSharedMem, inout uint strandType)
{
	indexForSharedMem = local_id;
	numVerticesInTheStrand = (THREAD_GROUP_SIZE / g_NumOfStrandsPerThreadGroup);

	localStrandIndex = local_id % g_NumOfStrandsPerThreadGroup;
	globalStrandIndex = group_id * g_NumOfStrandsPerThreadGroup + localStrandIndex;
	globalStrandIndex *= (g_NumFollowHairsPerGuideHair + 1);
	localVertexIndex = (local_id - localStrandIndex) / g_NumOfStrandsPerThreadGroup;

	strandType = g_HairStrandType.Load(globalStrandIndex);
	globalVertexIndex = globalStrandIndex * numVerticesInTheStrand + localVertexIndex;
}

void CalcIndicesInStrandLevelTotal(uint local_id, uint group_id, inout uint globalStrandIndex, inout uint numVerticesInTheStrand, inout uint globalRootVertexIndex, inout uint strandType)
{
	globalStrandIndex = THREAD_GROUP_SIZE*group_id + local_id;
	numVerticesInTheStrand = (THREAD_GROUP_SIZE / g_NumOfStrandsPerThreadGroup);
	strandType = g_HairStrandType.Load(globalStrandIndex);
	globalRootVertexIndex = globalStrandIndex * numVerticesInTheStrand;
}

void CalcIndicesInStrandLevelMaster(uint local_id, uint group_id, inout uint globalStrandIndex, inout uint numVerticesInTheStrand, inout uint globalRootVertexIndex, inout uint strandType)
{
	globalStrandIndex = THREAD_GROUP_SIZE*group_id + local_id;
	globalStrandIndex *= (g_NumFollowHairsPerGuideHair + 1);
	numVerticesInTheStrand = (THREAD_GROUP_SIZE / g_NumOfStrandsPerThreadGroup);
	strandType = g_HairStrandType.Load(globalStrandIndex);
	globalRootVertexIndex = globalStrandIndex * numVerticesInTheStrand;
}

//--------------------------------------------------------------------------------------
//
//  Integrate
//
//  Uses Verlet integration to calculate the new position for the current time step
//
//--------------------------------------------------------------------------------------
float4 Integrate(float4 curPosition, float4 oldPosition, float4 initialPos, float4 force, int globalVertexIndex, int localVertexIndex, int numVerticesInTheStrand, float dampingCoeff = 1.0f)
{
	float4 outputPos = curPosition;

		force.xyz += g_GravityMagnitude * float3(0, -1.0f, 0);
	outputPos.xyz = curPosition.xyz + (1.0 - dampingCoeff)*(curPosition.xyz - oldPosition.xyz) + force.xyz*g_TimeStep*g_TimeStep;

	return outputPos;
}


struct CollisionCapsule
{
	float4 p1; // xyz = position 1 of capsule, w = radius
	float4 p2; // xyz = position 2 of capsule, w = radius * radius
};

//--------------------------------------------------------------------------------------
//
//  CapsuleCollision
//
//  Moves the position based on collision with capsule
//
//--------------------------------------------------------------------------------------
bool CapsuleCollision(float4 curPosition, float4 oldPosition, inout float3 newPosition, CollisionCapsule cc, bool bOnlyFirstSphere = false, float friction = 0.4f)
{
	const float radius = cc.p1.w;
	const float radius2 = cc.p2.w;
	newPosition = curPosition.xyz;

	if ( !IsMovable(curPosition) )
		return false;

	float3 segment = cc.p2.xyz - cc.p1.xyz;
	float3 delta1 = curPosition.xyz - cc.p1.xyz;
	float3 delta2 = cc.p2.xyz - curPosition.xyz;

	float dist1 = dot(delta1, segment);
	float dist2 = dot(delta2, segment);

	// colliding with sphere 1
	if ( dist1 < 0.001f )
	{
		if ( dot(delta1, delta1) < radius2 )
		{
			float3 n = normalize(delta1);
			newPosition = radius * n + cc.p1.xyz;
			return true;
		}

		return false;
	}

	if ( bOnlyFirstSphere )
		return false;

	// colliding with sphere 2
	if ( dist2 < 0.f )
	{
		if ( dot(delta2, delta2) < radius2 )
		{
			float3 n = normalize(-delta2);
			newPosition = radius * n + cc.p2.xyz;
			return true;
		}

		return false;
	}

	// colliding with middle cylinder
	float3 x = (dist1 * cc.p2.xyz + dist2 * cc.p1.xyz) / (dist1 + dist2);
		float3 delta = curPosition.xyz - x;

		if ( dot(delta, delta) < radius2 )
		{
			float3 n = normalize(delta);
			float3 vec = curPosition.xyz - oldPosition.xyz;
			float3 segN = normalize(segment);
			float3 vecTangent = dot(vec, segN) * segN;
			float3 vecNormal = vec - vecTangent;
			newPosition = oldPosition.xyz + friction * vecTangent + (vecNormal + radius * n - delta);
			return true;
		}

	return false;
}

//--------------------------------------------------------------------------------------
//
//  UpdateFinalVertexPositions
//
//  Updates the  hair vertex positions based on the physics simulation
//
//--------------------------------------------------------------------------------------
void UpdateFinalVertexPositions(float4 oldPosition, float4 newPosition, int globalVertexIndex, int localVertexIndex, int numVerticesInTheStrand)
{
	g_HairVertexPositionsPrev[globalVertexIndex] = oldPosition;
	g_HairVertexPositions[globalVertexIndex] = newPosition;
}

//--------------------------------------------------------------------------------------
//
//  IntegrationAndGlobalShapeConstraints
//
//  Compute shader to simulate the gravitational force with integration and to maintain the
//  global shape constraints.
//
// One thread computes one vertex.
//
//--------------------------------------------------------------------------------------
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void IntegrationAndGlobalShapeConstraints(uint GIndex : SV_GroupIndex,
										  uint3 GId : SV_GroupID,
										  uint3 DTid : SV_DispatchThreadID)
{
	uint globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType;
	CalcIndicesInVertexLevelMaster(GIndex, GId.x, globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType);

	float4 currentPos = float4(0, 0, 0, 0); // position when this step starts. In other words, a position from the last step.
	float4 initialPos = float4(0, 0, 0, 0); // rest position

	// Copy data into shared memory
	initialPos = g_InitialHairPositions[globalVertexIndex];

	if ( g_bSingleHeadTransform )
		initialPos.xyz = mul(float4(initialPos.xyz, 1), g_ModelTransformForHead).xyz;
	else
		initialPos.xyz = mul(float4(initialPos.xyz, 1), g_Transforms[globalStrandIndex].tfm).xyz;

	currentPos = sharedPos[indexForSharedMem] = g_HairVertexPositions[globalVertexIndex];

	GroupMemoryBarrierWithGroupSync();

	// Integrate
	float dampingCoeff = 0.03f;

	if ( strandType == 0 )
		dampingCoeff = g_Damping0;
	else if ( strandType == 1 )
		dampingCoeff = g_Damping1;
	else if ( strandType == 2 )
		dampingCoeff = g_Damping2;
	else if ( strandType == 3 )
		dampingCoeff = g_Damping3;

	float4 oldPos = g_HairVertexPositionsPrev[globalVertexIndex];
	float4 force = float4(0, 0, 0, 0);

	if ( IsMovable(currentPos) )
		sharedPos[indexForSharedMem] = Integrate(currentPos, oldPos, initialPos, force, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, dampingCoeff);
	else
		sharedPos[indexForSharedMem] = initialPos;

	// Global Shape Constraints
	float stiffnessForGlobalShapeMatching = 0;
	float globalShapeMatchingEffectiveRange = 0;

	if ( strandType == 0 )
	{
		stiffnessForGlobalShapeMatching = g_StiffnessForGlobalShapeMatching0;
		globalShapeMatchingEffectiveRange = g_GlobalShapeMatchingEffectiveRange0;
	}
	else if ( strandType == 1 )
	{
		stiffnessForGlobalShapeMatching = g_StiffnessForGlobalShapeMatching1;
		globalShapeMatchingEffectiveRange = g_GlobalShapeMatchingEffectiveRange1;
	}
	else if ( strandType == 2 )
	{
		stiffnessForGlobalShapeMatching = g_StiffnessForGlobalShapeMatching2;
		globalShapeMatchingEffectiveRange = g_GlobalShapeMatchingEffectiveRange2;
	}
	else if ( strandType == 3 )
	{
		stiffnessForGlobalShapeMatching = g_StiffnessForGlobalShapeMatching3;
		globalShapeMatchingEffectiveRange = g_GlobalShapeMatchingEffectiveRange3;
	}

	if ( stiffnessForGlobalShapeMatching > 0 && globalShapeMatchingEffectiveRange )
	{
		if ( IsMovable(sharedPos[indexForSharedMem]) )
		{
			if ( (float)localVertexIndex < globalShapeMatchingEffectiveRange * (float)numVerticesInTheStrand )
			{
				float factor = stiffnessForGlobalShapeMatching;
				float3 del = factor * (initialPos - sharedPos[indexForSharedMem]).xyz;
					sharedPos[indexForSharedMem].xyz += del;
			}
		}
	}

	// update global position buffers
	UpdateFinalVertexPositions(currentPos, sharedPos[indexForSharedMem], globalVertexIndex, localVertexIndex, numVerticesInTheStrand);
}

//--------------------------------------------------------------------------------------
//
//  ApplyHairTransformGlobally
//
//  Compute shader to apply skin hair transforms to all vertices globally. 
//
//  One thread computes one vertex.
//
//--------------------------------------------------------------------------------------
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void ApplyHairTransformGlobally(uint GIndex : SV_GroupIndex, uint3 GId : SV_GroupID, uint3 DTid : SV_DispatchThreadID)
{
	uint globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType;
	CalcIndicesInVertexLevelMaster(GIndex, GId.x, globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType);

	float4 currentPos = sharedPos[indexForSharedMem] = g_HairVertexPositions[globalVertexIndex]; 
	float4 initialPos = g_InitialHairPositions[globalVertexIndex];

	if ( g_bSingleHeadTransform )
		initialPos.xyz = mul(float4(initialPos.xyz, 1), g_ModelTransformForHead).xyz;
	else
		initialPos.xyz = mul(float4(initialPos.xyz, 1), g_Transforms[globalStrandIndex].tfm).xyz;

	sharedPos[indexForSharedMem] = initialPos;

	// update global position buffers
	UpdateFinalVertexPositions(currentPos, sharedPos[indexForSharedMem], globalVertexIndex, localVertexIndex, numVerticesInTheStrand);
}

//--------------------------------------------------------------------------------------
//
//  LocalShapeConstraints
//
//  Compute shader to maintain the local shape constraints.
//
// One thread computes one strand.
//
//--------------------------------------------------------------------------------------
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void LocalShapeConstraints(uint GIndex : SV_GroupIndex,
						   uint3 GId : SV_GroupID,
						   uint3 DTid : SV_DispatchThreadID)
{
	uint local_id, group_id, globalStrandIndex, numVerticesInTheStrand, globalRootVertexIndex, strandType;
	CalcIndicesInStrandLevelMaster(GIndex, GId.x, globalStrandIndex, numVerticesInTheStrand, globalRootVertexIndex, strandType);

	// stiffness for local shape constraints
	float stiffnessForLocalShapeMatching = 0.4f;

	if ( strandType == 2 )
		stiffnessForLocalShapeMatching = g_StiffnessForLocalShapeMatching2;
	else if ( strandType == 3 )
		stiffnessForLocalShapeMatching = g_StiffnessForLocalShapeMatching3;
	else if ( strandType == 1 )
		stiffnessForLocalShapeMatching = g_StiffnessForLocalShapeMatching1;
	else if ( strandType == 0 )
		stiffnessForLocalShapeMatching = g_StiffnessForLocalShapeMatching0;

	//1.0 for stiffness makes things unstable sometimes.
	stiffnessForLocalShapeMatching = 0.5f*min(stiffnessForLocalShapeMatching, 0.95f);

	//--------------------------------------------
	// Local shape constraint for bending/twisting
	//--------------------------------------------
	{
		float4 pos = g_HairVertexPositions[globalRootVertexIndex + 1];
		float4 pos_plus_one;
		uint globalVertexIndex = 0;
		float4 rotGlobal = g_GlobalRotations[globalRootVertexIndex];

			for ( uint localVertexIndex = 1; localVertexIndex < numVerticesInTheStrand - 1; localVertexIndex++ )
			{
				globalVertexIndex = globalRootVertexIndex + localVertexIndex;
				pos_plus_one = g_HairVertexPositions[globalVertexIndex + 1];

				//--------------------------------
				// Update position i and i_plus_1
				//--------------------------------
				float4 rotGlobalWorld;

				if ( g_bSingleHeadTransform )
					rotGlobalWorld = MultQuaternionAndQuaternion(g_ModelRotateForHead, rotGlobal);
				else
					rotGlobalWorld = MultQuaternionAndQuaternion(g_Transforms[globalStrandIndex].quat, rotGlobal);

				float3 orgPos_i_plus_1_InLocalFrame_i = g_HairRefVecsInLocalFrame[globalVertexIndex + 1].xyz;
				float3 orgPos_i_plus_1_InGlobalFrame = MultQuaternionAndVector(rotGlobalWorld, orgPos_i_plus_1_InLocalFrame_i) + pos.xyz;

				float3 del = stiffnessForLocalShapeMatching * (orgPos_i_plus_1_InGlobalFrame - pos_plus_one.xyz).xyz;

				if ( IsMovable(pos) )
					pos.xyz -= del.xyz;

				if ( IsMovable(pos_plus_one) )
					pos_plus_one.xyz += del.xyz;

				//---------------------------
				// Update local/global frames
				//---------------------------
				float4 invRotGlobalWorld = InverseQuaternion(rotGlobalWorld);
				float3 vec = normalize(pos_plus_one.xyz - pos.xyz);

				float3 x_i_plus_1_frame_i = normalize(MultQuaternionAndVector(invRotGlobalWorld, vec));
				float3 e = float3(1.0f, 0, 0);
				float3 rotAxis = cross(e, x_i_plus_1_frame_i);

				if ( length(rotAxis) > 0.001 )
				{
					float angle_radian = acos(dot(e, x_i_plus_1_frame_i));
					rotAxis = normalize(rotAxis);

					float4 localRot = MakeQuaternion(angle_radian, rotAxis);
					rotGlobal = MultQuaternionAndQuaternion(rotGlobal, localRot);
				}

				g_HairVertexPositions[globalVertexIndex].xyz = pos.xyz;
				g_HairVertexPositions[globalVertexIndex + 1].xyz = pos_plus_one.xyz;

				pos = pos_plus_one;
			}
	}

	return;
}

//--------------------------------------------------------------------------------------
//
//  LocalShapeConstraintsWithIteration
//
//  Compute shader to maintain the local shape constraints. This is the same as
//  the LocalShapeConstraints shader, except the iterations are done on the GPU
//  instead of multiple dispatch calls on the CPU, for better performance
//
//  This kernel should be called only when the number vertices per strand is 8 or 16.
//--------------------------------------------------------------------------------------
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void LocalShapeConstraintsWithIteration(uint GIndex : SV_GroupIndex,
										uint3 GId : SV_GroupID,
										uint3 DTid : SV_DispatchThreadID)
{
	uint local_id, group_id, globalStrandIndex, numVerticesInTheStrand, globalRootVertexIndex, strandType;
	CalcIndicesInStrandLevelMaster(GIndex, GId.x, globalStrandIndex, numVerticesInTheStrand, globalRootVertexIndex, strandType);

	// stiffness for local shape constraints
	float stiffnessForLocalShapeMatching = 0.4f;

	if ( strandType == 2 )
		stiffnessForLocalShapeMatching = g_StiffnessForLocalShapeMatching2;
	else if ( strandType == 3 )
		stiffnessForLocalShapeMatching = g_StiffnessForLocalShapeMatching3;
	else if ( strandType == 1 )
		stiffnessForLocalShapeMatching = g_StiffnessForLocalShapeMatching1;
	else if ( strandType == 0 )
		stiffnessForLocalShapeMatching = g_StiffnessForLocalShapeMatching0;

	//1.0 for stiffness makes things unstable sometimes.
	stiffnessForLocalShapeMatching = 0.5f*min(stiffnessForLocalShapeMatching, 0.95f);

	//------------------------------
	// Copy strand data into registers, for faster iteration
	//------------------------------
	uint globalVertexIndex = 0;

#define MAX_VERTS_PER_STRAND 16
	float4 sharedStrandPos[MAX_VERTS_PER_STRAND];
	for ( uint localVertexIndex = 0; localVertexIndex < numVerticesInTheStrand; localVertexIndex++ )
	{
		globalVertexIndex = globalRootVertexIndex + localVertexIndex;
		sharedStrandPos[localVertexIndex] = g_HairVertexPositions[globalVertexIndex];
	}

	//--------------------------------------------
	// Local shape constraint for bending/twisting
	//--------------------------------------------
	for ( uint iterations = 0; iterations < (uint)g_NumLocalShapeMatchingIterations; iterations++ )
	{
		float4 pos = sharedStrandPos[1];
			float4 rotGlobal = g_GlobalRotations[globalRootVertexIndex];

			for ( uint localVertexIndex = 1; localVertexIndex < numVerticesInTheStrand - 1; localVertexIndex++ )
			{
				globalVertexIndex = globalRootVertexIndex + localVertexIndex;
				float4 pos_plus_one = sharedStrandPos[localVertexIndex + 1];

				//--------------------------------
				// Update position i and i_plus_1
				//--------------------------------
				float4 rotGlobalWorld;

				if ( g_bSingleHeadTransform )
					rotGlobalWorld = MultQuaternionAndQuaternion(g_ModelRotateForHead, rotGlobal);
				else
					rotGlobalWorld = MultQuaternionAndQuaternion(g_Transforms[globalStrandIndex].quat, rotGlobal);

				float3 orgPos_i_plus_1_InLocalFrame_i = g_HairRefVecsInLocalFrame[globalVertexIndex + 1].xyz;
				float3 orgPos_i_plus_1_InGlobalFrame = MultQuaternionAndVector(rotGlobalWorld, orgPos_i_plus_1_InLocalFrame_i) + pos.xyz;

				float3 del = stiffnessForLocalShapeMatching * (orgPos_i_plus_1_InGlobalFrame - pos_plus_one.xyz).xyz;

				if ( IsMovable(pos) )
					pos.xyz -= del.xyz;

				if ( IsMovable(pos_plus_one) )
					pos_plus_one.xyz += del.xyz;

				//---------------------------
				// Update local/global frames
				//---------------------------
				float4 invRotGlobalWorld = InverseQuaternion(rotGlobalWorld);
				float3 vec = normalize(pos_plus_one.xyz - pos.xyz);

				float3 x_i_plus_1_frame_i = normalize(MultQuaternionAndVector(invRotGlobalWorld, vec));
				float3 e = float3(1.0f, 0, 0);
				float3 rotAxis = cross(e, x_i_plus_1_frame_i);

				if ( length(rotAxis) > 0.001 )
				{
					float angle_radian = acos(dot(e, x_i_plus_1_frame_i));
					rotAxis = normalize(rotAxis);

					float4 localRot = MakeQuaternion(angle_radian, rotAxis);
					rotGlobal = MultQuaternionAndQuaternion(rotGlobal, localRot);
				}

				sharedStrandPos[localVertexIndex].xyz = pos.xyz;
				sharedStrandPos[localVertexIndex + 1].xyz = pos_plus_one.xyz;

				pos = pos_plus_one;
			}
	}

	for ( uint locVertIndex = 0; locVertIndex < numVerticesInTheStrand; locVertIndex++ )
	{
		globalVertexIndex = globalRootVertexIndex + locVertIndex;
		g_HairVertexPositions[globalVertexIndex] = sharedStrandPos[locVertIndex];
	}

	return;
}

//--------------------------------------------------------------------------------------
//
//  LengthConstraintsWindAndCollision
//
//  Compute shader to move the vertex position based on wind, maintain the lenght constraints
//  and handles collisions.
//
// One thread computes one vertex.
//
//--------------------------------------------------------------------------------------
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void LengthConstraintsWindAndCollision(uint GIndex : SV_GroupIndex,
									   uint3 GId : SV_GroupID,
									   uint3 DTid : SV_DispatchThreadID)
{
	uint globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType;
	CalcIndicesInVertexLevelMaster(GIndex, GId.x, globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType);

	uint numOfStrandsPerThreadGroup = g_NumOfStrandsPerThreadGroup;

	//------------------------------
	// Copy data into shared memory
	//------------------------------
	sharedPos[indexForSharedMem] = g_HairVertexPositions[globalVertexIndex];
	sharedLength[indexForSharedMem] = g_HairRestLengthSRV[globalVertexIndex];
	GroupMemoryBarrierWithGroupSync();

	//------------
	// Wind
	//------------
	if ( g_Wind.x != 0 || g_Wind.y != 0 || g_Wind.z != 0 )
	{
		float4 force = float4(0, 0, 0, 0);
		float frame = g_Wind.w;

		if ( localVertexIndex >= 2 && localVertexIndex < numVerticesInTheStrand - 1 )
		{
			// combining four winds.
			float a = ((float)(globalStrandIndex % 20)) / 20.0f;
			float3  w = a*g_Wind.xyz + (1.0f - a)*g_Wind1.xyz + a*g_Wind2.xyz + (1.0f - a)*g_Wind3.xyz;
			uint sharedIndex = localVertexIndex * numOfStrandsPerThreadGroup + localStrandIndex;

			float3 v = sharedPos[sharedIndex].xyz - sharedPos[sharedIndex + numOfStrandsPerThreadGroup].xyz;
			float3 force = -cross(cross(v, w), v);
			sharedPos[sharedIndex].xyz += force*g_TimeStep*g_TimeStep;
		}
	}

	GroupMemoryBarrierWithGroupSync();

	//----------------------------
	// Enforce length constraints
	//----------------------------
	uint a = floor(numVerticesInTheStrand / 2.0f);
	uint b = floor((numVerticesInTheStrand - 1) / 2.0f);

	for ( int iterationE = 0; iterationE < g_NumLengthConstraintIterations; iterationE++ )
	{
		uint sharedIndex = 2 * localVertexIndex * numOfStrandsPerThreadGroup + localStrandIndex;

		if ( localVertexIndex < a )
			ApplyDistanceConstraint(sharedPos[sharedIndex], sharedPos[sharedIndex + numOfStrandsPerThreadGroup], sharedLength[sharedIndex].x);

		GroupMemoryBarrierWithGroupSync();

		if ( localVertexIndex < b )
			ApplyDistanceConstraint(sharedPos[sharedIndex + numOfStrandsPerThreadGroup], sharedPos[sharedIndex + numOfStrandsPerThreadGroup * 2], sharedLength[sharedIndex + numOfStrandsPerThreadGroup].x);

		GroupMemoryBarrierWithGroupSync();
	}


	//-------------------------------------------------
	// Collision handling hard-code collision shapes
	//-------------------------------------------------
	bool bColDetected = false;
	bool bAnyColDetected = false;
	float4 oldPos = g_HairVertexPositionsPrev[globalVertexIndex];

		if ( g_bCollision > 0 )
		{
			float3 newPos;
			row_major float4x4 xf;

			if ( g_bSingleHeadTransform )
				xf = g_ModelTransformForHead;
			else
				xf = g_Transforms[globalStrandIndex].tfm;

			{
				float3 center1 = g_cc0_center1AndRadius.xyz;
				center1 = mul(float4(center1.xyz, 1), xf).xyz;
				float3 center2 = g_cc0_center2AndRadiusSquared.xyz;
				center2 = mul(float4(center2.xyz, 1), xf).xyz;

				CollisionCapsule cc;
				cc.p1.xyz = center1;
				cc.p1.w = g_cc0_center1AndRadius.w;
				cc.p2.xyz = center2;
				cc.p2.w = g_cc0_center2AndRadiusSquared.w;

				bool bColDetected = CapsuleCollision(sharedPos[indexForSharedMem], oldPos, newPos, cc, false);

				if ( bColDetected )
					sharedPos[indexForSharedMem].xyz = newPos;
				bAnyColDetected = bColDetected;
			}

		{
			float3 center1 = g_cc1_center1AndRadius.xyz;
			center1 = mul(float4(center1.xyz, 1), xf).xyz;
			float3 center2 = g_cc1_center2AndRadiusSquared.xyz;
			center2 = mul(float4(center2.xyz, 1), xf).xyz;

			CollisionCapsule cc;
			cc.p1.xyz = center1;
			cc.p1.w = g_cc1_center1AndRadius.w;
			cc.p2.xyz = center2;
			cc.p2.w = g_cc1_center2AndRadiusSquared.w;

			bColDetected = CapsuleCollision(sharedPos[indexForSharedMem], oldPos, newPos, cc, false);

			if ( bColDetected )
				sharedPos[indexForSharedMem].xyz = newPos;
			
			bAnyColDetected = bAnyColDetected || bColDetected;
		}

		{
			float3 center1 = g_cc2_center1AndRadius.xyz;
			center1 = mul(float4(center1.xyz, 1), xf).xyz;
			float3 center2 = g_cc2_center2AndRadiusSquared.xyz;
			center2 = mul(float4(center2.xyz, 1), xf).xyz;

			CollisionCapsule cc;
			cc.p1.xyz = center1;
			cc.p1.w = g_cc2_center1AndRadius.w;
			cc.p2.xyz = center2;
			cc.p2.w = g_cc2_center2AndRadiusSquared.w;

			bColDetected = CapsuleCollision(sharedPos[indexForSharedMem], oldPos, newPos, cc, false);

			if ( bColDetected )
				sharedPos[indexForSharedMem].xyz = newPos;

			bAnyColDetected = bAnyColDetected || bColDetected;
		}
		}

	GroupMemoryBarrierWithGroupSync();

	//---------------------------------------
	// update global position buffers
	//---------------------------------------
	g_HairVertexPositions[globalVertexIndex] = sharedPos[indexForSharedMem];

	if ( bAnyColDetected )
		g_HairVertexPositionsPrev[globalVertexIndex] = sharedPos[indexForSharedMem];

	return;
}

// One thread computes one total vertex (guide + follow)
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void ComputeTangents(uint GIndex : SV_GroupIndex,
					 uint3 GId : SV_GroupID,
					 uint3 DTid : SV_DispatchThreadID)
{
	uint globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType;
	CalcIndicesInVertexLevelTotal(GIndex, GId.x, globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType);

	sharedPos[indexForSharedMem] = g_HairVertexPositions[globalVertexIndex];
	GroupMemoryBarrierWithGroupSync();

	uint numOfStrandsPerThreadGroup = g_NumOfStrandsPerThreadGroup;

	if ( localVertexIndex == 0 ) // vertex 0
	{
		float3 tangent = sharedPos[indexForSharedMem + numOfStrandsPerThreadGroup].xyz - sharedPos[indexForSharedMem].xyz;
		g_HairVertexTangents[globalVertexIndex].xyz = normalize(tangent);
	}
	else // vertex 1 through n-1
	{
		float3 vert_i_minus_1 = sharedPos[indexForSharedMem - numOfStrandsPerThreadGroup].xyz;
		float3 vert_i = sharedPos[indexForSharedMem].xyz;
		g_HairVertexTangents[globalVertexIndex].xyz = normalize(vert_i - vert_i_minus_1);
	}

	/*if ( localVertexIndex < numVerticesInTheStrand - 1 )
	{
		float3 tangent = sharedPos[indexForSharedMem + numOfStrandsPerThreadGroup].xyz - sharedPos[indexForSharedMem].xyz;
		g_HairVertexTangents[globalVertexIndex].xyz = normalize(tangent);
	}*/
}

// One thread computes one vertex.
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void UpdateFollowHairVertices(uint GIndex : SV_GroupIndex,
							  uint3 GId : SV_GroupID,
							  uint3 DTid : SV_DispatchThreadID)
{
	uint globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType;
	CalcIndicesInVertexLevelMaster(GIndex, GId.x, globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType);

	sharedPos[indexForSharedMem] = g_HairVertexPositions[globalVertexIndex];
	GroupMemoryBarrierWithGroupSync();

	for ( uint i = 0; i < g_NumFollowHairsPerGuideHair; i++ )
	{
		int globalFollowVertexIndex = globalVertexIndex + numVerticesInTheStrand * (i + 1);
		int globalFollowStrandIndex = globalStrandIndex + i + 1;
		float factor = g_TipSeparationFactor*((float)localVertexIndex / (float)numVerticesInTheStrand) + 1.0f;
		float3 followPos = sharedPos[indexForSharedMem].xyz + factor*g_FollowHairRootOffset[globalFollowStrandIndex].xyz;
		g_HairVertexPositions[globalFollowVertexIndex].xyz = followPos;
	}

	return;
}

// One thread computes one vertex.
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void PrepareFollowHairBeforeTurningIntoGuide(uint GIndex : SV_GroupIndex,
											 uint3 GId : SV_GroupID,
											 uint3 DTid : SV_DispatchThreadID)
{
	uint globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType;
	CalcIndicesInVertexLevelMaster(GIndex, GId.x, globalStrandIndex, localStrandIndex, globalVertexIndex, localVertexIndex, numVerticesInTheStrand, indexForSharedMem, strandType);

	sharedPos[indexForSharedMem] = g_HairVertexPositions[globalVertexIndex];
	GroupMemoryBarrierWithGroupSync();

	for ( uint i = 0; i < g_NumFollowHairsPerGuideHair; i++ )
	{
		int globalFollowVertexIndex = globalVertexIndex + numVerticesInTheStrand * (i + 1);
		g_HairVertexPositionsPrev[globalFollowVertexIndex].xyz = g_HairVertexPositions[globalFollowVertexIndex].xyz;
	}

	return;
}

//--------------------------------------------------------------------------------------
//
//  GenerateTransforms
//
//  Compute shader to create the transformations for each strand.
//  Used for fur skinning.
//
// One thread computes one strand.
//
//--------------------------------------------------------------------------------------
[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void GenerateTransforms(uint GIndex : SV_GroupIndex,
						uint3 GId : SV_GroupID,
						uint3 DTid : SV_DispatchThreadID)
{
	uint local_id, group_id, globalStrandIndex, numVerticesInTheStrand, globalRootVertexIndex, strandType;
	CalcIndicesInStrandLevelMaster(GIndex, GId.x, globalStrandIndex, numVerticesInTheStrand, globalRootVertexIndex, strandType);

	// get the index for the mesh triangle
	uint triangleIndex = g_HairToMeshMapping[globalStrandIndex].triangleIndex * 3;

	// get the barycentric coordinate for this hair strand
	float a = g_HairToMeshMapping[globalStrandIndex].barycentricCoord[0];
	float b = g_HairToMeshMapping[globalStrandIndex].barycentricCoord[1];
	float c = g_HairToMeshMapping[globalStrandIndex].barycentricCoord[2];

	// get the un-transformed triangle
	float3 vert1 = g_MeshVertices[triangleIndex].xyz;
	float3 vert2 = g_MeshVertices[triangleIndex + 1].xyz;
	float3 vert3 = g_MeshVertices[triangleIndex + 2].xyz;

	// get the transfomed (skinned) triangle
	float3 skinnedVert1 = g_TransformedVerts[triangleIndex].xyz;
	float3 skinnedVert2 = g_TransformedVerts[triangleIndex + 1].xyz;
	float3 skinnedVert3 = g_TransformedVerts[triangleIndex + 2].xyz;

	// calculate original hair position for the strand using the barycentric coordinate
	//float3 pos = mul(a, vert1) + mul (b, vert2) + mul(c, vert3);

	// calculate the new hair position for the strand using the barycentric coordinate
	float3 tfmPos = mul(a, skinnedVert1) + mul(b, skinnedVert2) + mul(c, skinnedVert3);
	float3 initialPos = g_InitialHairPositions[globalRootVertexIndex].xyz;

	//-------------------------------------------------
	// Calculate transformation matrix for the hair
	//-------------------------------------------------

	// create a coordinate system from the untransformed triangle
	// Note: this part only needs to be done once. We could pre-calculate it
	// for every hair and save it in a buffer.
	float3 normal;
	float3 tangent = normalize(vert1 - vert3);
	float3 tangent2 = vert2 - vert3;
	normal = normalize(cross(tangent, tangent2));
	float3 binormal = normalize(cross(normal, tangent));

	row_major float4x4  triangleMtx;
	triangleMtx._m00 = tangent.x;   triangleMtx._m01 = tangent.y;   triangleMtx._m02 = tangent.z;   triangleMtx._m03 = 0;
	triangleMtx._m10 = normal.x;    triangleMtx._m11 = normal.y;    triangleMtx._m12 = normal.z;    triangleMtx._m13 = 0;
	triangleMtx._m20 = binormal.x;  triangleMtx._m21 = binormal.y;  triangleMtx._m22 = binormal.z;  triangleMtx._m23 = 0;
	triangleMtx._m30 = 0;           triangleMtx._m31 = 0;           triangleMtx._m32 = 0;           triangleMtx._m33 = 1;

	// create a coordinate system from the transformed triangle
	tangent = normalize(skinnedVert1 - skinnedVert3);
	tangent2 = skinnedVert2 - skinnedVert3;
	normal = normalize(cross(tangent, tangent2));
	binormal = normalize(cross(normal, tangent));

	row_major float4x4  tfmTriangleMtx;
	tfmTriangleMtx._m00 = tangent.x;   tfmTriangleMtx._m01 = tangent.y;   tfmTriangleMtx._m02 = tangent.z;   tfmTriangleMtx._m03 = 0;
	tfmTriangleMtx._m10 = normal.x;    tfmTriangleMtx._m11 = normal.y;    tfmTriangleMtx._m12 = normal.z;    tfmTriangleMtx._m13 = 0;
	tfmTriangleMtx._m20 = binormal.x;  tfmTriangleMtx._m21 = binormal.y;  tfmTriangleMtx._m22 = binormal.z;  tfmTriangleMtx._m23 = 0;
	tfmTriangleMtx._m30 = 0;           tfmTriangleMtx._m31 = 0;           tfmTriangleMtx._m32 = 0;           tfmTriangleMtx._m33 = 1;

	// Find the rotation transformation from the untransformed triangle to the transformed triangle
	// rotation = inverse(triangleMtx) x tfmTriangleMtx = transpose(triangleMtx) x tfmTriangleMtx, since triangelMtx is orthonormal
	row_major float4x4  rotationMtx = mul(transpose(triangleMtx), tfmTriangleMtx);

	// translation matrix from hair to origin since we want to rotate the hair at it's root
	row_major float4x4  translationMtx;
	translationMtx._m00 = 1;                translationMtx._m01 = 0;                translationMtx._m02 = 0;                translationMtx._m03 = 0;
	translationMtx._m10 = 0;                translationMtx._m11 = 1;                translationMtx._m12 = 0;                translationMtx._m13 = 0;
	translationMtx._m20 = 0;                translationMtx._m21 = 0;                translationMtx._m22 = 1;                translationMtx._m23 = 0;
	translationMtx._m30 = -initialPos.x;    translationMtx._m31 = -initialPos.y;    translationMtx._m32 = -initialPos.z;    translationMtx._m33 = 1;

	// final rotation matrix
	rotationMtx = mul(translationMtx, rotationMtx);

	// translate back to the final position (as determined by the skinned mesh position)
	translationMtx._m30 = tfmPos.x;    translationMtx._m31 = tfmPos.y;    translationMtx._m32 = tfmPos.z;    translationMtx._m33 = 1;

	// combine the rotation and translation
	row_major float4x4  tfmMtx = mul(rotationMtx, translationMtx);

	// apply the global transformation for the model
	//tfmMtx = mul(tfmMtx, g_ModelTransformForHead);

	// calculate the quaternion from the matrix
	float4 quaternion;
	quaternion.w = sqrt(1 + tfmMtx._m00 + tfmMtx._m11 + tfmMtx._m22) / 2;
	quaternion.x = (tfmMtx._m21 - tfmMtx._m12) / (4 * quaternion.w);
	quaternion.y = (tfmMtx._m02 - tfmMtx._m20) / (4 * quaternion.w);
	quaternion.z = (tfmMtx._m10 - tfmMtx._m01) / (4 * quaternion.w);

	g_Transforms[globalStrandIndex].tfm = tfmMtx;
	g_Transforms[globalStrandIndex].quat = quaternion;
	return;
}
