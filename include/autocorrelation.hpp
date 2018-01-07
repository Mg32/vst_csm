
#pragma once

class Autocorrelation {
public:
    void operator()(const double * x, const int length_x, double * r, const int length_r)
    {
        for (int lag = 0; lag < length_r; lag++)
        {
            r[lag] = 0;
            for (int n = 0; n < length_x - lag; n++)
            {
                r[lag] += x[n] * x[n + lag];
            }
            r[lag] /= (double)length_x;
        }
        return;
    }
};