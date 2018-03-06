
#pragma once

#include <iostream>
#include <memory>
#include "autocorrelation.hpp"
#include "csm_analyzer.hpp"
#include "csm_synthesizer.hpp"

class CsmSampleHandler final {
public:
	CsmSampleHandler()
		: m_order(0)
		, m_fs(0)
		, m_buffer_size(0)
		, m_r_size(0)
		, m_offset(0)
		, m_buffer_i(nullptr)
		, m_buffer_o(nullptr)
		, m_r(nullptr)
		, m_csm_analyzer(nullptr)
		, m_csm_synth(nullptr)
	{
	}

	CsmSampleHandler(int order, double fs, double interval)
		: m_order(order)
		, m_fs(fs)
		, m_buffer_size(static_cast<unsigned>(fs * interval))
		, m_r_size(static_cast<unsigned>(2 * order + 2))
		, m_offset(0)
		, m_csm_analyzer(new CsmAnalyzer(order))
		, m_csm_synth(new CsmSynthesizer(fs, interval))
	{
		m_buffer_i = new double[m_buffer_size];
		m_buffer_o = new double[m_buffer_size];
		m_r = new double[m_r_size];

		for (size_t i = 0; i < m_buffer_size; i++)
		{
			m_buffer_i[i] = 0.0;
			m_buffer_o[i] = 0.0;
		}
		for (size_t i = 0; i < m_r_size; i++)
		{
			m_r[i] = 0.0;
		}
		m_r[0] = 1.0;
	}

	~CsmSampleHandler()
	{
		delete[] m_buffer_i;
		delete[] m_buffer_o;
		delete[] m_r;
	}

	void reset()
	{
		m_offset = 0;
	}

	double operator()(double x)
	{
		if (m_offset < m_buffer_size) {
			m_buffer_i[m_offset] = x;
			m_offset++;
			goto ret;
		}

		// analyze m_buffer when it becomes full
		analyze_frame(m_buffer_i, m_buffer_size);
		m_offset = 0;

	ret:
		return m_buffer_o[m_offset];
	}

private:
	int m_order;
	double m_fs;
	unsigned m_buffer_size;
	unsigned m_r_size;
	unsigned m_offset;
	double * m_buffer_i;
	double * m_buffer_o;
	double * m_r;
	std::unique_ptr<CsmAnalyzer> m_csm_analyzer;
	std::unique_ptr<CsmSynthesizer> m_csm_synth;
	Autocorrelation m_autocorr;

	void analyze_frame(const double * x, std::size_t length)
	{
		// autocorrelation
		m_autocorr(x, length, m_r, m_r_size);

		// csm analysis
		CompositeSinusoid * cs = (*m_csm_analyzer)(m_r, m_fs);

		// csm synthesis
		(*m_csm_synth)(m_buffer_o, cs->get_sines());

		delete cs;
	}

};
