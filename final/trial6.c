#pragma config(Sensor, S2,     color,          sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3,     color,          sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     Ultrasonic,     sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,         mb,          tmotorEV3_Large)
#pragma config(Motor,  motorC,         mc,         tmotorEV3_Large)

//initialize
int row_dest; //현재의 행위치
int col_dest; //현재의 열위치

int dest_num = 0;
int row_destination[10]; //최종 행위치, 먹이는 크게 잡아 10개라고 가정
int col_destination[10]; //최종 열위치

int r=0;
int rr;

//declare
void digestion(); //대사활동, 소화 //소리를 내는 함수
void homeostasis(); // 항상성 유지
void react(); //반응
void energy(); //에너지 획득
void Home(); //집으로 돌아옴. 
//void learn(); //학습

void Forward();
void Backward();
void LeftTurn();
void RightTurn();
void U_Turn();
void random_move();

//function
void digestion()
{
    playSound(soundBeepBeep);
}

void homeostasis(){
    row_dest = 0;
    col_dest = 0;
}

void react()
{
    if(SensorValue[S2]==1){ //왼쪽 센서가 검정선을 인식했을 때
        motor[mb] = 0; motor[mc] = 0; sleep(500);
        Backward();
        RightTurn(); //오른쪽 trun
    }
    else if(SensorValue[S3]==1){ //오른쪽 센서가 검정선을 인식했을 때
        motor[mb] = 0; motor[mc] = 0; sleep(500);
        Backward();
        LeftTurn(); //왼쪽 Turn
    }
    else{
        Forward(); //Forward();
    }

    while(SensorValue[S4]<10) //적외선 센서로 장애물을 인식`했을 때
    {   
        motor[mb] = 0; motor[mc] = 0; sleep(500);
        Backward(); //Righturn();
        LeftTurn();
    }
}

void energy() //먹이를 찾음
{
    while(true){
        react();
        if(SensorValue[S2]==5)
        {
            Forward();
            if(dest_num != 0){
                for(int i=0; i<dest_num; i++){
                    if(row_dest-5<=row_destination[i] && row_destination[i]<=row_dest+5 && col_dest-5<=col_destination[i] && col_destination[i]<=col_dest+5){ 
                        break; //현재 위치가 저장된 위치들과 비슷(먹이의 크기 고려)한지 판단 , 비슷하면 이미 먹은 먹이임
                    }
                }
            }
            row_destination[dest_num] = row_dest; //현재 위치 저장
            col_destination[dest_num] = col_dest;
            dest_num++;
            homeostasis();

            motor[mb]=0; motor[mc]=0; sleep(500);
            digestion();
            U_Turn();
            break;
        }
        else if(SensorValue[S3]==5)
        {
            Forward();
            if(dest_num != 0){
                for(int i=0; i<dest_num; i++){
                    if(row_dest-5<=row_destination[i] && row_destination[i]<=row_dest+5 && col_dest-5<=col_destination[i] && col_destination[i]<=col_dest+5){ 
                        break; //현재 위치가 저장된 위치들과 비슷(먹이의 크기 고려)한지 판단 , 비슷하면 이미 먹은 먹이임
                    }
                }
            }
            row_destination[dest_num] = row_dest; //현재 위치 저장
            col_destination[dest_num] = col_dest;
            dest_num++;
            homeostasis();

            motor[mb]=0; motor[mc]=0; sleep(500);
            digestion();
            U_Turn();
            break;
        }
    }
}

void Home()
{
    while(true){
        react();
        homeostasis(); //집으로 돌아올 때는 위치 저장할 필요 없음  
        if(SensorValue[S2]==2)
        {
            Forward();
            motor[mb]=0; motor[mc]=0; sleep(500);
            digestion();
            U_Turn();
            motor[mb]=50; motor[mc]=50; sleep(1000);
            random_move();
            break;
        }
        else if(SensorValue[S3]==2)
        {
            Forward();
            motor[mb]=0; motor[mc]=0; sleep(500);
            digestion();
            U_Turn();
            motor[mb]=50; motor[mc]=50; sleep(1000);
            random_move();
            break;
        }
    }
}

task main()
{
    homeostasis();
    while(true)
    {
        energy();
        Home();
    }
}

void LeftTurn() //mode1
{
    motor[mb]=-40; motor[mc]=40; sleep(600);
    row_dest += 1;
}

void RightTurn() //mode2
{
    motor[mb]=40; motor[mc]=-40; sleep(600);
    row_dest -= 1;
}

void Forward() //mode3
{
    motor[mb]=30; motor[mc]=30; sleep(150);
    col_dest += 1;
}

void Backward() //mode4
{
    motor[mb]=-30; motor[mc]=-30; sleep(600);
    col_dest -= 1;
}

void U_Turn()
{//LeftUTurn;
    motor[mb]=-40; motor[mc]=40; sleep(1300); //180도 회전
}

void random_move(){
    rr = random(1); //0~2의 무작위 수

    if(rr==0) LeftTurn();
    if(rr==1) RightTurn();
}