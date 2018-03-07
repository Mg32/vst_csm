
#pragma once

#include <cstddef>

class Autocorrelation {
public:
    // calculates autocorrelation coefficients r (of length_r) from signal x (of length_x).
    // returns false if the parameters are invalid.
    bool operator()(const double * x, const std::size_t length_x, double * r, const std::size_t length_r)
    {
        if (!x || !r || length_x < length_r)
            return false;

        for (std::size_t lag = 0; lag < length_r; lag++)
        {
            r[lag] = 0;
            for (std::size_t n = 0; n < length_x - lag; n++)
            {
                r[lag] += x[n] * x[n + lag];
            }
            r[lag] /= static_cast<double>(length_x);
        }

        return true;
    }
};
