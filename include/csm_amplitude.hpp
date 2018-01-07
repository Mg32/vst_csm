
#pragma once
#include <Eigen/Dense>

class CsmAmplitude {
public:
    CsmAmplitude()
    : m_order(0)
    {
    }

    CsmAmplitude(int order)
    : m_order(order)
    {
    }

    Eigen::MatrixXd create_vandermonde(double * x)
    {
        Eigen::MatrixXd xx = Eigen::Map<Eigen::MatrixXd>(x, 1, m_order);
        Eigen::MatrixXd vandermonde = xx.replicate(m_order, 1);

        for (int i = 0; i < m_order; i++)
            vandermonde.row(i) = vandermonde.row(i).array().pow(i);

        return vandermonde;
    }

    void operator()(double * x, double * u, double * m)
    {
        Eigen::VectorXd vec_u = Eigen::Map<Eigen::VectorXd>(u, m_order, 1);
        Eigen::MatrixXd inv_x = create_vandermonde(x).inverse();
        Eigen::Map<Eigen::VectorXd>(m, m_order) = inv_x * vec_u;
    }

private:
    int m_order;
};
