
#include <gtest/gtest.h>
#include "utils.hpp"
#include "chebyshev_expansion.hpp"

using namespace std;

namespace {

    class TestChebyshevExpansion : public testing::Test
    {
    };

    // cppcheck-suppress syntaxError
    TEST_F(TestChebyshevExpansion, Parameters)
    {
        const size_t max_length = 8;
        const double r[max_length] = { 1, 0, -1, 0, 1, 0, -1, 0 };
        double u[max_length];

        ChebyshevExpansion ce0;
        ASSERT_FALSE(ce0(NULL, u));
        ASSERT_FALSE(ce0(nullptr, u));
        ASSERT_FALSE(ce0(r, NULL));
        ASSERT_FALSE(ce0(r, nullptr));
        ASSERT_FALSE(ce0(r, u));            // invalid when size = 0

        ChebyshevExpansion ce5(5);
        ASSERT_FALSE(ce5(NULL, u));
        ASSERT_FALSE(ce5(nullptr, u));
        ASSERT_FALSE(ce5(r, NULL));
        ASSERT_FALSE(ce5(r, nullptr));
        ASSERT_TRUE(ce5(r, u));
    }

    TEST_F(TestChebyshevExpansion, CompareResult)
    {
        double r[6] = { 1, 2, 3, 4, 5, 6 };
        double u_desired[6] = { 1, 2, 2, 2.5, 2.5, 2.875 };
        double u[6];

        ChebyshevExpansion ce(2);
        ce(r, u);

        // check all the elements
        for (int i = 0; i < 6; i++) {
            ASSERT_DOUBLE_EQ(u[i], u_desired[i]);
        }
    }
}
