
#pragma once
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

class PolynomialRoot final {
public:
    PolynomialRoot()
    : m_order(0)
    {
    }

    PolynomialRoot(const int order)
    : m_order(order)
    {
        m_companion = Eigen::MatrixXd::Zero(order, order);
        m_companion.block(1, 0, order-1, order-1).setIdentity();
    }

    void operator()(double * p, double * x)
    {
        // set polynomial coefficients to our companion matrix
        m_companion.col(m_order-1) = -Eigen::Map<Eigen::VectorXd>(p, m_order-1);

        // compute eigenvalues of the companion matrix
        e_solver.compute(m_companion, false);

        Eigen::Map<Eigen::VectorXd>(x, m_order) = e_solver.eigenvalues().real();
    }

private:
    int m_order;
    Eigen::MatrixXd m_companion;
    Eigen::EigenSolver<Eigen::MatrixXd> e_solver;
};
