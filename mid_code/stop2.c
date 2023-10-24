task main()
{
    if(SenSorValue[S2]<10)
    {
        wait1msec(1000);
        if (SenSorValue[S2]<10)
        {
            motor[motorB] = 0;
            motor[motorC] = 0;
        }
    }
}