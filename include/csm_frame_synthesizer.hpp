
#pragma once

#include "csm_frame_data.hpp"
#include "window_function.hpp"

class CsmFrameSynthesizer {
public:
    CsmFrameSynthesizer()
    : m_buffer_size(0)
    , m_prev(nullptr)
    , m_next(nullptr)
    , m_window(nullptr)
    {
    }

    CsmFrameSynthesizer(CsmFrameData frames)
    : m_frames(frames)
    {
        m_buffer_size = (unsigned)(m_frames.fs * m_frames.interval);

        m_prev = new double[m_buffer_size * 2];
        m_next = new double[m_buffer_size * 2];
        m_window = new double[m_buffer_size];

        for (unsigned i = 0; i < m_buffer_size; i++)
        {
            m_prev[i + m_buffer_size] = 0;
            m_window[i] = m_hann(0.5 * (1.0 + (double)i / (m_buffer_size-1)));
        }
    }

    ~CsmFrameSynthesizer()
    {
        delete[] m_prev;
        delete[] m_next;
        delete[] m_window;
    }

    std::size_t get_output_length()
    {
        return m_frames.sine_array.size() * m_buffer_size + 1;
    }

    void operator()(double * output)
    {
        auto & arr = m_frames.sine_array;
        unsigned size = arr.size();
        for (unsigned i = 0; i < size; i++) {
            synthesize(arr[i], output + (i * m_buffer_size));
        }
    }

protected:
    void synthesize(const sinusoid_frame_t & frame, double * output)
    {
        CompositeSinusoid cs(frame);
        cs.generate_to_array(m_next, m_buffer_size * 2, m_frames.fs);

        for (unsigned i = 0; i < m_buffer_size; i++)
        {
            double w1 = m_window[i];
            double w2 = 1.0 - w1;
            double x1 = m_prev[i + m_buffer_size];
            double x2 = m_next[i];
            output[i] = (w1 * x1) + (w2 * x2);

            // simultaneously copy
            m_prev[i] = m_next[i];
            m_prev[i + m_buffer_size] = m_next[i + m_buffer_size];
        }
    }

private:
    CsmFrameData m_frames;
    unsigned m_buffer_size;
    double * m_prev;
    double * m_next;
    double * m_window;
    HanningWindow m_hann;
};
