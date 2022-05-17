#include "linalg.h"

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


//int(*MySendBasicTrajectory)(TrajectoryPoint command);

#ifdef __linux__ 
void* commandLayer_handle;
#elif _WIN32
HINSTANCE commandLayer_handle;
#endif

//Function pointers to the functions we need
int(*MyInitAPI)();
int(*MyCloseAPI)();
int(*MySendBasicTrajectory)(TrajectoryPoint command);
int(*MyStartForceControl)();
int(*MyGetDevices)(KinovaDevice devices[MAX_KINOVA_DEVICE], int& result);
int(*MySetActiveDevice)(KinovaDevice device);
int(*MyMoveHome)();
int(*MyInitFingers)();
int(*MyGetCartesianCommand)(CartesianPosition&);
int(*MyGetAngularCommand)(AngularPosition&);
int(*MyGetCartesianPosition)(CartesianPosition&);
int(*MyGetAngularPosition)(AngularPosition&);
int(*MyStopForceControl)();

// additional functions for force control
int(*MyGetCartesianForce)(CartesianPosition&);
int(*MySwitchTrajectoryTorque)(GENERALCONTROL_TYPE type);
int(*MySetTorqueControlType)(TORQUECONTROL_TYPE type);
int(*MySetTorqueSafetyFactor)(float factor);
int(*MySetTorqueVibrationController)(float value);
int(*MySendAngularTorqueCommand)(float Command[COMMAND_SIZE]);
int(*MySendCartesianForceCommand)(float Command[COMMAND_SIZE]);
TrajectoryPoint* CartesianToPoint(float x, float y, float z);

TrajectoryPoint* CartesianToPoint(float3 point)
{
	return CartesianToPoint(point.x, point.y, point.z);
}


/*
* This func create TrajectoryPoint from Cartesian values.
* REMEMBER to free the pointer after use.
*/
TrajectoryPoint * CartesianToPoint(float x, float y, float z)
{
	TrajectoryPoint * start = new TrajectoryPoint;
	start->InitStruct();

	UserPosition  point = UserPosition();
	point.InitStruct();
	point.Type = CARTESIAN_POSITION;
	CartesianInfo pos = CartesianInfo();
	pos.X = x;
	pos.Y = y;
	pos.Z = z;
	 
	pos.ThetaX = -1.57;
	pos.ThetaY = -0.1145;
	pos.ThetaZ = -2.9938;
	point.Fingers.Finger1 = 6486;
	point.Fingers.Finger2 = 6312;
	point.Fingers.Finger3 = 6312;



	point.CartesianPosition = pos;
	start->Position = point;
	return start;
}


/*
* This func create TrajectoryPoint from Cartesian values.
* REMEMBER to free the pointer after use.
*/
TrajectoryPoint* CartesianToPoint(float x, float y, float z, float thetaX, float thetaY, float thetaZ)
{
	TrajectoryPoint* start = new TrajectoryPoint;
	start->InitStruct();

	UserPosition  point = UserPosition();
	point.InitStruct();
	point.Type = CARTESIAN_POSITION;
	CartesianInfo pos = CartesianInfo();
	pos.X = x;
	pos.Y = y;
	pos.Z = z;
	pos.ThetaX = thetaX;
	pos.ThetaY = thetaY;
	pos.ThetaZ = thetaZ;
	point.Fingers.Finger1 = 6486;
	point.Fingers.Finger2 = 6312;
	point.Fingers.Finger3 = 6312;



	point.CartesianPosition = pos;
	start->Position = point;
	return start;
}

float3 PointToCartesian(CartesianPosition point)
{
	return float3{ point.Coordinates.X,point.Coordinates.Y,point.Coordinates.Z };
}


/*
* This func create TrajectoryPoint from Cartesian values.
* finger3 is the finger that alone in the hand.
* REMEMBER to free the pointer after use.
*/
TrajectoryPoint* CartesianToPoint(float x, float y, float z, float thetaX, float thetaY, float thetaZ, float finger1, float finger2, float finger3)
{
	TrajectoryPoint* start = new TrajectoryPoint;
	start->InitStruct();

	UserPosition  point = UserPosition();
	point.InitStruct();
	point.Type = CARTESIAN_POSITION;
	CartesianInfo pos = CartesianInfo();
	pos.X = x;
	pos.Y = y;
	pos.Z = z;
	pos.ThetaX = thetaX;
	pos.ThetaY = thetaY;
	pos.ThetaZ = thetaZ;
	point.Fingers.Finger1 = finger1;
	point.Fingers.Finger2 = finger2;
	point.Fingers.Finger3 = finger3;

	point.CartesianPosition = pos;
	start->Position = point;
	return start;
}


