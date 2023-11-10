#pragma once

struct UIVertex
{
	float x;
	float y;
	float u;
	float v;

	static UIVertex Make(float x, float y, float u, float v)
	{
		UIVertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.u = u;
		vertex.v = v;
		return vertex;
	}
};
