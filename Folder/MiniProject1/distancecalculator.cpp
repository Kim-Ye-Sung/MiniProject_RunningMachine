#include "distancecalculator.h"

void distanceCalculator::Calculate(int Value)
{
    Distance += DistanceCalculate(Value);
}

void distanceCalculator::RunStart(double SpeedValue)
{
    Distance = 0.0f;

    CurrentSpeed= SpeedValue;
}


double distanceCalculator::DistanceCalculate(int UpdateCycle)
{
    double DistanceInInterval = 0.0f;

    DistanceInInterval = CurrentSpeed * (((double)UpdateCycle / 1000) / 3600);

    return DistanceInInterval;
}