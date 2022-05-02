#pragma once

#include <vector>
#include "linalg.h"
# include <math.h>

using namespace linalg::aliases;
using std::vector;

#define LEFT_DOWN float3{-0.6718, 0.3037, 0.3238} 
#define LEFT_UP float3{-0.4123, 0.3087, 0.8306}
#define RIGHT_DOWN float3{0.5576,0.3358,0.3265}


vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard);
vector<float3> getLine(float3 baseBoard, vector<float3> basis, float2 pos1_2D, float2 pos2_2D, int numOfPoints);