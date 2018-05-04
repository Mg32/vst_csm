
#pragma once
#include <cmath>
#include <Eigen/Dense>

class ChebyshevExpansion final {
public:
    ChebyshevExpansion()
    : m_size(0)
    {
    }

    explicit ChebyshevExpansion(const int n)
    {
        createTable(n);
    }

    void createTable(const int n)
    {
        if (n <= 0)
        {
            m_size = 0;
            return;
        }

        m_size = 2 * n + 2;
        m_table = Eigen::MatrixXd::Zero(m_size, m_size);

        // combination
        for (int l = 0; l < m_size; l++)
        {
            m_table(l, 0) = 1;
            for (int k = 1; k < l; k++)
            {
                m_table(l, k) = m_table(l-1, k) + m_table(l-1, k-1);
            }
            m_table(l, l) = 1;
        }

        // normalize
        for (int l = 0; l < m_size; l++)
        {
            double m = std::pow(2, l);
            for (int k = 0; k <= l; k++)
            {
                m_table(l, k) /= m;
            }
        }
    }

    bool operator()(const double * r, double * u)
    {
        if (m_size == 0 || !r || !u)
            return false;

        for (int l = 0; l < m_size; l++)
        {
            u[l] = 0;
            for (int k = 0; k <= l; k++)
            {
                u[l] += m_table(l, k) * r[abs(2 * k - l)];
            }
        }

        return true;
    }

private:
    int m_size;
    Eigen::MatrixXd m_table;
};
