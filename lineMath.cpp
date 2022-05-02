#include "lineMath.h"

float inner_prod(float3 v, float3 u)
{
	return (v.x * u.x) + (v.y * u.y) + (v.z * u.z);
}

float inverse_norm(float3 v)
{
	return 1 / sqrt(inner_prod(v, v));
}


vector<float3> getLine(float3 baseBoard, float3 XBoard, float3 YBoard, float2 pos1_2D, float2 pos2_2D)
{
	float3 relX = XBoard - baseBoard;
	float3 relY = YBoard - baseBoard;
	float3 e1 = inverse_norm(relX) * relX;
	float3 e2 = relY - (inner_prod(relY, e1) * e1);
	e2 *= inverse_norm(e2);
	vector<float3> positions;
	float3 pos1 = (pos1_2D.x * e1) + (pos1_2D.y * e2);
	float3 pos2 = (pos2_2D.x * e1) + (pos2_2D.y * e2);
	positions.push_back(baseBoard + pos1);
	positions.push_back(baseBoard + pos2);
	return positions;
	
}