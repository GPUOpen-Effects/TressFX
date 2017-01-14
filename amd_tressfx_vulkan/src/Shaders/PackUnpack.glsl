//--------------------------------------------------------------------------------------
// Helper functions for packing and unpacking the stored tangent and coverage
//--------------------------------------------------------------------------------------
uint PackFloat4IntoUint(vec4 vValue)
{
	uint byte3 = uint(vValue.x * 255.) & 0xFF;
	uint byte2 = uint(vValue.y * 255.) & 0xFF;
	uint byte1 = uint(vValue.z * 255) & 0xFF;
	uint byte0 = uint(vValue.w * 255) & 0xFF;
	return (byte3 << 24) | (byte2 << 16) | (byte1 << 8) | byte0;
}

vec4 UnpackUintIntoFloat4(uint uValue)
{
	return vec4(((uValue & 0xFF000000) >> 24) / 255.0, ((uValue & 0x00FF0000) >> 16) / 255.0, ((uValue & 0x0000FF00) >> 8) / 255.0, ((uValue & 0x000000FF)) / 255.0);
}

uint PackTangentAndCoverage(vec3 tangent, float coverage)
{
	return PackFloat4IntoUint(vec4(tangent.xyz*0.5 + 0.5, coverage));
}

vec3 GetTangent(uint packedTangent)
{
	return 2.0 * UnpackUintIntoFloat4(packedTangent).xyz - 1.0;
}

float GetCoverage(uint packedCoverage)
{
	return UnpackUintIntoFloat4(packedCoverage).w;
}
