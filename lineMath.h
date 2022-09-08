#pragma once

#include <vector>
#include "linalg.h"
# include <math.h>

using namespace linalg::aliases;
using std::vector;

#define LEFT_DOWN float3{-0.5111, 0.3219, 0.4641} 
#define LEFT_UP float3{-0.3843, 0.3131, 0.7940} 
#define RIGHT_DOWN float3{0.6694,0.2565,0.4842}

float3 getNormal();
void setNormal(float3 normal);
float3 getBaseBoard();
void updateBasis(float3 baseBoard, float3 e1, float3 e2, float3 e3);
vector<float3> getNewBasis(float3 baseBoard, float3 XBoard, float3 YBoard);

vector<float3> getLine(float2 pos1_2D, float2 pos2_2D, int numOfPoints, bool drawing);
float3 translateToBoardCoordinates(float3 v);
vector<float3> getCircArc(float2 center, float rad, float alpha0, float angle, int numOfPoints);
vector<float3> getQuadBezierCurve(float2 p1, float2 p2, float2 p3, int numOfPoints);
vector<float3> getCubicBezierCurve(float2 p1, float2 p2, float2 p3, float2 p4, int numOfPoints);
