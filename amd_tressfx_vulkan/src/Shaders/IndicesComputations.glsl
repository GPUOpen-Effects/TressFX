layout(set = 1, binding = IDSRV_HAIR_STRAND_TYPE) uniform usamplerBuffer g_HairStrandType;

void CalcIndicesInVertexLevelMaster(uint local_id, uint group_id, inout uint globalStrandIndex, inout uint localStrandIndex, inout int globalVertexIndex, inout uint localVertexIndex, inout uint numVerticesInTheStrand, inout uint indexForSharedMem, inout uint strandType)
{
	indexForSharedMem = local_id;
	numVerticesInTheStrand = (THREAD_GROUP_SIZE / g_NumOfStrandsPerThreadGroup);

	localStrandIndex = local_id % g_NumOfStrandsPerThreadGroup;
	globalStrandIndex = group_id * g_NumOfStrandsPerThreadGroup + localStrandIndex;
	globalStrandIndex *= (g_NumFollowHairsPerGuideHair + 1);
	localVertexIndex = (local_id - localStrandIndex) / g_NumOfStrandsPerThreadGroup;

	strandType = texelFetch(g_HairStrandType, int(globalStrandIndex)).x;
	globalVertexIndex = int(globalStrandIndex * numVerticesInTheStrand + localVertexIndex);
}

void CalcIndicesInVertexLevelTotal(uint local_id, uint group_id, inout uint globalStrandIndex, inout uint localStrandIndex, inout int globalVertexIndex, inout uint localVertexIndex, inout uint numVerticesInTheStrand, inout uint indexForSharedMem, inout uint strandType)
{
	indexForSharedMem = local_id;
	numVerticesInTheStrand = (THREAD_GROUP_SIZE / g_NumOfStrandsPerThreadGroup);

	localStrandIndex = local_id % g_NumOfStrandsPerThreadGroup;
	globalStrandIndex = group_id * g_NumOfStrandsPerThreadGroup + localStrandIndex;
	localVertexIndex = (local_id - localStrandIndex) / g_NumOfStrandsPerThreadGroup;

	strandType = texelFetch(g_HairStrandType, int(globalStrandIndex)).x;
	globalVertexIndex = int(globalStrandIndex * numVerticesInTheStrand + localVertexIndex);
}

void CalcIndicesInStrandLevelTotal(uint local_id, uint group_id, inout uint globalStrandIndex, inout uint numVerticesInTheStrand, inout uint globalRootVertexIndex, inout uint strandType)
{
	globalStrandIndex = THREAD_GROUP_SIZE*group_id + local_id;
	numVerticesInTheStrand = (THREAD_GROUP_SIZE / g_NumOfStrandsPerThreadGroup);
	strandType = texelFetch(g_HairStrandType, int(globalStrandIndex)).x;
	globalRootVertexIndex = globalStrandIndex * numVerticesInTheStrand;
}

void CalcIndicesInStrandLevelMaster(int local_id, int group_id, inout int globalStrandIndex, inout uint numVerticesInTheStrand, inout int globalRootVertexIndex, inout uint strandType)
{
	globalStrandIndex = THREAD_GROUP_SIZE * group_id + local_id;
	globalStrandIndex *= int(g_NumFollowHairsPerGuideHair + 1);
	numVerticesInTheStrand = (THREAD_GROUP_SIZE / g_NumOfStrandsPerThreadGroup);
	strandType = texelFetch(g_HairStrandType, globalStrandIndex).x;
	globalRootVertexIndex = globalStrandIndex * int(numVerticesInTheStrand);
}
