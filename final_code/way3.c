#pragma config(Sensor, S1,     ts,             sensorEV3_Color)//should input
#pragma config(Sensor, S2,     color,          sensorEV3_Color)
#pragma config(Sensor, S3,     color,          sensorEV3_modeEV3Color_Color)
#pragma config(Sensor, S4,     Ultrasonic,     sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,          ML,          tmotorEV3_Large)
#pragma config(Motor,  motorC,          MR,         tmotorEV3_Large)

#define Threshold 42
#define Th_dis 25

int pwr = 60;
int step_y = (int)(360*200/56/PI); //one step distance is 409 degree for 200mm
int step_y = (int)(360*240/56/PI);
int rot90 = 200; //point turn//swing turn (int)(PI*120.0/56.0*90);

int dirNorth = 0;
int dirEast = 1;
int dirSouth = 2;
int dirWest = 3;
int robotX = 3, robotY = 0, robotDir = dirNorth;
int right, left, center, distance;

void Move(short enc)
{
    nMotorEncoderTarget[ML] = abs(enc);
    nMotorEncoderTarget[MR] = abs(enc);
    motor[ML] = pwr;
    motor[MR] = pwr;
    while(nMotorRunState[ML] != runstateIdle || nMotorRunstate[MR] != runstateIdle){}
    
    if(robotDir == dirNorth) robotY++;
    else if (robotDir == dirEast) robotX++;
    else if (robotDir == dirSouth) robotY--;
    else if (robotDir == dirWest) robotX--;

}

void Turn(short pwrt, long enct)
{
    nMotorEncoderTarget[ML] = enct;
    nMotorEncoderTarget[MR] = enct;
    motor[ML] = pwrt;
    motor[MR] = -pwrt;
    while(nMotorRunState[ML] != runstateIdle || nMotorRunstate[MR] != runstateIdle){}
}

void Turn90(bool clockwise)
{
    if(clockwise){
        Turn(pwr/2, rot90);
        robotDir++;
    }
    else{
        Turn(-pwr/2, rot90);
        robotDir--;
    }
    robotDir += 4;
    robotDir %= 4;
}

task display()
{
    while(true)
    {
        right = SensorValue(RL);
        left = SensorValue[LL];
        center = SensorValue(CL);
        distance = SensorValue(Sonic);
        writeDebugStreamLine(0,"DIRECTION = %d",robotDir);
        writeDebugStreamLine(1,"X = %d, Y = %d",robotX, robotY);
        writeDebugStreamLine(2,"c/r/l = %d %d %d",center, right, left);
        writeDebugStreamLine(3,"distance = %d",distance);
    }
    

}

task main()
{
    startTask(display);
    Move(step_y);

    while(robotY<18)
    {
        switch (robotDir)
        {
        case 0:
            if(center>Threshold && distance>Th_dis){
                Move(step_y); 
                wait1Msec(500);
            }
            else if(left>Threshold){
                Turn90(false);
                wait1Msec(500);
            }
            else if(left>Threshold){
                Turn90(true);
                wait1Msec(500);
            }
            else{
                Turn90(true);
                Turn90(true);
                wait1Msec(500);
            }
            break;            
        case 1;
            if(left>Threshold){
                Turn90(false);
                wait1Msec(500);
            }
            else if(center>Threshold && distance>Th_dis){
                Move(step_x);
                wait1Msec(500);
            }
            else{
                Turn90(true);
                Turn90(true);
                wait1Msec(500);
            }            
            break;
        case 2:
            Turn90(true);
            Turn90(true);
            wait1Msec(500);
            break; 
        case 3:
            if(right>Threshold){
                Turn90(true); 
                wait1Msec(500);
            }
            else if(center>Threshold && distance>Th_dis){
                Move(step_x);
                wait1Msec(500);
            }
            else{
                Turn90(true);
                Turn90(true);
                wait1Msec(500);
            }            
            break;               
        default:
            robotDir %= 4;
        }
    }
    StopTask(display);
}