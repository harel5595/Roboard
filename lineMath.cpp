#include "lineMath.h"


vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard)
{
	float3 relX = XBoard - baseBoard;
	float3 relY = YBoard - baseBoard;
	float3 e1 = (1 / length(relX)) * relX;
	float3 e2 = relY - (dot(relY, e1) * e1);
	e2 *= 1 - length(e2);
	float3 e3 = cross(e1, e2);
	vector<float3> basis;
	basis.push_back(e1);
	basis.push_back(e2);
	basis.push_back(e3);
	return basis;


}


vector<float3> getLine(float3 baseBoard, vector<float3> basis, float2 pos1_2D, float2 pos2_2D, int numOfPoints)
{
	float3 e1 = basis[0];
	float3 e2 = basis[1];
	
	vector<float3> positions;
	float3 pos1 = (pos1_2D.x * e1) + (pos1_2D.y * e2);
	float3 pos2 = (pos2_2D.x * e1) + (pos2_2D.y * e2);
	float3 delta = pos2 - pos1;
	for (float t = 0; t <= 1; t += (1.0/numOfPoints))
	{
		positions.push_back(baseBoard + pos1 + (t * delta));
	}
	return positions;
	
}