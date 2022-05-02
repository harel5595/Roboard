#pragma once

#include <vector>
#include "linalg.h"
# include <math.h>

using namespace linalg::aliases;
using std::vector;

#define LEFT_DOWN float3{-0.6585, 0.3004, 0.3251} 
#define LEFT_UP float3{-0.3641, 0.3077, 0.8541}
#define RIGHT_DOWN float3{0.4821, 0.3267, 0.3163}


vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard);
vector<float3> getLine(float3 baseBoard, vector<float3> basis, float2 pos1_2D, float2 pos2_2D, int numOfPoints);