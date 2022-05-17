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