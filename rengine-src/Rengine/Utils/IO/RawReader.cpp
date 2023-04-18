#include "repch.hpp"
#include "RawReader.hpp"
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

RawReader::RawReader(const std::string &filepath) {
    const char *name_rule = "raw data should be named as `name_x_y_z_datatype`, "
                            "for example: isabel_500_500_100_uint8";
    auto filename = get_file_name(filepath);
    auto splited = split(filename, "_");
    if (splited.size() < 5) {
        throw std::runtime_error(name_rule);
    }

    fs.open(filepath, std::ios::binary);
    fs.unsetf(std::ios::skipws);
    if (!fs.is_open()) {
        throw std::runtime_error(std::string("cannot load file: ") + filepath);
    }

    for (int i = 0; i < 3; i++) {
        auto &dim = splited[splited.size() - 4 + i];
        dimensions[i] = strtoul(dim.c_str(), nullptr, 10);
        if (dimensions[i] == 0) {
        throw std::runtime_error(name_rule);
        }
    }

    auto &type = splited[splited.size() - 1];
    if (type == "uint8") {
        value_type = RawValueType::UINT8;
    } else if (type == "uint16") {
        value_type = RawValueType::UINT16;
    } else if (type == "int16") {
        value_type = RawValueType::INT16;
    } else if (type == "float") {
        value_type = RawValueType::FLOAT;
    } else {
        throw std::runtime_error(std::string("unsupported value type: ") + type);
    }
}

auto RawReader::load() -> std::vector<float>& {
    if (value_type == RawValueType::UINT8) {
        std::vector<uint8_t> buffer;
        buffer.resize(Count());
        fs.read(reinterpret_cast<char *>(buffer.data()), Count() * sizeof(uint8_t));
        m_buffer = convert_uint8_to_float(buffer.data(), Count());
    }else if (value_type == RawValueType::UINT16) {
        std::vector<uint16_t> buffer;
        buffer.resize(Count());
        fs.read(reinterpret_cast<char *>(buffer.data()), Count() * sizeof(uint16_t));
        m_buffer = convert_uint16_to_float(buffer.data(), Count());
    }else if (value_type == RawValueType::INT16) {
        std::vector<int16_t> buffer;
        buffer.resize(Count());
        fs.read(reinterpret_cast<char *>(buffer.data()), Count() * sizeof(int16_t));
        m_buffer = convert_int16_to_float(buffer.data(), Count());
    }else if (value_type == RawValueType::FLOAT) {
        m_buffer.resize(Count());
        fs.read(reinterpret_cast<char *>(m_buffer.data()), Count() * sizeof(float));
        // m_buffer.insert(m_buffer.begin(),std::istream_iterator<float>(fs),std::istream_iterator<float>());
    } else {
        throw std::runtime_error(std::string("unsupported value type"));
    }
    return m_buffer;
}

} // namespace Rengin