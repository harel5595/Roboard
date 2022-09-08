#pragma once
#include "linalg.h"
#include <fstream>
#include <iostream>
#include "KinovaTypes.h"
//#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#include <dlfcn.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include "Kinova.API.CommLayerUbuntu.h"
#include "Kinova.API.UsbCommandLayerUbuntu.h"
#elif _WIN32
#include <time.h>
#include <fstream>
#include <Windows.h>
#include "CommunicationLayer.h"
#include "CommandLayer.h"
#include <conio.h>
#include <iomanip>
#include "lineMath.h"
#endif

#define MAX_FORCE 17

using namespace linalg::aliases;
using namespace linalg::ostream_overloads;
using namespace std;


#ifdef __linux__
void* commandLayer_handle;
#elif _WIN32
HINSTANCE commandLayer_handle;
#endif


TrajectoryPoint* Float3ToCartesian(float x, float y, float z);
TrajectoryPoint* Float3ToCartesian(float3 point);
TrajectoryPoint* Float3ToCartesian(float x, float y, float z, float thetaX, float thetaY, float thetaZ);
float3 CartesianToFloat3(CartesianPosition point);
TrajectoryPoint* Float3ToCartesian(float3 point, float3 direction);
TrajectoryPoint* Float3ToCartesian(float x, float y, float z, float thetaX, float thetaY, float thetaZ, float finger1, float finger2, float finger3);

int initRobotAPI(int vebrose);
void disconnectFromRobot();

void MoveToPoint(float3 point);
float3 GetMyForce();
float3 GetMyCommand();
float3 GetMyPosition();
vector<float> GetMyAngularForce();

float3 GoToTheBoard(float3 start_point, vector<int> effectedMotors, float pushStrength, float waitInterval, int vebrose);
vector <float3> findTheBoard(vector<float3> startingPoints, vector<int> effectedMotors, float pushStrength, float bigWaitInterval, float waitInterval, int vebrose);

void saveBasis(vector <float3> basis, string path, int vebrose);
void loadBasis(vector <float3> basis, string path, int vebrose);

void mainLoopForDrawLine(vector <float3> line, float waitInterval, float endWaitInterval, int vebrose);
void drawFile(string fileName, float waitInterval, float bigWaitInterval, int pointsInLine, int vebrose);
