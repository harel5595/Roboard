#include "linalg.h"

#include <iostream>
#include "KinovaTypes.h"
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


vector<float2>	drawCapH(float2 tRC);
vector<float2>	drawCapI(float2 tRC);

float2 form(float2 start, float X, float Y)
{
	return float2(start.x + X, start.y + Y);
}

vector<float2>	drawCapH(float2 tRC)
{
	//trc is top right corner, im lazy :) 
	vector<float2> result;
	result.push_back(tRC);
	result.push_back(form(tRC, 0, -0.2));
	result.push_back(form(tRC, 0, -0.1));
	result.push_back(form(tRC, 0.5, -0.1));
	result.push_back(form(tRC, 0.5,0));
	result.push_back(form(tRC, 0.5, -0.2));
	return result;
}


vector<float2>	drawCapI(float2 tRC)
{
	//trc is top right corner, im lazy :) 
	vector<float2> result;
	result.push_back(tRC);
	result.push_back(form(tRC, 0.5, 0));
	result.push_back(form(tRC, 0.25, 0));
	result.push_back(form(tRC, 0.25, -0.2));
	result.push_back(form(tRC, 0, -0.2));
	result.push_back(form(tRC, 0.5, -0.2));
	return result;
}