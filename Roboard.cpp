#include "Roboard.h"

//Function pointers to the functions we need
int (*MyInitAPI)();

int (*MyCloseAPI)();

int (*MySendBasicTrajectory)(TrajectoryPoint command);

int (*MyStartForceControl)();

int (*MyGetDevices)(KinovaDevice devices[MAX_KINOVA_DEVICE], int &result);

int (*MySetActiveDevice)(KinovaDevice device);

int (*MyMoveHome)();

int (*MyInitFingers)();

int (*MyGetCartesianCommand)(CartesianPosition &);

int (*MyGetAngularCommand)(AngularPosition &);

int (*MyGetCartesianPosition)(CartesianPosition &);

int (*MyGetAngularPosition)(AngularPosition &);

int (*MyGetAngularForce)(AngularPosition &);

int (*MyStopForceControl)();

int (*MyEraseAllTrajectories)();

// additional functions for force control
int (*MyGetCartesianForce)(CartesianPosition &);

int (*MySwitchTrajectoryTorque)(GENERALCONTROL_TYPE type);

int (*MySetTorqueControlType)(TORQUECONTROL_TYPE type);

int (*MySetTorqueSafetyFactor)(float factor);

int (*MySetTorqueVibrationController)(float value);

int (*MySendAngularTorqueCommand)(float Command[COMMAND_SIZE]);

int (*MySendCartesianForceCommand)(float Command[COMMAND_SIZE]);

float3 defaultTheta = float3{ -1.4648 ,-0.1016,-2.8996 };
float3 defaultFingers = float3{ 6986,  6872 , 7112 };


/*
* Translate a float3 point into TrajectoryPoint pointer.
* NEED to free in the end!
* There will be default values for the theta and the fingers!!
*/
TrajectoryPoint *Float3ToCartesian(float3 point) {
    return Float3ToCartesian(point.x, point.y, point.z);
}



/*
* This func create TrajectoryPoint from Cartesian values.
* IMPORTENT: if used without insertin theta and fingers values then default values will be used.
* REMEMBER to free the pointer after use.
*/
TrajectoryPoint *Float3ToCartesian(float x, float y, float z) {
    TrajectoryPoint *start = new TrajectoryPoint;
    start->InitStruct();

    UserPosition point = UserPosition();
    point.InitStruct();
    point.Type = CARTESIAN_POSITION;
    CartesianInfo pos = CartesianInfo();
    pos.X = x;
    pos.Y = y;
    pos.Z = z;

    pos.ThetaX = defaultTheta.x;
    pos.ThetaY = defaultTheta.y;
    pos.ThetaZ = defaultTheta.z;
    point.Fingers.Finger1 = defaultFingers.x;
    point.Fingers.Finger2 = defaultFingers.y;
    point.Fingers.Finger3 = defaultFingers.z;


    point.CartesianPosition = pos;
    start->Position = point;
    return start;
}


/*
* This func create TrajectoryPoint from Cartesian values.
* REMEMBER to free the pointer after use.
*/
TrajectoryPoint *Float3ToCartesian(float x, float y, float z, float thetaX, float thetaY, float thetaZ) {
    TrajectoryPoint *start = new TrajectoryPoint;
    start->InitStruct();

    UserPosition point = UserPosition();
    point.InitStruct();
    point.Type = CARTESIAN_POSITION;
    CartesianInfo pos = CartesianInfo();
    pos.X = x;
    pos.Y = y;
    pos.Z = z;
    pos.ThetaX = thetaX;
    pos.ThetaY = thetaY;
    pos.ThetaZ = thetaZ;
    point.Fingers.Finger1 = 6986;
    point.Fingers.Finger2 = 6812;
    point.Fingers.Finger3 = 6812;


    point.CartesianPosition = pos;
    start->Position = point;
    return start;
}

