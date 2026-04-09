# pragma once

#include "Calculator.h"

class distanceCalculator : public Calculator
{
private:
    double Distance = 0.0f;

    double CurrentSpeed = 0.0f;

    void Calculate(int Value) override;

    void RunStart(double SpeedValue) override;

    double DistanceCalculate(int UpdateCycle);

public:

    inline double GetDistance() const {return Distance;}
    inline void SetCurrentSpeed(double Value) {CurrentSpeed = Value;}

};
