
#pragma once

#include <vector>
#include <cmath>

typedef struct Sinusoid {
    Sinusoid()
    : m(0)
    , frequency(0)
    {
    }

    double m;
    double frequency;
} sinusoid_t;

typedef std::vector<sinusoid_t> sinusoid_frame_t;
typedef std::vector<sinusoid_frame_t> sinusoid_frame_list_t;

class CompositeSinusoid {
public:
    CompositeSinusoid()
    {
    }

    CompositeSinusoid(std::size_t num_sines)
    {
        set_num_sines(num_sines);
    }

    CompositeSinusoid(sinusoid_frame_t csm)
    : m_sines(csm)
    {
    }

    CompositeSinusoid(std::size_t num_sines, const double * m, const double * freq)
    {
        set_num_sines(num_sines);

        for (std::size_t i = 0; i < num_sines; i++)
        {
            set_at(i, m[i], freq[i]);
        }
    }

    void set_num_sines(std::size_t num_sines)
    {
        m_sines.resize(num_sines);
    }

    void set_at(int index, sinusoid_t sine)
    {
        m_sines[index] = sine;
    }

    void set_at(int index, double m, double frequency)
    {
        m_sines[index].m = m;
        m_sines[index].frequency = frequency;
    }

    double m(int index)
    {
        return m_sines[index].m;
    }

    double frequency(int index)
    {
        return m_sines[index].frequency;
    }

    void sines(std::vector<sinusoid_t> sines)
    {
        m_sines = sines;
    }

    std::vector<sinusoid_t> get_sines()
    {
        return m_sines;
    }

    double generate(const double t)
    {
        double out = 0.0;

        for (auto iter = m_sines.begin(); iter != m_sines.end(); iter++)
        {
            auto & s = *iter;
            double amp = std::sqrt(2.0 * s.m);
            double omega = 2.0 * M_PI * s.frequency;
            out += amp * std::sin(omega * t);
        }

        return out;
    }

    double generate(const int frame, const double fs)
    {
        return generate((double)frame / fs);
    }

    void generate_to_array(double * x, const std::size_t length, const double fs)
    {
        for (std::size_t i = 0; i < length; i++)
        {
            x[i] = generate(i, fs);
        }
    }

private:
    std::vector<sinusoid_t> m_sines;
};