/*
* Translate a CartesianPosition into a float3 of the position. (lose the theta and the fingers position)
*/
float3 CartesianToFloat3(CartesianPosition point) {
    return float3{point.Coordinates.X, point.Coordinates.Y, point.Coordinates.Z};
}
/*
* Translate a point and direction to TrafictoryPoint.
* THIS WILL have default value in the fingers.
*/
TrajectoryPoint *Float3ToCartesian(float3 point, float3 direction) {
    return Float3ToCartesian(point.x, point.y, point.z, direction.x, direction.y, direction.z);
}


/*
* This func create TrajectoryPoint from Cartesian values.
* finger3 is the finger that alone in the hand.
* REMEMBER to free the pointer after use.
*/
TrajectoryPoint *
Float3ToCartesian(float x, float y, float z, float thetaX, float thetaY, float thetaZ, float finger1, float finger2,
                 float finger3) {
    TrajectoryPoint *start = new TrajectoryPoint;
    start->InitStruct();

    UserPosition point = UserPosition();
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

/*
* The first method need to be called.
* creating all the needed methods from the libery used.
* conneceting with the robot.
*/
int initRobotAPI(int vebrose) {
    int result;
    commandLayer_handle = LoadLibrary(L"CommandLayerWindows.dll");
    if (commandLayer_handle == 0)
    {
        cerr << "Could not load libery. CommandLayerWindows.dll" << endl;
        return 1;
    }
    MyInitAPI = (int (*)()) GetProcAddress(commandLayer_handle, "InitAPI");
    MyCloseAPI = (int (*)()) GetProcAddress(commandLayer_handle, "CloseAPI");
    MyGetCartesianForce = (int (*)(CartesianPosition &)) GetProcAddress(commandLayer_handle, "GetCartesianForce");
    MyMoveHome = (int (*)()) GetProcAddress(commandLayer_handle, "MoveHome");
    MyInitFingers = (int (*)()) GetProcAddress(commandLayer_handle, "InitFingers");
    MyStartForceControl = (int (*)()) GetProcAddress(commandLayer_handle, "StartForceControl");
    MyGetDevices = (int (*)(KinovaDevice devices[MAX_KINOVA_DEVICE], int &result)) GetProcAddress(commandLayer_handle,
                                                                                                  "GetDevices");
    MySetActiveDevice = (int (*)(KinovaDevice devices)) GetProcAddress(commandLayer_handle, "SetActiveDevice");
    MyGetCartesianCommand = (int (*)(CartesianPosition &)) GetProcAddress(commandLayer_handle, "GetCartesianCommand");
    MyGetAngularCommand = (int (*)(AngularPosition &)) GetProcAddress(commandLayer_handle, "GetAngularCommand");
    MyGetCartesianPosition = (int (*)(CartesianPosition &)) GetProcAddress(commandLayer_handle, "GetCartesianPosition");
    MyGetAngularPosition = (int (*)(AngularPosition &)) GetProcAddress(commandLayer_handle, "GetAngularPosition");
    MyGetAngularForce = (int (*)(AngularPosition &)) GetProcAddress(commandLayer_handle, "GetAngularForce");
    MyStopForceControl = (int (*)()) GetProcAddress(commandLayer_handle, "StopForceControl");
    MyEraseAllTrajectories = (int (*)()) GetProcAddress(commandLayer_handle, "EraseAllTrajectories");
    MySendBasicTrajectory = (int (*)(TrajectoryPoint)) GetProcAddress(commandLayer_handle, "SendBasicTrajectory");

    // additional functions for force control
    MySwitchTrajectoryTorque = (int (*)(GENERALCONTROL_TYPE)) GetProcAddress(commandLayer_handle,
                                                                             "SwitchTrajectoryTorque");
    MySetTorqueSafetyFactor = (int (*)(float)) GetProcAddress(commandLayer_handle, "SetTorqueSafetyFactor");
    MySetTorqueVibrationController = (int (*)(float)) GetProcAddress(commandLayer_handle,
                                                                     "SetTorqueVibrationController");
    MySendAngularTorqueCommand = (int (*)(float Command[COMMAND_SIZE])) GetProcAddress(commandLayer_handle,
                                                                                       "SendAngularTorqueCommand");
    MySendCartesianForceCommand = (int (*)(float Command[COMMAND_SIZE])) GetProcAddress(commandLayer_handle,
                                                                                        "SendCartesianForceCommand");
    MySetTorqueControlType = (int (*)(TORQUECONTROL_TYPE)) GetProcAddress(commandLayer_handle, "SetTorqueControlType");


    //If the API was loaded correctly
    if ((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyGetDevices == NULL)
        || (MySetActiveDevice == NULL) || (MyStartForceControl == NULL)) {
        cerr << "* * *  E R R O R   D U R I N G   I N I T I A L I Z A T I O N  * * *" << endl;
        return 0;
    } else {
        if (MySwitchTrajectoryTorque == NULL)
            cout << "* * * ERROR LOADING MySwitchTrajectoryTorque * * * " << endl;
        if(vebrose >= 1)
            cout << "I N I T I A L I Z A T I O N   C O M P L E T E D" << endl << endl;

        result = (*MyInitAPI)();
        if(vebrose >= 2)
            cout << "Initialization's result :" << result << endl;

        KinovaDevice list[MAX_KINOVA_DEVICE];


        int devicesCount = MyGetDevices(list, result);

        for (int i = 0; i < devicesCount; i++) {
            if (vebrose >= 2)
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


/*
* Using SendBasicTrajectory by with float3.
* THIS WILL USE default values for the thea and the fingers!!
*/
void MoveToPoint(float3 point) {
    TrajectoryPoint *start = Float3ToCartesian(point);
    MySendBasicTrajectory(*start);
    free(start); // ??
}

/*
* Get the force of the robot in the x, y,z in a shape of float3.
* NOT RECOMMENDED: this can be very inaccurate, recommed to use the GetMyAngularForce function.
*/
float3 GetMyForce() {
    float3 force;
    CartesianPosition curr_force;
    MyGetCartesianForce(curr_force);
    force = CartesianToFloat3(curr_force);
    return force;
}

/*
* Get the command on the robot in a float3 (of the position).
*/
float3 GetMyCommand() {
    CartesianPosition pos;
    MyGetCartesianCommand(pos);
    return float3{pos.Coordinates.X, pos.Coordinates.Y, pos.Coordinates.Z};
}
/*
* Get the position of the robot in a float3.
*/
float3 GetMyPosition() {
    CartesianPosition pos;
    MyGetCartesianPosition(pos);
    return float3{pos.Coordinates.X, pos.Coordinates.Y, pos.Coordinates.Z};
}

/*
* This is easy access to the force on the motors in the shape of a vector. 
*/
vector<float> GetMyAngularForce() {
    AngularPosition pos;
    MyGetAngularForce(pos);
    vector<float> motors;
    motors.push_back(pos.Actuators.Actuator1);
    motors.push_back(pos.Actuators.Actuator2);
    motors.push_back(pos.Actuators.Actuator3);
    motors.push_back(pos.Actuators.Actuator4);
    motors.push_back(pos.Actuators.Actuator5);
    motors.push_back(pos.Actuators.Actuator6);
    motors.push_back(pos.Actuators.Actuator7);
    return motors;
}


/*
* Used in the calibration process.
* from the strting point going to the board (in the direction of the normal - defined in the lineMath.cpp, can be seted and accessed by functions).
* stopping when the sum of the strength on the effected motors is bigger then the pushStrength.
*/
float3 GoToTheBoard(float3 start_point, vector<int> effectedMotors ,float pushStrength ,float waitInterval, int vebrose) {
    float3 curr_point = start_point;
    CartesianPosition curr_pos;
    int counter = 0, other_counter = 0, powersSum;
    vector<float> motorsPowers;
    while (true) {
        other_counter++;
        motorsPowers = GetMyAngularForce();
        if (vebrose >= 2)
        {
            cout << "Forces: ";
            for (int motorNumber : effectedMotors)
            {
                cout << " force motor " << motorNumber << ": " << motorsPowers[motorNumber];
            }
            cout << endl;
        }

        powersSum = 0;
        for (int motorNumber : effectedMotors)
            powersSum += abs(motorsPowers[motorNumber]); // default is only motor 0 and power limit of 1.2

        if (counter > 20 && powersSum > pushStrength) {
            
            if(vebrose >= 2)
                cout << "Found the board! at point: " << curr_point << endl;
            MyGetCartesianPosition(curr_pos);

            return CartesianToFloat3(curr_pos);
        }

        MoveToPoint(curr_point);
        Sleep(waitInterval); // default is 10
        MyGetCartesianPosition(curr_pos);
        if (length(CartesianToFloat3(curr_pos) - curr_point) < 0.01 ||
            other_counter >= 10) // got to the point, but not the board yet
        {
            MyEraseAllTrajectories();
            counter++;
            other_counter = 0;
            curr_point -= (float3) (getNormal() * 0.0002);

        }
    }
}



/*
* Get a line as a sequence of a float3 points, and going to them one by one.
*/
void mainLoopForDrawLine(vector <float3> line, float waitInterval, float endWaitInterval ,int vebrose) {
    for (auto point: line) {
        TrajectoryPoint *pos = Float3ToCartesian(point);
        MySendBasicTrajectory(*pos);
        Sleep(waitInterval); // default is 2
        if(vebrose >= 3)
            cout << "Send to Robot the point: " << point << endl;
    }
    if (vebrose >= 2)
        cout << "Finish a line!" << endl;
    Sleep(endWaitInterval); // default is 300 
}

/*
* Disconnect form the robot in a safe way.
*/
void disconnectFromRobot() {
    MyCloseAPI();
}



/*
* Reading a file with the format of drawing, and draw by it. (and the basis that loaded).
*/
void drawFile(string fileName, float waitInterval, float bigWaitInterval, int pointsInLine, int vebrose) {
    ifstream MyReadFile(fileName);
    string line;
    string delimiter = ":";
    vector <string> line_splited;
    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, line)) {
        // Output the text from the file
        line_splited = vector<string>();
        size_t pos = 0;
        std::string token;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, pos);
            line_splited.push_back(token);
            line.erase(0, pos + delimiter.length());
        }


        if (line_splited[0] == "L") // need to draw a line
        {
            float2 first_point = float2{std::stof(line_splited[1]), std::stof(line_splited[2])};
            float2 second_point = float2{std::stof(line_splited[3]), std::stof(line_splited[4])};
            bool drawing = line_splited[5] == "T";
            mainLoopForDrawLine(getLine(first_point, second_point, pointsInLine, drawing), waitInterval, bigWaitInterval, vebrose);
        } else if (line_splited[0] == "C") {
            float2 center = float2{std::stof(line_splited[1]), std::stof(line_splited[2])};
            float radios = std::stof(line_splited[3]);
            float start_angle = std::stof(line_splited[4]);
            float draw_angle = std::stof(line_splited[5]);
            mainLoopForDrawLine(getCircArc(center, radios, start_angle, draw_angle, pointsInLine * 2), waitInterval, bigWaitInterval, vebrose);
        } else if (line_splited[0] == "Q") {
            float2 first_point = float2{std::stof(line_splited[1]), std::stof(line_splited[2])};
            float2 second_point = float2{std::stof(line_splited[3]), std::stof(line_splited[4])};
            float2 third_point = float2{std::stof(line_splited[5]), std::stof(line_splited[6])};
            mainLoopForDrawLine(getQuadBezierCurve(first_point, second_point, third_point, pointsInLine * 2), waitInterval, bigWaitInterval, vebrose);
        } else if (line_splited[0] == "B") {
            float2 first_point = float2{std::stof(line_splited[1]), std::stof(line_splited[2])};
            float2 second_point = float2{std::stof(line_splited[3]), std::stof(line_splited[4])};
            float2 third_point = float2{std::stof(line_splited[5]), std::stof(line_splited[6])};
            float2 fourth_point = float2{std::stof(line_splited[7]), std::stof(line_splited[8])};
            mainLoopForDrawLine(getCubicBezierCurve(first_point, second_point, third_point, fourth_point, pointsInLine * 2), waitInterval, bigWaitInterval, vebrose);
        }
    }
}

