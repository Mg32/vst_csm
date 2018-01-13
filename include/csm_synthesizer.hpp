
#pragma once

#include "csm_frame_data.hpp"
#include "window_function.hpp"

class CsmSynthesizer {
public:
	CsmSynthesizer()
		: m_fs(0)
		, m_interval(1)
		, m_prev(nullptr)
		, m_next(nullptr)
		, m_window(nullptr)
    {
    }

	CsmSynthesizer(double fs, double interval)
		: m_fs(fs)
		, m_interval(interval)
    {
        unsigned bufsize = buffer_size();

        m_prev = new double[bufsize * 2];
        m_next = new double[bufsize * 2];
        m_window = new double[bufsize];

        for (unsigned i = 0; i < bufsize; i++)
        {
            m_prev[i + bufsize] = 0;
            m_window[i] = m_hann(0.5 * (1.0 + (double)i / (bufsize - 1)));
        }
    }

    ~CsmSynthesizer()
    {
        delete[] m_prev;
        delete[] m_next;
        delete[] m_window;
    }

    void operator()(double * output, const sinusoid_frame_t & frame)
    {
		unsigned bufsize = buffer_size();

		CompositeSinusoid cs(frame);
		cs.generate_to_array(m_next, bufsize * 2, m_fs);

		for (unsigned i = 0; i < bufsize; i++)
		{
			double w1 = m_window[i];
			double w2 = 1.0 - w1;
			double x1 = m_prev[i + bufsize];
			double x2 = m_next[i];
			output[i] = (w1 * x1) + (w2 * x2);

			// simultaneously copy
			m_prev[i] = m_next[i];
			m_prev[i + bufsize] = m_next[i + bufsize];
		}
    }

protected:
	double m_fs;
	double m_interval;
	double * m_prev;
	double * m_next;
	double * m_window;
	HanningWindow m_hann;

	unsigned buffer_size()
	{
		return (unsigned)(m_fs * m_interval);
	}

};