int initRobotAPI()
{
	int result;
	commandLayer_handle = LoadLibrary(L"CommandLayerWindows.dll");

	MyInitAPI = (int(*)()) GetProcAddress(commandLayer_handle, "InitAPI");
	MyCloseAPI = (int(*)()) GetProcAddress(commandLayer_handle, "CloseAPI");
	MyGetCartesianForce = (int(*)(CartesianPosition&)) GetProcAddress(commandLayer_handle, "GetCartesianForce");
	MyMoveHome = (int(*)()) GetProcAddress(commandLayer_handle, "MoveHome");
	MyInitFingers = (int(*)()) GetProcAddress(commandLayer_handle, "InitFingers");
	MyStartForceControl = (int(*)()) GetProcAddress(commandLayer_handle, "StartForceControl");
	MyGetDevices = (int(*)(KinovaDevice devices[MAX_KINOVA_DEVICE], int& result)) GetProcAddress(commandLayer_handle, "GetDevices");
	MySetActiveDevice = (int(*)(KinovaDevice devices)) GetProcAddress(commandLayer_handle, "SetActiveDevice");
	MyGetCartesianCommand = (int(*)(CartesianPosition&)) GetProcAddress(commandLayer_handle, "GetCartesianCommand");
	MyGetAngularCommand = (int(*)(AngularPosition&)) GetProcAddress(commandLayer_handle, "GetAngularCommand");
	MyGetCartesianPosition = (int(*)(CartesianPosition&)) GetProcAddress(commandLayer_handle, "GetCartesianPosition");
	MyGetAngularPosition = (int(*)(AngularPosition&)) GetProcAddress(commandLayer_handle, "GetAngularPosition");
	MyStopForceControl = (int(*)()) GetProcAddress(commandLayer_handle, "StopForceControl");
	MySendBasicTrajectory = (int(*)(TrajectoryPoint)) GetProcAddress(commandLayer_handle, "SendBasicTrajectory");

	// additional functions for force control
	MySwitchTrajectoryTorque = (int(*)(GENERALCONTROL_TYPE)) GetProcAddress(commandLayer_handle, "SwitchTrajectoryTorque");
	MySetTorqueSafetyFactor = (int(*)(float)) GetProcAddress(commandLayer_handle, "SetTorqueSafetyFactor");
	MySetTorqueVibrationController = (int(*)(float)) GetProcAddress(commandLayer_handle, "SetTorqueVibrationController");
	MySendAngularTorqueCommand = (int(*)(float Command[COMMAND_SIZE])) GetProcAddress(commandLayer_handle, "SendAngularTorqueCommand");
	MySendCartesianForceCommand = (int(*)(float Command[COMMAND_SIZE])) GetProcAddress(commandLayer_handle, "SendCartesianForceCommand");
	MySetTorqueControlType = (int(*)(TORQUECONTROL_TYPE)) GetProcAddress(commandLayer_handle, "SetTorqueControlType");


	//If the API was loaded correctly
	if ((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyGetDevices == NULL)
		|| (MySetActiveDevice == NULL) || (MyStartForceControl == NULL))
	{
		cout << "* * *  E R R O R   D U R I N G   I N I T I A L I Z A T I O N  * * *" << endl;
		return 0;
	}
	else
	{
		if (MySwitchTrajectoryTorque == NULL)
			cout << "* * * ERROR LOADING MySwitchTrajectoryTorque * * * " << endl;

		cout << "I N I T I A L I Z A T I O N   C O M P L E T E D" << endl << endl;

		result = (*MyInitAPI)();

		cout << "Initialization's result :" << result << endl;

		KinovaDevice list[MAX_KINOVA_DEVICE];


		int devicesCount = MyGetDevices(list, result);

		for (int i = 0; i < devicesCount; i++)
		{
			cout << "Found a robot on the USB bus (" << list[i].SerialNumber << ")" << endl;

			//Setting the current device as the active device.
			MySetActiveDevice(list[i]);

			// MyStartForceControl();

			// cout << endl << "The robot's admittance control is now active, you can move it freely with your hand." <<
			// 	" To deactivate it call the function StopForceControl()." << endl;
			// collect a few points here

		}
	}
	return 1;
}

