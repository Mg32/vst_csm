
#pragma once

#include <iostream>
#include "autocorrelation.hpp"
#include "csm_analyzer.hpp"
#include "csm_frame_data.hpp"

class CsmFrameAnalyzer {
public:
    CsmFrameAnalyzer()
    : m_order(0)
    , m_fs(0)
    , m_interval(0)
    , m_buffer_size(0)
    , m_r_size(0)
    , m_buffer(nullptr)
    , m_r(nullptr)
    , m_offset(0)
    {
    }

    CsmFrameAnalyzer(int order, double fs, double interval)
    : m_order(order)
    , m_fs(fs)
    , m_interval(interval)
    , m_buffer_size((unsigned)(fs * interval))
    , m_r_size((unsigned)(2 * order + 2))
    , m_offset(0)
    {
        m_buffer = new double[m_buffer_size];
        m_r = new double[m_r_size];
        m_csm_analyzer = CsmAnalyzer(m_order);
    }

    ~CsmFrameAnalyzer()
    {
        delete[] m_r;
    }

    void reset()
    {
        m_offset = 0;
    }

    CsmFrameData get_csm_frames()
    {
        m_frames.fs = m_fs;
        m_frames.interval = m_interval;
        return m_frames;
    }

    void next_array(const double * x, std::size_t length)
    {
        for (std::size_t i = 0; i < length; i++)
        {
            next_sample(x[i]);
        }
    }

    void next_sample(double x)
    {
        if (m_offset < m_buffer_size) {
            m_buffer[m_offset] = x;
            m_offset++;
            return;
        }

        // when m_buffer become full, then analyze it
        analyze_frame(m_buffer, m_buffer_size);
        m_offset = 0;
    }

protected:
    void analyze_frame(const double * x, std::size_t length)
    {
        // autocorrelation
        m_autocorr(x, length, m_r, m_r_size);

        if (m_r[0] < 1e-6)
        {
            // add an empty frame if RMS is almost zero
            CompositeSinusoid cs(0);
            m_frames.add(cs.get_sines());
            return;
        }

        // csm analyze
        CompositeSinusoid * cs = m_csm_analyzer(m_r, m_fs);

        // add a frame
        m_frames.add(cs->get_sines());
        delete cs;
    }

private:
    int m_order;
    double m_fs;
    double m_interval;
    unsigned m_buffer_size;
    unsigned m_r_size;
    double * m_buffer;
    double * m_r;
    unsigned m_offset;
    Autocorrelation m_autocorr;
    CsmAnalyzer m_csm_analyzer;
    CsmFrameData m_frames;
};
