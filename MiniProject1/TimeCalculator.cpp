#include "timeCalculator.h"

void timeCalculator::Calculate(int Value)
{
    RunTime += (double)Value/1000;
}

void timeCalculator::RunStart(double SpeedValue)
{
    RunTime = 0.0f;
}