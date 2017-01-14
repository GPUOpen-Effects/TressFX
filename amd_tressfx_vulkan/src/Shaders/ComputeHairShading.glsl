
vec3 ComputeHairShading(vec3 iPos, vec3 iTangent, vec4 iTex, float amountLight, vec3 color)
{
	vec3 baseColor = g_MatBaseColor.xyz;
	float rand_value = 1;

	//    if(abs(iTex.x) + abs(iTex.y) >1e-5) // if texcoord is available, use texture map
	//        rand_value = g_txNoise.SampleLevel(g_samLinearWrap, iTex.xy, 0).x;

	// define baseColor and Ka Kd Ks coefficient for hair
	float Ka = g_MatKValue.x, Kd = g_MatKValue.y,
		Ks1 = g_MatKValue.z, Ex1 = g_MatKValue.w,
		Ks2 = g_fHairKs2, Ex2 = g_fHairEx2;

	vec3 lightPos = g_PointLightPos.xyz;
	vec3 vLightDir = normalize(lightPos - iPos.xyz);
	vec3 vEyeDir = normalize(g_vEye.xyz - iPos.xyz);
	vec3 tangent = normalize(iTangent);

	// in Kajiya's model: diffuse component: sin(t, l)
	float cosTL = (dot(tangent, vLightDir));
	float sinTL = sqrt(1 - cosTL*cosTL);
	float diffuse = sinTL; // here sinTL is apparently larger than 0

	float alpha = (rand_value * 10) * PI / 180; // tiled angle (5-10 dgree)

												// in Kajiya's model: specular component: cos(t, rl) * cos(t, e) + sin(t, rl)sin(t, e)
	float cosTRL = -cosTL;
	float sinTRL = sinTL;
	float cosTE = (dot(tangent, vEyeDir));
	float sinTE = sqrt(1 - cosTE*cosTE);

	// primary highlight: reflected direction shift towards root (2 * Alpha)
	float cosTRL_root = cosTRL * cos(2 * alpha) - sinTRL * sin(2 * alpha);
	float sinTRL_root = sqrt(1 - cosTRL_root * cosTRL_root);
	float specular_root = max(0., cosTRL_root * cosTE + sinTRL_root * sinTE);

	// secondary highlight: reflected direction shifted toward tip (3*Alpha)
	float cosTRL_tip = cosTRL*cos(-3 * alpha) - sinTRL*sin(-3 * alpha);
	float sinTRL_tip = sqrt(1 - cosTRL_tip * cosTRL_tip);
	float specular_tip = max(0., cosTRL_tip * cosTE + sinTRL_tip * sinTE);

	vec3 vColor = Ka * g_AmbientLightColor.xyz * baseColor + // ambient
		amountLight * g_PointLightColor.xyz * (
			Kd * diffuse * baseColor + // diffuse
			Ks1 * pow(specular_root, Ex1) + // primary hightlight r
			Ks2 * pow(specular_tip, Ex2) * baseColor); // secondary highlight rtr

	return vColor * amountLight;
}

#define SUPERSIMPLESHADING

vec3 SimpleHairShading(vec3 iPos, vec3 iTangent, vec4 iTex, float amountLight)
{
	vec3 baseColor = g_MatBaseColor.xyz;
	float Kd = g_MatKValue.y;

#ifdef SUPERSIMPLESHADING
	vec3 vColor = amountLight * Kd * baseColor;
#else
	// define baseColor and Ka Kd Ks coefficient for hair
	float Ka = g_MatKValue.x;
	float Ks1 = g_MatKValue.z;
	float Ex1 = g_MatKValue.w;
	float Ks2 = g_fHairKs2;
	float Ex2 = g_fHairEx2;

	vec3 lightPos = g_PointLightPos.xyz;
	vec3 vLightDir = normalize(lightPos - iPos.xyz);
	vec3 tangent = normalize(iTangent);

	// in Kajiya's model: diffuse component: sin(t, l)
	float cosTL = (dot(tangent, vLightDir));
	float sinTL = sqrt(1 - cosTL*cosTL);
	float diffuse = sinTL; // here sinTL is apparently larger than 0

	vec3 vColor = Ka * g_AmbientLightColor.xyz * baseColor +                          // ambient
		amountLight * g_PointLightColor.xyz * (Kd * diffuse * baseColor); // diffuse
#endif
	return vColor * amountLight;
}

