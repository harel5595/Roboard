#include "lineMath.h"

float3 glob_e1;
float3 glob_e2;
float3 glob_e3;

vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard)
{
	float3 relX = XBoard - baseBoard;
	float3 relY = YBoard - baseBoard;
	float3 e1 = (1 / length(relX)) * relX;
	float3 e2 = relY - (dot(relY, e1) * e1);
	e2 /= length(e2);
	float3 e3 = cross(e1, e2);
	vector<float3> basis;
	basis.push_back(e1);
	basis.push_back(e2);
	basis.push_back(e3);
	glob_e1 = e1;
	glob_e2 = e2;
	glob_e3 = e3;
	return basis;

}


float3 translateToBoardCoordinates(float3 v)
{
	return float3{ dot(glob_e1, v), dot(glob_e2, v), dot(glob_e3, v) };
}

float3 translateToRealCoordinates(float2 v)
{
	return (glob_e1 * v.x) + (glob_e2 * v.y);
}

vector<float3> getLine(float3 baseBoard, vector<float3> basis, float2 pos1_2D, float2 pos2_2D, int numOfPoints, bool drawing)
{
	float3 e1 = basis[0];
	float3 e2 = basis[1];
	float3 base = baseBoard;
	if (!drawing)
		base += (float3)(0.03 * glob_e3);

	vector<float3> positions;
	float3 pos1 = (pos1_2D.x * e1) + (pos1_2D.y * e2);
	float3 pos2 = (pos2_2D.x * e1) + (pos2_2D.y * e2);
	float3 delta = pos2 - pos1;
	for (float t = 0; t <= 1.01; t += (1.0/numOfPoints))
	{
		positions.push_back(base + pos1 + (t * delta));
	}
	return positions;
	
}

vector<float3> getCircArc(float3 baseBoard, vector<float3> basis, float2 center, float rad, float alpha0, float angle, int numOfPoints)
{
	vector<float3> positions;
	
	for (float t = 0; t <= 1.01; t += (1.0 / numOfPoints))
	{
		float2 circ_offset = rad * float2{ cos(alpha0 + (t * angle)), sin(alpha0 + (t * angle)) };
		float2 board_point = center + circ_offset;
		positions.push_back(baseBoard + translateToRealCoordinates(board_point));
	}
	return positions;

}


vector<float3> getCurve(float3 baseBoard, float2(*gamma)(float), int numOfPoints, bool drawing)
{
	float3 base = drawing ? baseBoard : (float3)(baseBoard + (0.01 * glob_e3));
	vector<float3> positions;
	for (float t = 0; t <= 1; t += (1.0 / numOfPoints))
	{
		positions.push_back(base + translateToRealCoordinates(gamma(t)));
	}
	return positions;
}