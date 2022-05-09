#pragma once

#include <vector>
#include "linalg.h"
# include <math.h>

using namespace linalg::aliases;
using std::vector;

#define LEFT_DOWN float3{-0.6494, 0.29, 0.4030} 
#define LEFT_UP float3{-0.3403, 0.29, 0.8360}
#define RIGHT_DOWN float3{0.6434,0.29,0.4074}


vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard);
vector<float3> getLine(float3 baseBoard, vector<float3> basis, float2 pos1_2D, float2 pos2_2D, int numOfPoints);