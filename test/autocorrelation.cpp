
#include <gtest/gtest.h>
#include "utils.hpp"
#include "autocorrelation.hpp"

using namespace std;

namespace {

    class TestAutocorr : public testing::Test
    {
    protected:
        Autocorrelation ac;
    };

    // cppcheck-suppress syntaxError
    TEST_F(TestAutocorr, Parameters)
    {
        const size_t max_length = 4;
        const double x[max_length] = { 0, 1, -1, 0 };
        double r[max_length];

        // NULL pointer is invalid
        ASSERT_FALSE(ac(NULL,    max_length, r, max_length));
        ASSERT_FALSE(ac(nullptr, max_length, r, max_length));
        ASSERT_FALSE(ac(x, max_length, NULL,    max_length));
        ASSERT_FALSE(ac(x, max_length, nullptr, max_length));

        // must be xlen >= rlen
        ASSERT_FALSE(ac(x, 1, r, 2));
        ASSERT_FALSE(ac(x, 2, r, 4));
        ASSERT_TRUE(ac(x, 2, r, 2));

        // all valid parameters
        ASSERT_TRUE(ac(x, max_length, r, max_length));
    }

    TEST_F(TestAutocorr, CompareResult)
    {
        const size_t x_length = 10, r_length = 5;
        const double x[x_length] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
        double r[r_length];
        const double   desired_r[r_length] = { 0.5, 0.4, 0.3, 0.2, 0.1 };
        const double undesired_r[r_length] = { 5.0, 4.0, 3.0, 2.0, 1.0 };

        // calculation
        ac(x, x_length, r, r_length);

        // ... and check
        ASSERT_TRUE (isapprox(r,   desired_r, r_length));
        ASSERT_FALSE(isapprox(r, undesired_r, r_length));
    }
}
