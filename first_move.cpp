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
#include "alphabet.h"
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
int(*MyEraseAllTrajectories)();

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
TrajectoryPoint* CartesianToPoint(float3 point, float3 direction)
{
	return CartesianToPoint(point.x, point.y, point.z, direction.x, direction.y, direction.z);
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
	MyEraseAllTrajectories = (int(*)()) GetProcAddress(commandLayer_handle, "EraseAllTrajectories");
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

// returns a velocity vector from b to a with length 1
TrajectoryPoint operator- (const TrajectoryPoint& a, const TrajectoryPoint& b)
{
	TrajectoryPoint differenceTrajectory;
	float len, len2;
	float pi = 3.1415f;

	differenceTrajectory.Position.CartesianPosition.X = (a.Position.CartesianPosition.X - b.Position.CartesianPosition.X);
	differenceTrajectory.Position.CartesianPosition.Y = (a.Position.CartesianPosition.Y - b.Position.CartesianPosition.Y);
	differenceTrajectory.Position.CartesianPosition.Z = (a.Position.CartesianPosition.Z - b.Position.CartesianPosition.Z);
	len2 = pow(differenceTrajectory.Position.CartesianPosition.X, 2) +
		pow(differenceTrajectory.Position.CartesianPosition.Y, 2) +
		pow(differenceTrajectory.Position.CartesianPosition.Z, 2);
	len = pow(len2, 0.5f);
	differenceTrajectory.Position.CartesianPosition.X /= len;
	differenceTrajectory.Position.CartesianPosition.Y /= len;
	differenceTrajectory.Position.CartesianPosition.Z /= len;

	differenceTrajectory.Position.CartesianPosition.ThetaX = (a.Position.CartesianPosition.ThetaX - b.Position.CartesianPosition.ThetaX);
	differenceTrajectory.Position.CartesianPosition.ThetaY = (a.Position.CartesianPosition.ThetaY - b.Position.CartesianPosition.ThetaY);
	differenceTrajectory.Position.CartesianPosition.ThetaZ = (a.Position.CartesianPosition.ThetaZ - b.Position.CartesianPosition.ThetaZ);

	if (differenceTrajectory.Position.CartesianPosition.ThetaX > pi)
		differenceTrajectory.Position.CartesianPosition.ThetaX -= 2 * pi;
	else if (differenceTrajectory.Position.CartesianPosition.ThetaX < -pi)
		differenceTrajectory.Position.CartesianPosition.ThetaX += 2 * pi;
	if (differenceTrajectory.Position.CartesianPosition.ThetaY > pi)
		differenceTrajectory.Position.CartesianPosition.ThetaY -= 2 * pi;
	else if (differenceTrajectory.Position.CartesianPosition.ThetaY < -pi)
		differenceTrajectory.Position.CartesianPosition.ThetaY += 2 * pi;
	if (differenceTrajectory.Position.CartesianPosition.ThetaZ > pi)
		differenceTrajectory.Position.CartesianPosition.ThetaZ -= 2 * pi;
	else if (differenceTrajectory.Position.CartesianPosition.ThetaZ < -pi)
		differenceTrajectory.Position.CartesianPosition.ThetaZ += 2 * pi;

	differenceTrajectory.Position.Fingers.Finger1 = (a.Position.Fingers.Finger1 - b.Position.Fingers.Finger1);
	differenceTrajectory.Position.Fingers.Finger2 = (a.Position.Fingers.Finger2 - b.Position.Fingers.Finger2);
	differenceTrajectory.Position.Fingers.Finger3 = (a.Position.Fingers.Finger3 - b.Position.Fingers.Finger3);

	return differenceTrajectory;
}

std::ostream& operator<< (std::ostream& os, const TrajectoryPoint& p)
{
	// write obj to stream
	os << std::setprecision(2) << p.Position.CartesianPosition.X << " " << p.Position.CartesianPosition.Y << " " << p.Position.CartesianPosition.Z << " "
		<< p.Position.CartesianPosition.ThetaX << " " << p.Position.CartesianPosition.ThetaY << " " << p.Position.CartesianPosition.ThetaZ;
	return os;
}


void PushP2P(TrajectoryPoint fromPoint, TrajectoryPoint toPoint, float x_speed, float y_force)
{
	TrajectoryPoint velocityToSend, currentPoint;
	CartesianPosition dataPosition;
	float pi = 3.14f;
	float epsilon = 0.03f;
	float y_speed, tmp_y_speed, init_y_speed;
	float actual_force;
	CartesianPosition data;

	velocityToSend.InitStruct();

	// go to fromPoint - use cartesian poition control
	cout << "going to start position" << endl;
	fromPoint.Position.Type = CARTESIAN_POSITION;
	MySendBasicTrajectory(fromPoint);

	// go to toPoint - use velocity control (later read torque to mediate force)
	cout << "Starting Velocity Control - going to target point (with pressure)" << endl;
	velocityToSend.Position.Type = CARTESIAN_VELOCITY;

	// initiate y_speed based on y_force desired direction, and arbitrarily 0.01 of x_speed. Then update it based on actual force
	init_y_speed = 0.01 * abs(x_speed) * y_force / abs(y_force);
	y_speed = init_y_speed;
	tmp_y_speed = y_speed;

	// implement force application by velocity control
	// send the velocity vector every 5 ms as long as we want the robot to move along that vector.
	for (int i = 0; i < 500; i++)
	{
		(*MyGetCartesianPosition)(dataPosition);

		float x;
		x = currentPoint.Position.CartesianPosition.X = dataPosition.Coordinates.X;
		currentPoint.Position.CartesianPosition.Y = dataPosition.Coordinates.Y;
		currentPoint.Position.CartesianPosition.Z = dataPosition.Coordinates.Z;
		currentPoint.Position.CartesianPosition.ThetaX = dataPosition.Coordinates.ThetaX;
		currentPoint.Position.CartesianPosition.ThetaY = dataPosition.Coordinates.ThetaY;
		currentPoint.Position.CartesianPosition.ThetaZ = dataPosition.Coordinates.ThetaZ;
		currentPoint.Position.Fingers.Finger1 = dataPosition.Fingers.Finger1;
		currentPoint.Position.Fingers.Finger2 = dataPosition.Fingers.Finger2;
		currentPoint.Position.Fingers.Finger3 = dataPosition.Fingers.Finger3;

		if (abs(toPoint.Position.CartesianPosition.X - dataPosition.Coordinates.X) < epsilon &&
			abs(toPoint.Position.CartesianPosition.Y - dataPosition.Coordinates.Y) < epsilon &&
			abs(toPoint.Position.CartesianPosition.Z - dataPosition.Coordinates.Z) < epsilon)
			break;

		// get normalized direction vector (only X, Y, Z are normalized, not angles or fingers)
		velocityToSend = toPoint - currentPoint;

		// fix orientation of the end effoctor
		velocityToSend.Position.CartesianPosition.ThetaX *= 1.0f;
		velocityToSend.Position.CartesianPosition.ThetaY *= -1.0f;  // BUG??? when not negating, the arm rotates all over
		velocityToSend.Position.CartesianPosition.ThetaZ *= 1.0f;

		velocityToSend.Position.CartesianPosition.X = x_speed;
		// TODO: calculate speed based on read torque
		velocityToSend.Position.CartesianPosition.Y = tmp_y_speed; // push against the board
		velocityToSend.Position.CartesianPosition.Z /= 5.0;

		velocityToSend.Position.Type = CARTESIAN_VELOCITY;

		// cout << "current: " << currentPoint << " target: " << toPoint << " velocity: " << velocityToSend << endl;
		MySendBasicTrajectory(velocityToSend);

		// calibrate y speed (against the board) based on current pressure
		int result = (*MyGetCartesianForce)(data);
		actual_force = data.Coordinates.Y;
		if (actual_force / y_force > 10)
		{
			// in case pressure is too hard already, free the pressure
			tmp_y_speed = -1 * init_y_speed;
			y_speed = init_y_speed;
		}
		else
		{
			y_speed *= y_force / actual_force;
			tmp_y_speed = y_speed;
		}

		cout << "Forces: " << "X: " << data.Coordinates.X << " N, " << "Y: " << data.Coordinates.Y << " N, (tmp speed: " << tmp_y_speed << "), " << "Z: " << data.Coordinates.Z << " N, " << endl;

		Sleep(5);  // robot will persevere 5 ms in speed control
	}

	// stay at target point, but change robot back to position control (in case it matters)
	currentPoint.Position.Type = CARTESIAN_POSITION;
	MySendBasicTrajectory(currentPoint);
}

void MoveToPoint(float3 point)
{
	TrajectoryPoint* start = CartesianToPoint(point);
	MySendBasicTrajectory(*start);
	//free(start);
}

float3 GetNewDstByForce(float3 oldDst, float3 force)
{
	cout << force << length(force) << endl;
	if (length(force) < 5)
	{
		return oldDst;
	}
	return oldDst +(float3)(getNormal() * 0.0005);
}

float3 GetMyForce()
{
	float3 force;
	CartesianPosition curr_force;
	MyGetCartesianForce(curr_force);
	force = PointToCartesian(curr_force);
	return force;
}

void GetToPointWithForceControl(float3 dst)
{
	CartesianPosition curr_point;
	float3 force, start_force = GetMyForce();
	int counter = 0;
	while (counter < 100) // need to sert maximum of moving time to be 2 seconds
	{
		counter++;
		MyGetCartesianPosition(curr_point);
		if (length(PointToCartesian(curr_point) - dst) < 0.01)// got to point
		{
			MyEraseAllTrajectories();
			return; // stop the moveing
		}

		force = GetMyForce();
		dst = GetNewDstByForce(dst, force - start_force); // change dst by the position of the board
		
		MoveToPoint(dst);
		Sleep(5);
		MyEraseAllTrajectories();
	}
}






void waitUntilGetToPoint(float3 wanted_pos)
{
	CartesianPosition cur_point;
	float3 priv = float3{ 0,0,0 };
	int counter = 0;
	cout << "dst: " << wanted_pos << endl;
	while (true)
	{
		//cout << float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } << endl;
		
		MyGetCartesianPosition(cur_point);
		if(length(float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } - wanted_pos) < 0.008 || (float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } == priv && length(float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } - wanted_pos) < 0.01))
			return; // Got to point
		priv = float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z };
		//cout << "dst: " << wanted_pos << ", pos: " << float3{ cur_point.Coordinates.X, cur_point.Coordinates.Y, cur_point.Coordinates.Z } << endl;
		TrajectoryPoint* pos = CartesianToPoint(wanted_pos);
		MySendBasicTrajectory(*pos);
		Sleep(2);
		MyEraseAllTrajectories();
		free(pos);
		
	}
	//CartesianPosition pos = MyGetCartesianForce();
}

