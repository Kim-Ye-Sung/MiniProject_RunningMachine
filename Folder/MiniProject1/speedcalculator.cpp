#include "speedcalculator.h"

void speedCalculator::SpeedUp()
{
    if(Speed == 15.0f)        // 최대 15까지만 속도 가능
    {
        return;
    }
    else if(Speed +0.1f >=15.0f)
    {
        Speed= 15.0f;
    }
    else
    {
        Speed += 0.1f;
    }

}

void speedCalculator::SpeedDown()
{
    if(Speed == 0.0f)  // 최소 0 밑으로는 떨어질수 없음
    {
        return;
    }
    else if(Speed - 0.1f <= 0.0f)
    {
        Speed = 0.0f;
    }
    else
    {
        Speed -= 0.1f;
    }
}

void speedCalculator::RunStart(double SpeedValue)
{
    Speed = InitSpeed;
}

void speedCalculator::Calculate(int Value)
{

}

double speedCalculator::AvrSpeedCalulate(double Distance, double RunTime)
{
    AverageSpeed = 0.0f;

    if (RunTime > 0.0f)
    {
        AverageSpeed = Distance / (RunTime / 3600.0f);
    }

    return AverageSpeed;
}