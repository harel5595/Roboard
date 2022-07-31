#pragma once

#include <vector>
#include "linalg.h"
# include <math.h>

using namespace linalg::aliases;
using std::vector;

#define LEFT_DOWN float3{-0.6268, 0.3743, 0.3625} 
#define LEFT_UP float3{-0.4504, 0.3692, 0.7236}
#define RIGHT_DOWN float3{0.5564,0.4046,0.3792}

float3 getNormal();


vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard);
vector<float3> getLine(vector<float3> basis, float2 pos1_2D, float2 pos2_2D, int numOfPoints, bool drawing);
float3 translateToBoardCoordinates(float3 v);
vector<float3> getCircArc(vector<float3> basis, float2 center, float rad, float alpha0, float angle, int numOfPoints);
