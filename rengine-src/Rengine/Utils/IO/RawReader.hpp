#pragma once
#include "Rengine/Core/core.hpp"
#include <fstream>
#include <vector>

namespace Rengin
{
enum class RawValueType : uint8_t {
    FLOAT = 0,
    DOUBLE = 1,
    UINT8 = 2,
    UINT16 = 3,
    UINT32 = 4,
    UINT64 = 5,
    INT8 = 6,
    INT16 = 7,
    INT32 = 8,
    INT64 = 9,
};

struct RawReader{
    std::ifstream fs;
    std::array<uint32_t, 3> dimensions;
    RawValueType value_type;
    std::vector<float> m_buffer;

    explicit RawReader(const std::string &filepath);
    auto load() -> std::vector<float>&;
    inline uint64_t Count() const
    {
        return dimensions[0] * dimensions[1] * dimensions[2];
    }
};


} // namespace Rengin