float3 nextPointByForce(float3 nextPoint, float3 normalBoard)
{	
	int MEASURE = 5;

	float3 forceSum;
	CartesianPosition force;
	for (int i = 0; i < MEASURE; i++) {
		MyGetCartesianForce(force);
		forceSum += PointToCartesian(force);
		Sleep(1);
	}
	forceSum /= MEASURE;
	
	if (forceSum.y > 10)
	{
		nextPoint.y -= 0.01;
	}
	else if (forceSum.y < 5)
	{
		nextPoint.y += 0.01;
	}

	return nextPoint;
	
	//float3 force_diff = force - nowForce; //difference between what we want and what we have
	//if (length(nowForce) > MAX_FORCE)
	//{
	//	perror("too much force");
	//	exit(1);
	//}
	//float3 force_corr = dot(force_diff, normalBoard) * normalBoard * float3 { 0.00, 0.04, 0.00 }; //force correction, how much to move in the direction of the normal.
	//cout <<"curr force:     " << length(nowForce) << "      curr_correction:   " << force_corr << endl;
	//return nextPoint + force_corr;
}


float3 GoToTheBoard(float3 start_point)
{
	float3 start_force = GetMyForce(), curr_point = start_point, my_force;
	CartesianPosition curr_pos;
	float diff;
	int counter = 0;
	while (true)
	{
		my_force = GetMyForce();
		//my_force.z = 0;
		//start_force.z = 0;
		diff = length(start_force - my_force);
		cout << "force diff: " << diff << endl;
		if (diff > 1.5 && counter > 20)
		{
			cout << "found the board!" << curr_point << endl;
			return curr_point;
		}

		MoveToPoint(curr_point);
		Sleep(4);
		MyEraseAllTrajectories();
		MyGetCartesianPosition(curr_pos);
		if (length(PointToCartesian(curr_pos) - curr_point) < 0.01) // got to the point, but not the board yet
		{
			counter++;
			curr_point -= (float3)(getNormal() * 0.0002);
			start_force = GetMyForce();
			cout << "other point!" << curr_point << endl;
		}
	}
}



