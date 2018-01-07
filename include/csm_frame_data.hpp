
#pragma once

#include <vector>
#include "composite_sinusoid.hpp"

struct CsmFrameData {
    CsmFrameData()
    : fs(0)
    , interval(1)
    {
    }

    CsmFrameData(double t_fs, double t_interval)
    : fs(t_fs)
    , interval(t_interval)
    {
    }

    void add(std::vector<sinusoid_t> s)
    {
        sine_array.push_back(s);
    }

    double fs;
    double interval;
    sinusoid_frame_list_t sine_array;
};