/*
* This is used in the calibration.
* This function using the normal (not have to be the real normal, can be only in the general direction of the board) 
* to go in the direction of the board from the three starting points. 
* and return the 
*/
vector <float3> findTheBoard(vector<float3> startingPoints, vector<int> effectedMotors, float pushStrength, float bigWaitInterval, float waitInterval, int vebrose) {
    vector <float3> basis = getNewBasis(LEFT_DOWN, RIGHT_DOWN, LEFT_UP);
    
    TrajectoryPoint *start = Float3ToCartesian(startingPoints[0]);
    MySendBasicTrajectory(*start);
    Sleep(bigWaitInterval); // default is 4000
    float3 right_down = GoToTheBoard(startingPoints[0], effectedMotors, pushStrength, waitInterval, vebrose);
    MySendBasicTrajectory(*start);
    Sleep(bigWaitInterval);

    start = Float3ToCartesian(startingPoints[1]);
    MySendBasicTrajectory(*start);
    Sleep(bigWaitInterval);
    float3 right_up = GoToTheBoard(startingPoints[1], effectedMotors, pushStrength, waitInterval, vebrose);
    MySendBasicTrajectory(*start);
    Sleep(bigWaitInterval);


    start = Float3ToCartesian(startingPoints[2]);
    MySendBasicTrajectory(*start);
    Sleep(bigWaitInterval);
    float3 left_down = GoToTheBoard(startingPoints[2], effectedMotors, pushStrength, waitInterval, vebrose);
    MySendBasicTrajectory(*start);
    Sleep(bigWaitInterval);
    free(start);

    return getNewBasis(left_down, right_down, right_up);
}

