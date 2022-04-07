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
#endif

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
	//pos.ThetaX = -1.7;
	//pos.ThetaY = 0.021;
	//pos.ThetaZ = -2.58;
	//point.Fingers.Finger1 = 4896;
	//point.Fingers.Finger2 = 4896;
	//point.Fingers.Finger3 = 4962;



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
	//point.Fingers.Finger1 = 4896;
	//point.Fingers.Finger2 = 4896;
	//point.Fingers.Finger3 = 4962;



	point.CartesianPosition = pos;
	start->Position = point;
	return start;
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

void waitUntilGetToPoint(float x, float y, float z)
{
	//CartesianPosition pos = MyGetCartesianForce();
}


void mainLoopForDraw()
{
	bool finishDraw = false;
	while (!finishDraw)
	{
		// call eyal func

		// pass params to jonathan func

		// use the result to move the robot 

		// wait until the robot arives
	}
}


void disconnectFromRobot()
{
	CloseAPI();
}




int main(void)
{
	int programResult, result;

	if (!initRobotAPI())
		return 1;

	MyMoveHome();
	MyInitFingers();
	

	mainLoopForDraw();

	disconnectFromRobot();

	/*
	TrajectoryPoint start;
	start.InitStruct();
	
	UserPosition point = UserPosition();
	point.InitStruct();
	point.Type = CARTESIAN_POSITION;
	CartesianInfo pos;
	pos.X = 0.54;
	pos.Y = 0.2815;
	pos.Z = 0.56;
	pos.ThetaX = -1.7;
	pos.ThetaY = 0.021;
	pos.ThetaZ = -2.58;
	point.Fingers.Finger1 = 4896;
	point.Fingers.Finger2 = 4896;
	point.Fingers.Finger3 = 4962;



	point.CartesianPosition = pos;
	start.Position = point;

	TrajectoryPoint end;
	end.InitStruct();

	UserPosition end_pos = UserPosition();
	end_pos.InitStruct();
	end_pos.Type = CARTESIAN_POSITION;
	CartesianInfo end_pos_info;
	end_pos_info.X = 0.3879;
	end_pos_info.Y = 0.2815;
	end_pos_info.Z = 0.559;
	end_pos_info.ThetaX = -1.7;
	end_pos_info.ThetaY = -0.1215;
	end_pos_info.ThetaZ = -2.593;
	end_pos.Fingers.InitStruct();
	end_pos.Fingers.Finger1 = 4896;
	end_pos.Fingers.Finger2 = 4896;
	end_pos.Fingers.Finger3 = 4962;


	end_pos.CartesianPosition = end_pos_info;
	end.Position = end_pos;

	//MyInitFingers();


	//We specify that this point will use Cartesian position
	//pointToSend.Position.Type = CARTESIAN_POSITION;
	//Sleep(10000);
	TrajectoryPoint close_fingers;
	close_fingers.InitStruct();

	UserPosition close_fingers_pos = UserPosition();
	close_fingers_pos.InitStruct();
	close_fingers_pos.Type = CARTESIAN_POSITION;
	CartesianInfo close_fingers_pos_info;
	close_fingers_pos_info.X = 0.212;
	close_fingers_pos_info.Y = -0.256;
	close_fingers_pos_info.Z = 0.5;
	close_fingers_pos_info.ThetaX = 1.65;
	close_fingers_pos_info.ThetaY = 1.11;
	close_fingers_pos_info.ThetaZ = 0.1217;
	close_fingers_pos.Fingers.InitStruct();
	close_fingers_pos.CartesianPosition = close_fingers_pos_info;
	close_fingers_pos.Fingers.Finger1 = 4896;
	close_fingers_pos.Fingers.Finger2 = 4896;
	close_fingers_pos.Fingers.Finger3 = 4962;
	
		
		
	//MySendBasicTrajectory(close_fingers);
	MySendBasicTrajectory(start);
	Sleep(10000);
	MySendBasicTrajectory(end);
	*/

}