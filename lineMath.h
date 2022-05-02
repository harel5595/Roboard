#pragma once

#include <vector>
#include "linalg.h"
# include <math.h>

using namespace linalg::aliases;
using std::vector;

#define LEFT_DOWN float3{-0.6002, 0.2777, 0.3616} 
#define LEFT_UP float3{-0.4748, 0.2777, 0.7154}
#define RIGHT_DOWN float3{0.5287, 0.2777, 0.3472}


vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard)
vector<float3> getLine(float3 baseBoard, vector<float3> basis, float2 pos1_2D, float2 pos2_2D, int numOfPoints)