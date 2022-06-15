#pragma once

#include <vector>
#include "linalg.h"
# include <math.h>

using namespace linalg::aliases;
using std::vector;

#define LEFT_DOWN float3{-0.6204, 0.4020, 0.4737} 
#define LEFT_UP float3{-0.4703, 0.4010, 0.7115}
#define RIGHT_DOWN float3{0.5724,0.4168,0.4759}


vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard);
vector<float3> getLine(float3 baseBoard, vector<float3> basis, float2 pos1_2D, float2 pos2_2D, int numOfPoints, bool drawing);
float3 translateToBoardCoordinates(float3 v);
vector<float3> getCircArc(float3 baseBoard, vector<float3> basis, float2 center, float rad, float alpha0, float angle, int numOfPoints);
