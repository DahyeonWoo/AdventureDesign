task main()
{
    if(SenSorValue[S2]>95)
    {
        wait1msec(1500);
        if (SenSorValue[S2]>95)
        {
            motor[motorB] = 0;
            motor[motorC] = 0;
        }
    }
}