/*
This function saving the basis of the board in a file.
*/
void saveBasis(vector <float3> basis, string path, int vebrose) {
    ofstream myfile;
    float3 baseBoard = getBaseBoard();
    myfile.open(path);


    myfile.write((char *) &(basis[0]), sizeof(float3));
    myfile.write((char *) &(basis[1]), sizeof(float3));
    myfile.write((char *) &(basis[2]), sizeof(float3));
    myfile.write((char *) &(baseBoard), sizeof(float3));
    myfile.close();
    if (vebrose >= 1)
        cout << "Saved the basis!" << endl;
}

/*
* This function is for loading a basis for the board from a file.
*/
void loadBasis(vector <float3> basis, string path, int vebrose) {
    ifstream myfile;
    float3 left_down;
    myfile.open(path);

    while (basis.size() < 3)
        basis.push_back(float3());

    myfile.read((char *) &(basis[0]), sizeof(float3));
    myfile.read((char *) &(basis[1]), sizeof(float3));
    myfile.read((char *) &(basis[2]), sizeof(float3));
    myfile.read((char *) &(left_down), sizeof(float3));
    myfile.close();
    updateBasis(left_down, basis[0], basis[1], basis[2]);
    if (vebrose >= 1)
        cout << "Load the basis!" << endl;
    if(vebrose >= 3)
        cout << "left down: " << left_down << ", basis[0]: " << basis[0] << ", basis[1]: " << basis[1] << ", basis[2]: "
         << basis[2] << endl;
}

