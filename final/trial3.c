#pragma config(Sensor, S2,     color,          sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3,     color,          sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     Ultrasonic,     sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,         mb,          tmotorEV3_Large)
#pragma config(Motor,  motorC,         mc,         tmotorEV3_Large)

//initialize
int dest_num = 0;
int destination[2]; //(Row,Col)
int all_destination[10][2];

//declare
void digestion(); //대사활동, 소화 //소리를 내는 함수
void react(); //반응
void energy(); //에너지 획득
void Home(); //집으로 돌아옴. 

void Forward();
void Backward();
void LeftTurn();
void RightTurn();
void U_Turn();


void digestion()
{
    playSound(soundBeepBeep);
}

void react()
{
    if(SensorValue[S2]==1){ //왼쪽 센서가 검정선을 인식했을 때
        motor[mb] = 0; motor[mc] = 0; sleep(500);
        RightTurn(); //오른쪽 trun
    }
    else if(SensorValue[S3]==1){ //오른쪽 센서가 검정선을 인식했을 때
        motor[mb] = 0; motor[mc] = 0; sleep(500);
        LeftTurn(); //왼쪽 Turn
    }
    else{
        Forward();
    }

    while(SensorValue[S4]<10) //적외선 센서로 장애물을 인식`했을 때
    {   
        motor[mb] = 0; motor[mc] = 0; sleep(500);
        RightTurn();
    }
}

void energy() //먹이를 찾음
{ 
    while(true){
        react();
        if(SensorValue[S2]==5 || SensorValue[S3]==5)
        {
            motor[mb]=0; motor[mc]=0; sleep(500);
            digestion();
            U_Turn();
            break;
            /*
            먹이(빨간원)의 위치 저장
            if(dest_num == 0){
                dest_num++;

                motor[mb]=0; motor[mc]=0; sleep(500);
                digestion();
                U_Turn();
                break;
            }
            else{
                for(int i=0; i<dest_num; i++){
                    if(all_destination[i][0] != all_destination[dest_num][0] && all_destination[i][1] != all_destination[dest_num][1])
                    {
                        dest_num++;
                        all_destination[dest_num][0] = destination[0];
                        all_destination[dest_num][1] = destination[1];

                        motor[mb]=0; motor[mc]=0; sleep(500);
                        digestion();
                        U_Turn();
                        break;
                    }
                }
            }
            */
        }
    }
}

void Home()
{
    while(true)
    {
        react();  
        if(SensorValue[S2]==2 || SensorValue[S3]==2)
        {
            motor[mb]=0; motor[mc]=0; sleep(500);
            digestion();
            U_Turn();
            break;
        }
    }
}

task main()
{
    playSound(soundBeepBeep);
    while(true)
    {
        energy();
        Home();
    }
}

void LeftTurn() //mode1
{
    motor[mb]=-40; motor[mc]=40; sleep(600);
    destination[0] += 1;
}

void RightTurn() //mode2
{
    motor[mb]=40; motor[mc]=-40; sleep(600);
    destination[0] -= 1;
}

void Forward() //mode3
{
    motor[mb]=30; motor[mc]=30; sleep(500);
    destination[1] += 1;
}

void Backward() //mode4
{
    motor[mb]=-30; motor[mc]=-30; sleep(500);
    destination[1] -= 1;
}

void U_Turn()
{//LeftUTurn;
    motor[mb]=-40; motor[mc]=40; sleep(1250); //180도 회전
}

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