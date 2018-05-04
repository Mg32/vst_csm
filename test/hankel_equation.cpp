
#include <gtest/gtest.h>
#include "utils.hpp"
#include "hankel_equation.hpp"

using namespace std;

namespace {

    class TestHankelEquation : public testing::Test
    {
    };

    // cppcheck-suppress syntaxError
    TEST_F(TestHankelEquation, Parameters)
    {
        double u[6] = { 1, 2, 2, 2.5, 2.5, 2.875 };
        double p[3] = { 0 };

        HankelEquation he0;
        ASSERT_FALSE(he0(NULL, p));
        ASSERT_FALSE(he0(u, NULL));
        ASSERT_FALSE(he0(nullptr, p));
        ASSERT_FALSE(he0(u, nullptr));
        ASSERT_FALSE(he0(u, p));            // invalid when size = 0

        HankelEquation he2(2);
        ASSERT_FALSE(he2(NULL, p));
        ASSERT_FALSE(he2(u, NULL));
        ASSERT_FALSE(he2(nullptr, p));
        ASSERT_FALSE(he2(u, nullptr));
        ASSERT_TRUE(he2(u, p));
    }

    TEST_F(TestHankelEquation, CompareResult)
    {
        double u[6] = { 1, 2, 2, 2.5, 2.5, 2.875 };
        double p_desired[3] = { -0.5, -0.75, 1 };
        double p[3] = { 0 };

        // calculate
        HankelEquation he(2);
        he(u, p);

        // check all the elements
        ASSERT_TRUE(isapprox(p, p_desired, 3));
    }
}