/*
* The main function to use for drawing and calibration the robot.
* all the options are from the command line. (there will not be input mid-run).
* the default state is with calibration for the robot, and it will not be saved.
* OPTIONS:
-v Vebrose - print actions.
-vv Vebrose2 - print more actions. 
-VV Vebrose3 - print everything.
-f [path] path for a file to draw. (if not used will not draw anything).
-l [path] load a calibration.
-s [path] save the calibration.
-cw calibration small wait interval (effection the speed of the movment of the arm druin calibrations)
-cW calibration big wait interval (effetion the wait between two times touching the board)
-w normal small wait interval (effection the speed of the movment of the arm in normal drawing)
-W normal big wait interval (effection the wait between two drawings)
-c-points [x] [y] [z] [x1] [y1] [z1] [x2] [y2] [z2] set the starting points for the calibration.
-c-normal [x] [y] [z] set the normal to be used in the calibration.
-c-motors [HowManyMotors] [motorNumber] ... [number1] (the motors to consider in the calibration) 
-p [strength] push strength in the calibration
-theta [x] [y] [z] he theta to be used on the arm.
-fingers [1] [2] [3] the position to be used on the fingers.
-n [number] the numbers of points in a line (more will be slower, but more acurrate)
-starting-point [x] [y] [z] the point to start from when drawing. (here to help with difficult places to draw in at the start)
-h pring help
*/
int main(int argc, char ** argv) {
    int programResult, result, vebrose = 0, motorsCounter = 0, pointsInLine = 40;
    float calibrationWaitInterval = 10, calibrationBigWaitInterval = 4000, waitInterval = 2, bigWaitInterval = 300, pushStrength = 1.2;
    vector<float3> calibrationPoints;
    vector<int> relevantMotors;
    float3 calibrationNormal = float3{ 0,0,0 };
    float3 startingPoint = float3{ 0,0,0 };
    string loadPath = "", savePath = "", drawPath = "";
    
    bool error = false;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-v"))
        {
            vebrose = 1;
        }
        else if (!strcmp(argv[i], "-vv"))
        {
            vebrose = 2;
        }
        else if (!strcmp(argv[i], "-VV"))
        {
            vebrose = 3;
        }
        else if (!strcmp(argv[i], "-cw"))
        {
            if (i + 1 > argc)
            {
                error = true;
                break;
            }
            i++;
            calibrationWaitInterval = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-cW"))
        {
            if (i + 1 > argc)
            {
                error = true;
                break;
            }
            i++;
            calibrationBigWaitInterval = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-w"))
        {
            if (i + 1 > argc)
            {
                error = true;
                break;
            }
            i++;
            waitInterval = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-W"))
        {
            if (i + 1 > argc)
            {
                error = true;
                break;
            }
            i++;
            bigWaitInterval = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-p"))
        {
            if (i + 1 > argc)
            {
                error = true;
                break;
            }
            i++;
            pushStrength = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-c-points"))
        {
            if (i + 9 > argc)
            {
                error = true;
                break;
            }
            calibrationPoints.push_back(float3{ (float)atof(argv[i + 1]), (float)atof(argv[i + 2]), (float)atof(argv[i + 3]) });
            i += 3;
            calibrationPoints.push_back(float3{ (float)atof(argv[i + 1]), (float)atof(argv[i + 2]), (float)atof(argv[i + 3]) });
            i += 3; 
            calibrationPoints.push_back(float3{ (float)atof(argv[i + 1]), (float)atof(argv[i + 2]), (float)atof(argv[i + 3]) });
            i += 3;
        }
        else if (!strcmp(argv[i], "-c-normal"))
        {
            if (i + 3 > argc)
            {
                error = true;
                break;
            }
            calibrationNormal = float3{ (float)atof(argv[i + 1]), (float)atof(argv[i + 2]), (float)atof(argv[i + 3]) };
            i += 3;
        }
        else if (!strcmp(argv[i], "-theta"))
        {
            if (i + 3 > argc)
            {
                error = true;
                break;
            }
            defaultTheta = float3{ (float)atof(argv[i + 1]), (float)atof(argv[i + 2]), (float)atof(argv[i + 3]) };
            i += 3;
        }
        else if (!strcmp(argv[i], "-fingers"))
        {
            if (i + 3 > argc)
            {
                error = true;
                break;
            }
            defaultFingers = float3{ (float)atof(argv[i + 1]), (float)atof(argv[i + 2]), (float)atof(argv[i + 3]) };
            i += 3;
        }
        else if (!strcmp(argv[i], "-starting-point"))
        {
            if (i + 3 > argc)
            {
                error = true;
                break;
            }
            startingPoint = float3{ (float)atof(argv[i + 1]), (float)atof(argv[i + 2]), (float)atof(argv[i + 3]) };
            i += 3;
        }
        else if (!strcmp(argv[i], "-l"))
        {
            if (i + 1 > argc)
            {
                error = true;
                break;
            }
            i++;
            loadPath = argv[i];
        }
        else if (!strcmp(argv[i], "-n"))
        {
        if (i + 1 > argc)
        {
            error = true;
            break;
        }
        i++;
        pointsInLine = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-f"))
        {
        if (i + 1 > argc)
        {
            error = true;
            break;
        }
        i++;
        drawPath = argv[i];
        }
        else if (!strcmp(argv[i], "-s"))
        {
            if (i + 1 > argc)
            {
                error = true;
                break;
            }
            i++;
            savePath = argv[i];
        }
        else if (!strcmp(argv[i], "-c-motors"))
        {
            if (i + 1 > argc)
            {
                error = true;
                break;
            }
            i++;
            motorsCounter = atoi(argv[i]);
            if (i + motorsCounter > argc)
            {
                error = true;
                break;
            }
            i++;
            for (int j = 0; j < motorsCounter; j++)
            {
                relevantMotors.push_back(atoi(argv[i + j]));
            }
            i += motorsCounter - 1;
        }
        else if (!strcmp(argv[i], "-h"))
        {
            cout << "* The main function to use for drawing and calibration the robot.\n* all the options are from the command line. (there will not be input mid - run).\n* the default state is with calibration for the robot, and it will not be saved.\n* OPTIONS:\n-v Vebrose - print actions.\n-vv Vebrose2 - print more actions.\n-VV Vebrose3 - print everything.\n-f [path] path for a file to draw. (if not used will not draw anything).\n-l [path] load a calibration.\n-s [path] save the calibration.\n-cw calibration small wait interval(effection the speed of the movment of the arm druin calibrations).\n-cW calibration big wait interval(effetion the wait between two times touching the board)\n-w normal small wait interval(effection the speed of the movment of the arm in normal drawing)\n-W normal big wait interval(effection the wait between two drawings)\n-c-points [x] [y] [z] [x1] [y1] [z1] [x2] [y2] [z2] set the starting points for the calibration.\n-c-normal [x] [y] [z] set the normal to be used in the calibration.\n-c-motors [HowManyMotors] [motorNumber] ... [number1] (the motors to consider in the calibration)\n-p [strength] push strength in the calibration\n-theta [x] [y] [z] he theta to be used on the arm.\n-fingers [1] [2] [3] the position to be used on the fingers.\n-n [number] the numbers of points in a line (more will be slower, but more acurrate)\n-starting-point [x] [y] [z] the point to start from when drawing. (here to help with difficult places to draw in at the start)\n-h pring help" << endl;
            return 0;
        }
        else
        {
            error = true;
            break;
        }
    }

    if (error)
    {
        cerr << "ERROR: in the parameters." << endl;
        return 1;
    }

    if (!initRobotAPI(vebrose))
        return 1;
    MyMoveHome();
    Sleep(5000);

    vector <float3> basis;
    if (loadPath.empty())
    {
        if (calibrationNormal != float3{ 0,0,0 })
            setNormal(calibrationNormal);
        if (calibrationPoints.size() != 3)
        {
            cerr << "ERROR: try to calibraite and didn't set calibration points." << endl;
            return 1;
        }
        if (relevantMotors.size() == 0)
        {
            relevantMotors.push_back(0);
        }
        basis = findTheBoard(calibrationPoints,relevantMotors, pushStrength, calibrationBigWaitInterval, calibrationWaitInterval, vebrose);
        if(!savePath.empty())
            saveBasis(basis, savePath, vebrose);
    }
    else
    {
        loadBasis(basis, loadPath, vebrose);
    }
    
    if (startingPoint != float3{ 0,0,0 })
    {
        TrajectoryPoint* start = Float3ToCartesian(startingPoint);
        MySendBasicTrajectory(*start);
        Sleep(7000);
    }
    
    if(!drawPath.empty())
        drawFile(drawPath, waitInterval, bigWaitInterval, pointsInLine, vebrose);

    if (vebrose >= 1)
        cout << "Done!" << endl;
    disconnectFromRobot();
}