void waitUntilGetToPoint(float3 wanted_pos)
{
	CartesianPosition cur_point;
	float3 priv = float3{ 0,0,0 };
	int counter = 0;
	cout << "dst: " << wanted_pos << endl;
	while (true)
	{
		counter++;
		//cout << float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } << endl;
		Sleep(2);
		MyGetCartesianPosition(cur_point);
		if(length(float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } - wanted_pos) < 0.008 || (float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } == priv && length(float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } - wanted_pos) < 0.01))
			return;
		priv = float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z };
		if (counter == 100) {
			counter = 0;
			cout << "dst: " << wanted_pos << ", pos: " << float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } << endl;
			TrajectoryPoint* pos = CartesianToPoint(wanted_pos);
			MySendBasicTrajectory(*pos);
			free(pos);
		}
	}
	//CartesianPosition pos = MyGetCartesianForce();
}

float3 nextPointByForce(float3 force, float3 nextPoint, float3 nowForce, float3 normalBoard)
{
	
	float3 force_diff = force - nowForce; //difference between what we want and what we have
	if (length(nowForce) > MAX_FORCE)
	{
		perror("too much force");
		exit(1);
	}
	float3 force_corr = dot(force_diff, normalBoard) * normalBoard * float3 { 0.00, 0.04, 0.00 }; //force correction, how much to move in the direction of the normal.
	cout <<"curr force:     " << length(nowForce) << "      curr_correction:   " << force_corr << endl;
	return nextPoint + force_corr;
}

void mainLoopForDrawLine(vector<float3> line, float3 normalBoard)
{
	cout << normalBoard << " - the normal" << endl;
	bool finishDraw = false;
	cout << line[0] << endl << line[1] << endl;
	TrajectoryPoint* pos = CartesianToPoint(line[0]);
	MySendBasicTrajectory(*pos);
	free(pos);
	waitUntilGetToPoint(line[0]);

	float3 wanted_force = float3{0.5,1.5,-0.7 };
	for (auto point : line)
	{
		CartesianPosition force;
		MyGetCartesianPosition(force);
		cout << "my pos: " << translateToBoardCoordinates(PointToCartesian(force)) << endl;
		point = nextPointByForce(wanted_force, point, float3{ force.Coordinates.X,force.Coordinates.Y,force.Coordinates.Z }, normalBoard);
		TrajectoryPoint* pos = CartesianToPoint(point);
		MySendBasicTrajectory(*pos);
		free(pos);
		Sleep(10);
		//waitUntilGetToPoint(point);
	}
}


void disconnectFromRobot()
{
	MyCloseAPI();
}





int main(void)
{
	int programResult, result;

    if (!initRobotAPI())
		return 1;

	MyMoveHome();
	//MySetTorqueSafetyFactor(15);
	//MyInitFingers();
	Sleep(2000);
	vector<float3> basis = getNewBasis(LEFT_DOWN, RIGHT_DOWN, LEFT_UP);

	vector<float3> line = getLine(LEFT_DOWN, basis, float2{ 1.2, 0.5 }, float2{ 1.2,1.0 }, 10); // call eyal func
	mainLoopForDrawLine(line, basis[2]);


	line = getLine(LEFT_DOWN, basis, float2{ 1.2, 1.0 }, float2{ 1.0,1.0 }, 10); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.0, 1.0 }, float2{ 1.2,0.5 }, 10); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.2, 0.5 }, float2{ 1.0,0.5 }, 10); // call eyal func
	mainLoopForDrawLine(line, basis[2]);



	//line = getLine(LEFT_DOWN, basis, float2{ 1.0, 0.0 }, float2{ 2.0,2.0 }, 3000); // call eyal func
	//mainLoopForDrawLine(line, basis[2]);
	
	//vector<float3> line = getLine(LEFT_DOWN, RIGHT_DOWN, LEFT_UP, float2{ 1, 0.0 }, float2{ 1,0.4 }); // call eyal func
	//mainLoopForDrawLine(line);

	//line = getLine(LEFT_DOWN, RIGHT_DOWN, LEFT_UP, float2{ 1, 0.4 }, float2{ 0,0.4 }); // call eyal func
	//mainLoopForDrawLine(line);

	disconnectFromRobot();

}