void mainLoopForDrawLine(vector<float3> line, float3 normalBoard, bool drawing)
{
	//cout << normalBoard << " - the normal" << endl;
	bool finishDraw = false;
	//cout << line[0] << endl << line[1] << endl;
	TrajectoryPoint* pos = CartesianToPoint(line[0]);
	MySendBasicTrajectory(*pos);
	free(pos);
	//waitUntilGetToPoint(line[0]);

	float3 wanted_force = float3{0.5,1.5,-0.7 };
	TrajectoryPoint* priv = NULL;
	for (auto point : line)
	{
		if (priv == NULL)
		{
			priv = CartesianToPoint(point);
			continue;
		}
		//cout << "3d: " << point << endl;
		if(drawing)
			GetToPointWithForceControl(point);
		else
		{
			priv = pos;
			TrajectoryPoint* pos = CartesianToPoint(point);
			MySendBasicTrajectory(*pos);
		}
		//cout << "my pos: " << translateToBoardCoordinates(PointToCartesian(force)) << ", " << PointToCartesian(force) << endl;
		//point = nextPointByForce(wanted_force, point, float3{ force.Coordinates.X,force.Coordinates.Y,force.Coordinates.Z }, normalBoard);
		//TrajectoryPoint* pos = CartesianToPoint(point);//, float3{-1.5,-0.33,-2.99});
		//PushP2P(*priv, *pos, 0.15f, 3.0f);
		//priv = pos;
		//TrajectoryPoint* pos = CartesianToPoint(point);
		//MySendBasicTrajectory(*pos);
		//free(priv);
		//Sleep(2);
		//waitUntilGetToPoint(point);
	}
}


