#pragma once

#include "Calculator.h"

class calorieCalculator : public Calculator
{
private:
    double Calorie = 0.0f;

    double CurrentSpeed = 0.0f;

    void Calculate(int Value) override;

    void RunStart(double SpeedValue) override;

    double KcalCalculate(int UpdateCycle);

public:
    inline double GetCalorie() const {return Calorie;}
    inline void SetCurrentSpeed(double Value) {CurrentSpeed = Value;}
};
