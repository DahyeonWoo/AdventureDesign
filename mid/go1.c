task main()
{
    while(1)
    {
        writeDebugStreamLine("%d", SenSorValue[S2]);

        if(SensorValue[S2]>80) // 80~100
        {
            moter[moterB]=50;
            motor[motorC]=-20;
        }
        else if (SensorValue[S2]>70) //80~70
        {
            moter[moterB]=45;
            motor[motorC]=-15;
        }
        else if(SenSorValue[S2]>30) //70~30
        {
            motor[motorB]=-20;
            motor[motorC]=50;
        }
    }
}