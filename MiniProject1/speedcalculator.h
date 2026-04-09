#pragma once

#include "Calculator.h"

class speedCalculator : public Calculator
{
private:
    double Speed = 0.0f;
    double InitSpeed = 4.0f;
    double AverageSpeed = 0.0f;

    void Calculate(int Value) override;
    void RunStart(double SpeedValue) override;

public:
    void SpeedUp();
    void SpeedDown();
    inline void SetSpeed(double Value) {Speed = Value;}

    inline double GetSpeed() const {return Speed;}

    double AvrSpeedCalulate(double Distance, double RunTime);
};
