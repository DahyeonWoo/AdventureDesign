#pragma config(Sensor, S1,     color,          sensorEV3_Color)//should input
#pragma config(Sensor, S2,     color,          sensorEV3_Color)
#pragma config(Sensor, S3,     color,          sensorEV3_modeEV3Color_Color)
#pragma config(Sensor, S4,     color,          sensorEV3_modeEV3Color_Color)
#pragma config(Motor,  motorA,          ma,         tmotorEV3_Medium,PIDControl,encoder)
#pragma config(Motor,  motorC,          mb,         tmotorEV3_Large,PIDControl,driveLeft,encoder)
#pragma config(Motor,  motorB,          mc,         tmotorEV3_Large,PIDControl,driveRight,encoder)
#pragma config(Motor,  motorC,          md,         tmotorEV3_Large,PIDControl,encoder)

//initialize
int Threshold = 20;
int readcolor[10] = {0,0,0,0,0,0,0,0,0,0};
int destination[2]; //(Row,Col)
int current_position[2]; //(Row,Col)
int current_direction; //U1 R2 D3 L4
int requested_direction;

//declare;
void LeftTurn();
void RightTurn();
void U_Turn();

void ReadColor();
void Destination();
void Navigation();
void LineTracing();
void Hand();

void Home();
//declare

task main()
{
    motor[ma] = 30; sleep(300); //open =+, close=+
    motor[md] = 30; sleep(700); //hand up =-, hand down =+
    //displayTextLine(1,%d)
    motor[ma] = 50;
    motor[mc] = 50;
    sleep(800);

    while(true)
    {
        if(SensorValue(S1)<ThresHold){
            motor[mb] = 0; motor[mc] = 0; sleep(100);
            break;
        }
        else{
            motor[mb]=30; motor[mc]=30;
        }
        motor[mb] = 50; motor[mc] = 50; sleep(200);
        
        LineTracing();

        current_direction = 2;
        current_position[0] = 3; //(r,c) = (3,-1)
        current_position[1] = 0;

        for(int count=1;count<=5;++count)//5
        {
            Destination();
            displayTextLine(3, "Destination: %d %d",destination[0],destination[1]);

            Navigation();
            displayTextLine(5,"Block:%d,Tile:%d", readcolor[8],readcolor[9]);
        }
        
        Home(); //go Home

        return;
    }
}// main

//function
void LeftTurn()
{
    motor[mb]=40; motor[mc]=40; sleep(500);
    motor[mb]=-40; motor[mc]=40; sleep(500);

    while(true)
    {
        if(SensorValue(S1)<Threshold){
            motor[mb]=-40; motor[mc]=40; sleep(50);
            return;
        }
        else motor[mb]=-40; motor[mc]=40;
    }
    motor[mb]=50; motor[mc]=50; sleep(100);
}

void RightTurn()
{
    motor[mb]=40; motor[mc]=40; sleep(500);
    motor[mb]=40; motor[mc]=-40; sleep(500);

    while(true)
    {
        if(SensorValue(S2)<Threshold){
            motor[mb]=40; motor[mc]=-40; sleep(50);
            return;
        }
        else motor[mb] = 40; motor[mc]=-40;
    }
    return;
}

void UTurn()
{//LeftUTrun
    motor[mb]=40; motor[mc]=40; sleep(500); //half go
    motor[mb]=-40; motor[mc]=40; sleep(1500);

    while(true)
    {
        if(SensorValue(S2)<Threshold){
            motor[mb]=0; motor[mc]=0; sleep(50);
            return;
        }
    }
    return;
}

void ReadColor()
{
    setSensorMode(S4, 2); //leftside == 1, 0==reflect, 1==Ambient, 2==color
    int ref=0;

    while(true)
    {
        if(SensorValue(S4)==5||SensorValue(S4)==3||SensorValue(S4)==2||SensorValue(S4)==4) //0:nocolor, 1:black, 6:while//Red:5 Green:3, Blue:2, yellow:4
        {
            readcolor[ref] = SensorValue(S4); //save color
            ++ref;
            if(ref==10){
                motor[mb]=0; motor[mc]=0; sleep(100);
                return;
            }
            else{
                playSound(soundBlip);
                motor[mb]=50; motor[mc]=50; sleep(200);
            }

        }
        //S2 LineTracing, S1 Read
        if(SensorValue(S1)<Threshold){
            motor[mb]=25; motor[mc]=30;
        }
        else if(SensorValue(S1)>Threshold){
            motor[mb]=30; motor[mc]=25;
        }
    }
}

void Destination()
{
    int object[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    int r,c;

    for(r=0;r<4;++r){
        if(readcolor[r]==readcolor[8]){
            ++object[r][0];
            ++object[r][1];
            ++object[r][2];
            ++object[r][3];
            goto skip1;
        }
    }

    skip1:
        for(c=4;c<8;++c){
            if(readcolor[c] == readcolor[9]){
                ++object[0][c-4];
                ++object[1][c-4];
                ++object[2][c-4];
                ++object[3][c-4];
                goto skip2;
            }
        }

    skip2:
        for(r=0;r<4;++r){
            for(c=0;c<4;++c){
                if(object[r][c] == 2){
                    destination[0] = r;
                    destination[1] = c;
                    playSound(soundBeepBeep); sleep(100);
                    return;
                }
            }
        }       
}

void Navigation()
{
    //+r == down, +c == right, -r == up, -c==left

    while(destination[0] != current_position[0])
    {
        if(destination[0]<current_position[0]){ //go up

            requested_direction = 1;
            if(current_direction == 1){}
            else if(current_direction == 2) LeftTurn();
            else if(current_direction == 3) U_Turn();
            else if(current_direction == 4) RightTurn();

            current_direction = 1; //turning complete
            LineTracing();
            current_position[0]--;
        }
        else if(destination[0]>current_position[0]){ //go down

            requested_direction = 3;
            if(current_direction == 1) U_Turn();
            else if(current_direction == 2) RightTurn();
            else if(current_direction == 3) {}
            else if(current_direction == 4) LeftTurn();

            current_direction = 3; //turning complete
            LineTracing();
            current_position[0]++;
        }
    } //while
    while(destination[1] != current_position[1])
    {
        if(destination[1]<current_position[1]){ //go left

            requested_direction = 4;
            if(current_direction == 1) LeftTurn();
            else if(current_direction == 2) U_Turn();
            else if(current_direction == 3) RightTurn();
            else if(current_direction == 4){}

            current_direction = 4; //turning complete
            LineTracing();
            current_position[1]--;
        }
        else if(destination[1]>current_position[1]){ //go right

            requested_direction = 2;
            if(current_direction == 1) RightTurn();
            else if(current_direction == 2) {}
            else if(current_direction == 3) LeftTurn();
            else if(current_direction == 4) U_Turn();

            current_direction = 2; //turning complete
            LineTracing();
            current_position[1]++;
        }
    } //while
    //navigate complete

    playSound(soundUpwardTones); sleep(100);
    Hand();
    return;
}

void LineTracing()
{
    setSensorMode(S3,2); //color
    //ma: open =+, close =-;
    //md: hand up =-, hand down=+

    if(destination[0]==current_position[0] && abs(destination[1]-current_position[1])==1) 
    || (abs(destination[0]-current_position[0]==1&&destination[1]==current_position[1])){
        motor[md]=30; sleep(700);
        motor[ma]=30; sleep(300);
    }
    while(true)
    {

    }
}

void Home()
{
    destination[0] = 3; //y
    destination[1] = -3; //x
    Navigation();
    playSound(soundException); sleep(100);
    return;
}