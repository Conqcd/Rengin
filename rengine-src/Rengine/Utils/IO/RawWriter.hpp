#pragma once
#include "Rengine/Core/core.hpp"
#include <fstream>
#include <vector>
#include "RawReader.hpp"

namespace Rengin
{
struct RawWriter{
    std::ofstream fs;
    std::array<uint32_t, 3> dimensions;
    RawValueType value_type;
    std::vector<float> m_buffer;

    explicit RawWriter(const std::vector<float> &filepath,uint32_t w,uint32_t h,uint32_t d);
    void write(const std::string&);
    inline uint64_t Count() const
    {
        return dimensions[0] * dimensions[1] * dimensions[2];
    }
};


} // namespace Rengin