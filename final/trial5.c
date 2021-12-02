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
void first_react();
void first_energy();

void Forward();
void Backward();
void LeftTurn();
void RightTurn();
void U_Turn();
void random_move();
void frandom_move();

//function
void digestion()
{
    playSound(soundBeepBeep);
}

void homeostasis(){
    row_dest = 0;
    col_dest = 0;
}

void first_react(){
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
        Forward(); //random_move(); //Forward();
    }

    while(SensorValue[S4]<10) //적외선 센서로 장애물을 인식`했을 때
    {   
        motor[mb] = 0; motor[mc] = 0; sleep(500);
        Backward(); //Righturn();
        LeftTurn();
    }
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
        random_move(); //Forward();
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
        if(SensorValue[S2]==5 || SensorValue[S3]==5)
        {
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

void first_energy() //먹이를 찾음
{
    while(true){
        first_react();
        if(SensorValue[S2]==5 || SensorValue[S3]==5)
        {
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
        if(SensorValue[S2]==2 || SensorValue[S3]==2)
        {
            motor[mb]=0; motor[mc]=0; sleep(500);
            digestion();
            U_Turn();
            motor[mb]=0; motor[mc]=0; sleep(300);
            frandom_move();
            break;
        }
    }
}

void first_Home() //먹이를 찾음
{
    while(true){
        first_react();
        if(SensorValue[S2]==5 || SensorValue[S3]==5)
        {
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
    motor[mb]=30; motor[mc]=30; sleep(300);
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

void frandom_move(){
    while(1){
        if(r==0) motor[mb]=-40; motor[mc]=40; sleep(300); r++; break;
        if(r==1) motor[mb]=-40; motor[mc]=40; sleep(600); r++; break;
        if(r==2) motor[mb]=-40; motor[mc]=40; sleep(900); r++; break;
        if(r==3) motor[mb]=-40; motor[mc]=40; sleep(1200); r++; break;
    }
}

/*
void frandom_move(){
    for(int i=1;i<food_num;i++){
        motor[mb]=-40; motor[mc]=40; sleep(300*i); 
        food_num++;
        break;
    }
}
*/

/*
제작 실습
필수 수행내용  구현 여부 
아이디어의 참신성
효율적으로 필수 기능이 구현되었는지?
스스로 학습하는 기능이 구현되었는지?
필수 수행내용 이외의 생명체의 특징을 구현하였는지?

로봇은 경기장 정보에 대해서 모르며 무작위로 움직임
경기장 가장자리를 인식하여 밖으로 나가면 안됨(반응) – 이탈시마다 감점
로봇을 먹이(빨간색)를 찾으려고 하며 (에너지 획득) 먹이를 먹은 뒤 집(파란색)으로 돌아가야 함. (대사활동) 먹이를 만나거나 집으로 돌아오면 소리를 냄
적(사각박스)을 만나면 피해야함 (반응)
제한시간 (3분) 이내에 먹이를 최대한 많이 먹는(먹이와 집을 왕복하는) 로봇을 구현하는 것이 목표임 (적응)

구성
 구조적 기능적 기본 단위인 세포로 구성
(물질 대사)
 생명을 유지하기 위한 에너지의 흡수와 방출
(자극에 대한 반응)
 외부 환경요인에 의한 행동,운동, 혹은 대사과정의 변화
항상성 유지
 외부 환경 변화에 내부 환경을 일정하게 유지
생식과 발달   
 생식에 의한 탄생, 발달 단계를 거치는 성장과 죽음
적응과 진화
 외부 환경요인에 유리한 특성을 획득
*/