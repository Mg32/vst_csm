
#pragma once
#include <Eigen/Dense>

class HankelEquation final {
public:
    HankelEquation()
    : m_order(0)
    {
    }

    HankelEquation(const int order)
    : m_order(order)
    {
        m_polynomial = Eigen::MatrixXd::Zero(order + 1, order + 1);
    }

    ~HankelEquation()
    {
    }

    void operator()(const double * u, double * p)
    {
        if (m_order <= 0) { return; }

        double q_sum = u[1] / u[0];
        double q = q_sum;

        double r_prod = 1;
        double r = r_prod;

        // m_polynomial(order, iter)
        // P0(x) = 1
        m_polynomial(0, 0) = 1;
        // P1(x) = x - q_1
        m_polynomial(0, 1) = 1;
        m_polynomial(1, 1) = -q;

        for (int k = 2; k <= m_order; k++) {
            double q_sum_prev  = q_sum;
            double r_prod_prev = r_prod;

            double qs1 = 0;
            double qs2 = 0;
            for (int i = 0; i < k; i++) {
                qs1 += m_polynomial(i, k-1) * u[2*k-i-1];
                qs2 += m_polynomial(i, k-1) * u[2*k-i-2];
            }
            q_sum  = qs1 / qs2;
            r_prod = qs2 / u[0];

            q = q_sum - q_sum_prev;
            r = r_prod / r_prod_prev;

            for (int i = 0; i <= k; i++) {
                // p1 = [p[0:k-1, k-1]; 0]
                // p2 = [0; p[0:k-1, k-1]]
                // p3 = [0; 0; p[0:k-2, k-2]]
                double index_p1 = i;
                double index_p2 = i - 1;
                double index_p3 = i - 2;
                double p1 = (i < k) ? m_polynomial(index_p1, k-1) : 0;
                double p2 = (i < 1) ? 0 : m_polynomial(index_p2, k-1);
                double p3 = (i < 2) ? 0 : m_polynomial(index_p3, k-2);

                // p[0:k, k] = p1 .- q[k] * p2 .- r[k-1] * p3
                double p_ik = p1 - q * p2 - r * p3;
                m_polynomial(i, k) = p_ik;
                p[m_order-i] = p_ik;
            }
        }

        return;
    }

private:
    int m_order;
    Eigen::MatrixXd m_polynomial;
};
