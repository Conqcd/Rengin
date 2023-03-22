#include "repch.hpp"
#include "RawWriter.hpp"
#include <cassert>

namespace Rengin
{
inline auto convert_uint8_to_float(const uint8_t *source, size_t size)
    -> std::vector<float> {
    std::vector<float> data;
    data.reserve(size);

    for (size_t i = 0; i < size; i++) {
        data.push_back(source[i]);
    }
    return data;
}

inline auto convert_int16_to_float(const int16_t *source, size_t size)
    -> std::vector<float> {
    std::vector<float> data;
    data.reserve(size);

    for (size_t i = 0; i < size; i++) {
        data.push_back(source[i]);
    }
    return data;
}

inline auto convert_int16_to_uint8(const int16_t *source, size_t size,float max, float min)
    -> std::vector<uint8_t> {
    std::vector<uint8_t> data;
    data.reserve(size);

    double max_range = static_cast<double>(max) - static_cast<double>(min);
    uint8_t max_value = 255;
    for (size_t i = 0; i < size; i++) {
        double range = static_cast<double>(source[i]) - static_cast<double>(min);
        auto value = round(range / max_range * max_value);
        data.push_back(value);
    }
    return data;
}

inline auto convert_int16_to_uint8(const int16_t *source, size_t size)
    -> std::vector<uint8_t> {
    auto min = source[0];
    auto max = source[0];
    for (size_t i = 0; i < size; i++) {
        auto value = source[i];
        if (value > max) {
            max = value;
        } else if (value < min) {
            min = value;
        }
    }
    return convert_int16_to_uint8(source, size, max, min);
}

inline auto convert_uint16_to_float(const uint16_t *source, size_t size)
    -> std::vector<float> {
    std::vector<float> data;
    data.reserve(size);

    for (size_t i = 0; i < size; i++) {
        data.push_back(source[i]);
    }
    return data;
}

inline auto convert_uint16_to_uint8(const uint16_t *source, size_t size,float max, float min)
    -> std::vector<uint8_t> {
    std::vector<uint8_t> data;
    data.reserve(size);

    double max_range = static_cast<double>(max) - static_cast<double>(min);
    uint8_t max_value = 255;
    for (size_t i = 0; i < size; i++) {
        double range = static_cast<double>(source[i]) - static_cast<double>(min);
        auto value = round(range / max_range * max_value);
        data.push_back(value);
    }
    return data;
}

inline auto convert_uint16_to_uint8(const uint16_t *source, size_t size)
    -> std::vector<uint8_t> {
    auto min = source[0];
    auto max = source[0];
    for (size_t i = 0; i < size; i++) {
        auto value = source[i];
        if (value > max) {
            max = value;
        } else if (value < min) {
            min = value;
        }
    }
    return convert_uint16_to_uint8(source, size, max, min);
}

inline auto convert_float_to_uint8(const float *source, size_t size, float max,float min) -> std::vector<uint8_t> {
    std::vector<uint8_t> data;
    data.reserve(size);

    double max_range = max - min;
    uint8_t max_value = 255;
    for (size_t i = 0; i < size; i++) {
        double range = source[i] - min;
        auto value = round(range / max_range * max_value);
        data.push_back(value);
    }

    return data;
}

inline auto convert_float_to_uint8(const float *source, size_t size)
    -> std::vector<uint8_t> {
    auto min = source[0];
    auto max = source[0];
    for (size_t i = 0; i < size; i++) {
        auto value = source[i];
        if (value > max) {
            max = value;
        } else if (value < min) {
            min = value;
        }
    }
    return convert_float_to_uint8(source, size, max, min);
}

inline auto get_file_name(const std::string &filepath) -> std::string {
    size_t begin = 0;
    size_t end = filepath.size();
    auto ext_idx = filepath.find_last_of('.');
    if (ext_idx != std::string::npos) {
        end = ext_idx;
    }
    auto slash_idx = filepath.find_last_of('/');
    if (slash_idx != std::string::npos) {
        begin = slash_idx + 1;
    }
    return filepath.substr(begin, end - begin);
}

inline auto split(const std::string &s, const std::string &delimiter = " ")
    -> std::vector<std::string> {
    std::vector<std::string> tokens;
    auto last_pos = s.find_first_not_of(delimiter, 0);
    auto pos = s.find_first_of(delimiter, last_pos);
    while (pos != std::string::npos || last_pos != std::string::npos) {
        tokens.push_back(s.substr(last_pos, pos - last_pos));
        last_pos = s.find_first_not_of(delimiter, pos);
        pos = s.find_first_of(delimiter, last_pos);
    }
    return tokens;
}

RawWriter::RawWriter(const std::vector<float> &data,uint32_t d,uint32_t h,uint32_t w) 
        :m_buffer(data)
{
    dimensions[0] = d;
    dimensions[1] = h;
    dimensions[2] = w;
}


void RawWriter::write(const std::string& file)
{
    std::string filename = file;
    for (int i = 0; i < 3; i++) {
        filename +=  "_" + std::to_string(dimensions[i]);
    }
    filename += "_float.raw";
    std::ofstream fs;
    fs.open(filename, std::ios::binary);
    if (!fs.is_open()) {
        throw std::runtime_error(std::string("cannot load file: ") + filename);
    }
    fs.write(reinterpret_cast<char *>(m_buffer.data()),m_buffer.size() * sizeof(float));
    fs.close();
}
} // namespace Rengin