
#pragma once

#include <cstddef>
#include <cmath>

namespace {
    static inline bool isapprox(const double a, const double b, const double eps = 1e-8)
    {
        return std::fabs(a - b) < eps;
    }

    static inline bool isapprox(const double * a, const double * b, const std::size_t len, const double eps = 1e-8)
    {
        for (std::size_t i = 0; i < len; i++)
        {
            if (!isapprox(a[i], b[i], eps)) return false;
        }
        return true;
    }
}
