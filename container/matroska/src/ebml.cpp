//
// Created by Charles on 2017/11/15.
//

#include "ebml.hpp"

#include "util/endian_util.hpp"

using namespace std;
using namespace player;
using namespace player::container::matroska;


constexpr uint32_t ReadBufferSize = 64;

static int64_t read_string(istream &stream, uint64_t size, std::string &output)
{
    char tmp[ReadBufferSize];
    while (stream && size > ReadBufferSize) {
        stream.read(tmp, ReadBufferSize);
        auto count = stream.gcount();
        output.append(tmp, static_cast<string::size_type>(count));

        if (count < ReadBufferSize) {
            return size - count;
        }
        size -= ReadBufferSize;
    }

    if (size > ReadBufferSize) {
        return size;
    } else if (size != 0) {
        stream.read(tmp, size);
        auto count = stream.gcount();
        output.append(tmp, static_cast<string::size_type>(count));

        return size - count;
    }

    return 0;
}

inline static int64_t read_data(istream &stream, uint64_t size, char *data)
{
    stream.read(data, size);

    return size - stream.gcount();
}

inline static unsigned get_prefix_zero_count(unsigned char c, int max_count)
{
    while (c > 0) {
        c >>= 1;
        --max_count;
    }

    return static_cast<unsigned>(max_count);
}


int32_t ebml_parser::parse_next(ebml_node &result)
{
    auto pre = stream.peek();
    auto size = get_prefix_zero_count(static_cast<unsigned char>((pre >> 4) & 0x0f), 4) + 1;  // id element size
    if (size > 4) {
        return size;
    }

    char data[8] = {0};
    stream.read(data, size);
    if (size != stream.gcount()) {
        return -1;
    }
    result.id = static_cast<uint32_t>(util::from_big_endian_variable_unsigned(data, size));

    pre = stream.peek();
    if (!stream) {
        return -1;
    }
    size = get_prefix_zero_count(static_cast<unsigned char>(pre & 0xff), 8) + 1;    // length element size
    if (size > 8) {
        return size;
    }

    stream.read(data, size);
    if (size != stream.gcount()) {
        return -1;
    }

    // set the first "1" bit to zero.
    auto c = static_cast<unsigned char>(data[0] << size);
    data[0] = c >> size;
    result.size = util::from_big_endian_variable_unsigned(data, size);

    result.position = stream.tellg();

    return 0;
}

int32_t ebml_parser::read_integer(size_t size, int64_t &result)
{
    assert(size <= 8);

    char data[8];
    auto r = read_data(stream, size, data);
    if (r) {
        return static_cast<int32_t>(r);
    }

    result = util::from_big_endian_variable(data, static_cast<unsigned int>(size));

    return 0;
}

int32_t ebml_parser::read_unsigned_integer(size_t size, uint64_t &result)
{
    assert(size <= 8);

    char data[8];
    auto r = read_data(stream, size, data);
    if (r) {
        return static_cast<int32_t>(r);
    }

    result = util::from_big_endian_variable_unsigned(data, static_cast<unsigned int>(size));

    return 0;
}

int32_t ebml_parser::read_float(size_t size, double &result)
{
    assert(size == 4 || size == 8);

    char data[8];
    auto r = read_data(stream, size, data);
    if (r) {
        return static_cast<int32_t>(r);
    }

    if (4 == size) {
        result = util::from_big_endian_float(data);
    } else {
        result = util::from_big_endian_double(data);
    }

    return 0;
}

int32_t ebml_parser::read_string(size_t size, std::string &result)
{
    return static_cast<int32_t>(::read_string(stream, size, result));
}

int32_t ebml_parser::read_date(int64_t &result)
{
    // assert(node.size == 8);

    char data[8];
    auto r = read_data(stream, 8, data);
    if (r) {
        return static_cast<int32_t>(r);
    }

    result = util::from_big_endian(data);

    return 0;
}

int32_t ebml_parser::read_binary(size_t size, std::vector<uint8_t> &result)
{
    result.resize(size);

    uint64_t i = 0;
    char c;
    while (stream.get(c) && i < size) {
        result[i++] = static_cast<unsigned char>(c);
    }

    return static_cast<int32_t>(size - i);
}

int32_t ebml_parser::sync_to_ebml_id(uint32_t id)
{
    int64_t pos = stream.tellg();

    uint32_t i = 0;
    while (stream) {
        if (i == id) {
            stream.seekg(pos - 4);
            return 0;
        }

        i = (i << 8) | stream.get();
        ++pos;
    }

    return -1;
}

int32_t ebml_parser::sync_to_ebml_id(uint32_t id, uint32_t id_size)
{
    int64_t pos = stream.tellg();
    uint32_t i = 0;

    if (1 == id_size) {
        while (stream) {
            if (i == id) {
                stream.seekg(pos - 1);
                return 0;
            }

            i = static_cast<uint32_t>(stream.get());
            ++pos;
        }

        return -1;
    }

    uint32_t mask;
    switch (id_size) {
        case 2:
            mask = 0xffff;
            break;
        case 3:
            mask = 0xffffff;
            break;
        case 4:
            return sync_to_ebml_id(id);
        default:
            return -1;
    }
    while (stream) {
        if ((i & mask) == id) {
            stream.seekg(pos - id_size);
            return 0;
        }

        i = (i << 8) | stream.get();
        ++pos;
    }

    return -1;
}