void disconnectFromRobot()
{
	MyCloseAPI();
}


void drawRoad(vector<float2> road, vector<float3> basis)
{
	float2 priv = float2{ 0,0 };
	for (auto curr : road)
	{
		if (priv == float2{ 0,0 })
		{
			priv = curr;
			continue;
		}

		vector<float3> line = getLine(LEFT_DOWN, basis, priv, curr, 10, true);
		//mainLoopForDrawLine(line, basis[2]);
	}
}


void drawHI()
{
	vector<float3> basis = getNewBasis(LEFT_DOWN, RIGHT_DOWN, LEFT_UP);
	vector<float2> road = drawCapH(float2{ 0.8, 0.2 });
	drawRoad(road, basis);
	road = drawCapI(float2{ 1.2, 0.2 });
	drawRoad(road, basis);
}


// L:cord_x,cord_y:cord_x,cord_y:T/F

void drawFile(string fileName)
{
	vector<float3> basis = getNewBasis(LEFT_DOWN, RIGHT_DOWN, LEFT_UP);
	ifstream MyReadFile(fileName);
	string line;
	string delimiter = ":";
	vector<string> line_splited;
	// Use a while loop together with the getline() function to read the file line by line
	while (getline(MyReadFile, line)) {
		// Output the text from the file
		line_splited = vector<string>();
		size_t pos = 0;
		std::string token;
		while ((pos = line.find(delimiter)) != std::string::npos) {
			token = line.substr(0, pos);
			//std::cout << token << std::endl;
			line_splited.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
		 
		
		if (line_splited[0] ==  "L") // need to draw a line
		{
			float2 first_point = float2{std::stof(line_splited[1]), std::stof(line_splited[2])};
			float2 second_point = float2{std::stof(line_splited[3]), std::stof(line_splited[4])};
			bool drawing = line_splited[5] == "T";
			mainLoopForDrawLine(getLine(LEFT_DOWN, basis, first_point, second_point, 100, drawing), basis[2], drawing);
			//cout << "first point:" << first_point << " , second:" << second_point << endl;
		}
		else if (line_splited[0] == "C")
		{
			float2 center = float2{std::stof(line_splited[1]), std::stof(line_splited[2])};
			float radios = std::stof(line_splited[3]);
			float start_angle = std::stof(line_splited[4]);
			float draw_angle = std::stof(line_splited[5]);
			mainLoopForDrawLine(getCircArc(LEFT_DOWN, basis, center, radios, start_angle, draw_angle, 100), basis[2], 1);
		}
	}
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
	//drawHI();

	///return 0;
	vector<float3> basis = getNewBasis(LEFT_DOWN, RIGHT_DOWN, LEFT_UP);
	//cout << "globs: " << basis[0] << ", " << basis[1] << ", " << basis[2] << endl;
	TrajectoryPoint* start = CartesianToPoint(0.4514, 0.2646, 0.6031);
	MySendBasicTrajectory(*start);
	Sleep(7000);
	GoToTheBoard(float3{ 0.4514, 0.2646, 0.6031 });
	
	
	/*TrajectoryPoint* start = CartesianToPoint(0.4514, 0.2646, 0.5031);
	MySendBasicTrajectory(*start);
	Sleep(7000);
	*///GoToTheBoard(float3{ 0.4514, 0.2646, 0.5031 });

	
	
	//GetToPointWithForceControl(float3{ 0.4514, 0.2646, 0.5031 });
	
	
	
	
	//Sleep(5000);
	//MyEraseAllTrajectories();
	//free(start);
	//start = CartesianToPoint(-0.4911, 0.1204, 0.5203,-1.7394,-0.0947,-3.1058);
	//MySendBasicTrajectory(*start);
	//free(start);

	
	//drawFile(string("C:\\Users\\Administrator\\EyalHarelJonathan\\Hello2.txt"));
	
	
	//vector<float3> line = getLine(LEFT_DOWN, basis, float2{ 0.8, 0.2 }, float2{ 1.1, 0.2}, 10, true);
	//mainLoopForDrawLine(line, basis[2]);
	/*
	line = getLine(LEFT_DOWN, basis, float2{ 1.0, 0.3 }, float2{ 1.0,0.2 }, 10, true); // |
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.0, 0.3 }, float2{ 1.0,0.25 }, 10, false); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.0, 0.25 }, float2{ 1.05,0.25 }, 30, true); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.05, 0.25 }, float2{ 1.05,0.3 }, 10, false); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.05, 0.3 }, float2{ 1.05,0.2 }, 10, true); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	*/
	
	/*
	vector<float3> line =  getCircArc(LEFT_DOWN, basis, float2{ 1, 0.2 }, 0.1, 0, 6.28, 100);//getLine(LEFT_DOWN, basis, float2{ 1.2, 0.5 }, float2{ 1.2,1.0 }, 10); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.1,0.2 }, float2{ 1,0.3 }, 5, false);
	mainLoopForDrawLine(line, basis[2]);
	line = getCircArc(LEFT_DOWN, basis, float2{ 0.9, 0.3 }, 0.1, 0, 6.28, 100);//getLine(LEFT_DOWN, basis, float2{ 1.2, 0.5 }, float2{ 1.2,1.0 }, 10); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	*/
	//line = getCircArc(LEFT_DOWN, basis, float2{ 1.066, 0.2 }, 0.03, 0, 6.28, 6);//getLine(LEFT_DOWN, basis, float2{ 1.2, 0.5 }, float2{ 1.2,1.0 }, 10); // call eyal func
	//mainLoopForDrawLine(line, basis[2]);
	/*
	vector<float3> line = getLine(LEFT_DOWN, basis, float2{ 1.1, 0.3 }, float2{ 1.0,0.3 }, 10, true); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.0, 0.3 }, float2{ 1.0,0.2 }, 10, true); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.0, 0.2 }, float2{ 1.1,0.2 }, 10, true); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	line = getLine(LEFT_DOWN, basis, float2{ 1.1, 0.2 }, float2{ 1.1,0.3 }, 10, true); // call eyal func
	mainLoopForDrawLine(line, basis[2]);
	*/

	//line = getLine(LEFT_DOWN, basis, float2{ 1.0, 0.0 }, float2{ 2.0,2.0 }, 3000); // call eyal func
	//mainLoopForDrawLine(line, basis[2]);
	
	//vector<float3> line = getLine(LEFT_DOWN, RIGHT_DOWN, LEFT_UP, float2{ 1, 0.0 }, float2{ 1,0.4 }); // call eyal func
	//mainLoopForDrawLine(line);

	//line = getLine(LEFT_DOWN, RIGHT_DOWN, LEFT_UP, float2{ 1, 0.4 }, float2{ 0,0.4 }); // call eyal func
	//mainLoopForDrawLine(line);
	/*int i = 0;

	while (i < 150)
	{
		CartesianPosition force;
		MyGetCartesianForce(force);
		cout << PointToCartesian(force) << endl;
		Sleep(200);
		i++;
	}*/


	disconnectFromRobot();

}