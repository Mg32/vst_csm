
#pragma once

#include <cmath>

class WindowFunction {
public:
    virtual double operator()(double x)
    {
        (void)x;
        return 0;
    }
};

class HanningWindow : public WindowFunction {
public:
    virtual double operator()(double x)
    {
        if (x > 1 || x < 0) return 0;
        return 0.5 - 0.5 * std::cos(2 * M_PI * x);
    }
};
