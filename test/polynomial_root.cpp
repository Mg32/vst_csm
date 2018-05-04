
#include <gtest/gtest.h>
#include "utils.hpp"
#include "polynomial_root.hpp"

using namespace std;

namespace {

    class TestPolynomialRoot : public testing::Test
    {
    };

    // cppcheck-suppress syntaxError
    TEST_F(TestPolynomialRoot, Parameters)
    {
        double p[6] = { -120, 274, -225, 85, -15, 1 };
        double x[5] = { 0 };

        PolynomialRoot pr0;
        ASSERT_FALSE(pr0(NULL, x));
        ASSERT_FALSE(pr0(p, NULL));
        ASSERT_FALSE(pr0(nullptr, x));
        ASSERT_FALSE(pr0(p, nullptr));
        ASSERT_FALSE(pr0(p, x));                // invalid when size = 0

        PolynomialRoot pr5(5);
        ASSERT_FALSE(pr5(NULL, x));
        ASSERT_FALSE(pr5(p, NULL));
        ASSERT_FALSE(pr5(nullptr, x));
        ASSERT_FALSE(pr5(p, nullptr));
        ASSERT_TRUE(pr5(p, x));
    }

    TEST_F(TestPolynomialRoot, CompareResult)
    {
        const int order = 5;
        double p[order + 1] = { -120, 274, -225, 85, -15, 1 };
        double x_desired[order] = { 1, 2, 3, 4, 5 };
        double x[order] = { 0 };

        // calculate
        PolynomialRoot pr(order);
        pr(p, x);

        // check
        ASSERT_TRUE(isapprox(x, x_desired, order));
    }
}
