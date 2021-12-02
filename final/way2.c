#pragma config(Sensor, S1,     ts,             sensorEV3_Touch)//should input
#pragma config(Sensor, S2,     color,          sensorEV3_Color)
#pragma config(Sensor, S3,     color,          sensorEV3_Color)
#pragma config(Sensor, S4,     Ultrasonic,     sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,          left,          tmotorEV3_Large)
#pragma config(Motor,  motorC,          right,         tmotorEV3_Large)

int play[1000][1000];
int modearr[1000]; //1000으로 크게 우선 설정
int mode = 3; //처음 모드는 3
int arrsize = 0;

int loc[] = {0,0}; //location

void Forward()
{
    while(nMotorEncoder[moterB]<360) //17.6cm
    {
        motor[motorB] = 80;
        motor[motorC] = 30;
    }

    loc[2] += 20;
}

void Right()
{
    while(nMotorEncoder[moterB]<360) //17.6cm
    {
        motor[motorB] = 30;
        motor[motorC] = 80;
    }
    loc[1] += 10;
    loc[2] += 20;
}

void Left()
{
    while(nMotorEncoder[moterB]<360) //17.6cm
    {
        motor[motorB] = 80;
        motor[motorC] = 30;
    }
    loc[1] += 20;
    loc[2] += 10;
}

task main(){
  while(true){
    if(SensorValue[S4]<20) //적외선 센서로 장애물을 인식`했을 때
    {   
        motor[motorB] = 0;
        motor[motorC] = 0;
        //일단 정지
        wait1Msec(100);
        motor[motorB] = 50;
        motor[motorC] = -60;
        //무조건 우회전
    }
    else{ //장애물이 멀리 있을 때, 이동하기

      if(SensorValue[S2]==1 && SensorValue[S3]==6) //종이 밖으로 벗어나려고 할 때, 왼 검정 우 흰색
      {   
          motor[motorB] = 50;
          motor[motorC] = -60;
          //우회전
      }else if(SensorValue[S2]==6 && SensorValue[S3]==1) //종이 밖으로 벗어나려고 할 때, 왼 흰색 우 검정
      {   
          motor[motorB] = -60;
          motor[motorC] = 50;
          //좌회전
      }

      if(SensorValue[S2]==2 || SensorValue[S3]==2) //파랑, 집이라면, 시작
      { 
        modearr[num] = 0; //mode가 0이라는 것은 시작이라는 뜻
      }
      while(SensorValue[S2] !=5 || SensorValue[S3] !=5) //빨간색, 먹이에 도착하기 전까지
      {
        run(mode);
        modearr[arrsize] = mode;
        arrsize++;
        mode = radom();
      }
      if(SensorValue[S2]==5 || SensorValue[S3]==5) //빨강, 먹이에 도착했다면
      {  
        playSound(soundBeepBeep);
        reverserun();
      }
    }
  }
}