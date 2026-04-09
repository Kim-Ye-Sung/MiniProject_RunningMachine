#pragma once

#include "Calculator.h"

class timeCalculator : public Calculator
{
private:
    double RunTime = 0.0f;

    void Calculate(int Value) override;

    void RunStart(double SpeedValue) override;


public:
    inline double GetRunTime() const {return RunTime;};
};


