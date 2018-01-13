
#pragma once
#include "composite_sinusoid.hpp"
#include "chebyshev_expansion.hpp"
#include "hankel_equation.hpp"
#include "polynomial_root.hpp"
#include "csm_amplitude.hpp"

#include <iostream>
#include <cmath>
#include <Eigen/Dense>

class CsmAnalyzer final {
public:
    CsmAnalyzer()
		: m_order(0)
		, m_chebyshev(nullptr)
		, m_polynomial(nullptr)
		, m_roots(nullptr)
		, m_csm_freq(nullptr)
		, m_csm_amp(nullptr)
    {
    }

    CsmAnalyzer(const int order)
		: m_order(order)
		, m_chebyshev(new double[2 * (order + 1)])
		, m_polynomial(new double[order + 1])
		, m_roots(new double[order])
		, m_csm_freq(new double[order])
		, m_csm_amp(new double[order])
    {
        m_chebyshev_expansion = ChebyshevExpansion(m_order);
        m_hankel_equation = HankelEquation(m_order);
        m_polynomial_root = PolynomialRoot(m_order);
        m_csm_amplitude = CsmAmplitude(m_order);
    }

    ~CsmAnalyzer()
    {
        delete[] m_chebyshev;
        delete[] m_polynomial;
        delete[] m_roots;
        delete[] m_csm_freq;
        delete[] m_csm_amp;
    }

    CompositeSinusoid * operator()(const double * r, const double fs)
    {
		if (r[0] < kAutocorrThreshold)
		{
			// return an empty frame if RMS is almost zero
			return new CompositeSinusoid(0);
		}

        // chebyshev expansion
        m_chebyshev_expansion(r, m_chebyshev);

        // solve hankel matrix equation
        m_hankel_equation(m_chebyshev, m_polynomial);

        // compute roots of the polynomial
        m_polynomial_root(m_polynomial, m_roots);

        // CSM frequency
        for (int i = 0; i < m_order; i++) {
            m_csm_freq[i] = (fs / 2.0) * std::acos(m_roots[i]) / M_PI;
        }

        // CSM amplitude
        m_csm_amplitude(m_roots, m_chebyshev, m_csm_amp);

        return new CompositeSinusoid(m_order, m_csm_amp, m_csm_freq);
    }

private:
	const double kAutocorrThreshold = 1e-6;

    int m_order;
    double * m_chebyshev;
    double * m_polynomial;
    double * m_roots;
    double * m_csm_freq;
    double * m_csm_amp;
    ChebyshevExpansion m_chebyshev_expansion;
    HankelEquation m_hankel_equation;
    PolynomialRoot m_polynomial_root;
    CsmAmplitude m_csm_amplitude;
};
