#pragma once

class Calculator{

public:
    virtual void Calculate(int Value) = 0;

    virtual void RunStart(double SpeedValue) = 0;
};
