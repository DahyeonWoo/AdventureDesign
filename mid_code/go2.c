int LR;  //left rotate
int RR; //rigth rotate
int threshold = 45; //기준값
int base = 20; //기본 동작
float rotation;
float r =0.6;

task main()
{
    while(1) {
        rotation = r * (threshold - SenSorValue[S2]);

        LR = base + rotation;
        RR = base - rotation;

        motor[motorB] = LR;
        motor[motorC] = RR - 10;
    }
}