layout(set = 0, binding = IDSRV_HAIRSM) uniform texture2D HairShadowMap;
layout(set = 0, binding = IDSRV_SHADOW_SAMPLER) uniform sampler g_samPointClamp;

//--------------------------------------------------------------------------------------
// ComputeShadow
//
// Computes the shadow using a simplified deep shadow map technique for the hair and
// PCF for scene objects. It uses multiple taps to filter over a (KERNEL_SIZE x KERNEL_SIZE)
// kernel for high quality results.
//--------------------------------------------------------------------------------------
float ComputeShadow(vec3 worldPos, float alpha)
{
	vec4 projPosLight = g_mViewProjLight * vec4(worldPos, 1);
	projPosLight.xy /= projPosLight.w;
	vec2 texSM = .5 * projPosLight.xy + .5;
	texSM.y = 1. - texSM.y;
	float depth = projPosLight.z / projPosLight.w;
	float epsilon = depth * SM_EPSILON;
	float depth_fragment = projPosLight.w;

	// for shadow casted by scene objs, use PCF shadow
	float total_weight = 0;
	float amountLight_hair = 0;

	total_weight = 0;
	for (int dx = (1 - KERNEL_SIZE) / 2; dx <= KERNEL_SIZE / 2; dx++)
	{
		for (int dy = (1 - KERNEL_SIZE) / 2; dy <= KERNEL_SIZE / 2; dy++)
		{
			float size = 2.4;
			float sigma = (KERNEL_SIZE / 2.0) / size; // standard deviation, when kernel/2 > 3*sigma, it's close to zero, here we use 1.5 instead
			float exp = -1 * (dx * dx + dy * dy) / (2 * sigma * sigma);
			float weight = 1 / (2 * PI * sigma * sigma) * pow(2.7, exp);

			// shadow casted by hair: simplified deep shadow map
			float depthSMHair = 2. * texture(sampler2D(HairShadowMap, g_samPointClamp), texSM + vec2(dx, dy) / 640.).x - 1.; //z/w

			float depth_smPoint = g_fNearLight / (1 - depthSMHair * (g_fFarLight - g_fNearLight) / g_fFarLight);

			float depth_range = max(0., depth_fragment - depth_smPoint);
			float numFibers = depth_range / (g_FiberSpacing * g_FiberRadius);

			// if occluded by hair, there is at least one fiber
			numFibers += (depth_range > .00001) ? 1. : 0.;
			amountLight_hair += pow(abs(1 - alpha), numFibers) * weight;

			total_weight += weight;
		}
	}
	amountLight_hair /= total_weight;

	float amountLight_scene = 1.;//g_txSMScene.SampleCmpLevelZero(g_samShadow, texSM, depth-epsilon);

	return (amountLight_hair * amountLight_scene);
}


//--------------------------------------------------------------------------------------
// ComputeSimpleShadow
//
// Computes the shadow using a simplified deep shadow map technique for the hair and
// PCF for scene objects. This function only uses one sample, so it is faster but
// not as good quality as ComputeShadow
//--------------------------------------------------------------------------------------
float ComputeSimpleShadow(vec3 worldPos, float alpha)
{
	vec4 projPosLight = g_mViewProjLight * vec4(worldPos, 1);
	projPosLight.xy /= projPosLight.w;

	vec2 texSM = .5 * projPosLight.xy + .5;
	texSM.y = 1. - texSM.y;
	float depth = projPosLight.z / projPosLight.w;
	float epsilon = depth * SM_EPSILON;
	float depth_fragment = projPosLight.w;

	// shadow casted by scene
	float amountLight_scene = 1.;//g_txSMScene.SampleCmpLevelZero(g_samShadow, texSM, depth-epsilon); // TODO

								 // shadow casted by hair: simplified deep shadow map
	float depthSMHair = 2. * texture(sampler2D(HairShadowMap, g_samPointClamp), texSM).x - 1.; //z/w

	float depth_smPoint = g_fNearLight / (1. - depthSMHair * (g_fFarLight - g_fNearLight) / g_fFarLight);

	float depth_range = max(0., depth_fragment - depth_smPoint);

	float numFibers = depth_range / (g_FiberSpacing * g_FiberRadius);

	// if occluded by hair, there is at least one fiber
	numFibers += (depth_range > .00001) ? 1. : 0.;
	float amountLight_hair = pow(abs(1 - alpha), numFibers);

	return amountLight_hair;
}
