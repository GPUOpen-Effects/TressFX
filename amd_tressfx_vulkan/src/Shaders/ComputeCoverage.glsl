//--------------------------------------------------------------------------------------
// ComputeCoverage
//
// Calculate the pixel coverage of a hair strand by computing the hair width
//--------------------------------------------------------------------------------------
float ComputeCoverage(vec2 p0, vec2 p1, vec2 pixelLoc)
{
	// p0, p1, pixelLoc are in d3d clip space (-1 to 1)x(-1 to 1)

	// Scale positions so 1.f = half pixel width
	p0 *= g_WinSize.xy;
	p1 *= g_WinSize.xy;
	pixelLoc.y *= -1;
	pixelLoc *= g_WinSize.xy;

	float p0dist = length(p0 - pixelLoc);
	float p1dist = length(p1 - pixelLoc);
	float hairWidth = length(p0 - p1);

	// will be 1.f if pixel outside hair, 0.f if pixel inside hair
	bool outside = any(bvec2(step(hairWidth, p0dist) > 0., step(hairWidth, p1dist) > 0.));

	// if outside, set sign to -1, else set sign to 1
	float sign = outside ? -1.f : 1.f;

	// signed distance (positive if inside hair, negative if outside hair)
	float relDist = sign * clamp(min(p0dist, p1dist), 0., 1.);

	// returns coverage based on the relative distance
	// 0, if completely outside hair edge
	// 1, if completely inside hair edge
	return (relDist + 1.f) * 0.5f;
}