//
// Created by Charles on 2017/11/15.
//

#include "ebml.hpp"

#include <istream>
#include "util/endian_util.hpp"

using namespace std;
using namespace player;
using namespace player::container::matroska;


static int64_t read_string(istream &stream, int64_t position, uint64_t size, std::string &output)
{
    stream.seekg(position);

    char tmp[16];
    while (stream && size > 16) {
        stream.read(tmp, 16);
        auto count = stream.gcount();
        output.append(tmp, static_cast<string::size_type>(count));

        if (count < 16) {
            return size - count;
        }
        size -= 16;
    }

    if (size > 16) {
        return size;
    } else if (size != 0) {
        stream.read(tmp, size);
        auto count = stream.gcount();
        output.append(tmp, static_cast<string::size_type>(count));

        return size - count;
    }

    return 0;
}

static int64_t read_data(istream &stream, int64_t position, uint64_t size, char *data)
{
    stream.seekg(position);
    stream.read(data, size);

    return size - stream.gcount();
}

static unsigned get_prefix_zero_count(unsigned char c, int max_count)
{
    while (c > 0) {
        c >>= 1;
        --max_count;
    }

    return static_cast<unsigned>(max_count);
}


int32_t ebml_parser::parse_next(ebml_node &result)
{
    stream.seekg(position);
    if (!stream) {
        return -1;
    }

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

    position = result.position + result.size;
    return 0;
}

int32_t ebml_parser::read_integer(const ebml_node &node, int64_t &result)
{
    assert(node.size <= 8);

    char data[8];
    auto r = read_data(stream, node.position, node.size, data);
    if (r) {
        return static_cast<int32_t>(r);
    }

    result = util::from_big_endian_variable(data, static_cast<unsigned int>(node.size));

    return 0;
}

int32_t ebml_parser::read_unsigned_integer(const ebml_node &node, uint64_t &result)
{
    assert(node.size <= 8);

    char data[8];
    auto r = read_data(stream, node.position, node.size, data);
    if (r) {
        return static_cast<int32_t>(r);
    }

    result = util::from_big_endian_variable_unsigned(data, static_cast<unsigned int>(node.size));

    return 0;
}

int32_t ebml_parser::read_float(const ebml_node &node, double &result)
{
    assert(node.size == 4 || node.size == 8);

    char data[8];
    auto r = read_data(stream, node.position, node.size, data);
    if (r) {
        return static_cast<int32_t>(r);
    }

    if (4 == node.size) {
        result = util::from_big_endian_float(data);
    } else {
        result = util::from_big_endian_double(data);
    }

    return 0;
}

int32_t ebml_parser::read_string(const ebml_node &node, std::string &result)
{
    return static_cast<int32_t>(::read_string(stream, node.position, node.size, result));
}

int32_t ebml_parser::read_date(const ebml_node &node, int64_t &result)
{
    assert(node.size <= 8);

    char data[8];
    auto r = read_data(stream, node.position, node.size, data);
    if (r) {
        return static_cast<int32_t>(r);
    }

    result = util::from_big_endian(data);

    return 0;
}

int32_t ebml_parser::read_binary(const ebml_node &node, std::vector<uint8_t> &result)
{
    result.resize(node.size);

    uint64_t i = 0;
    while (stream && i < node.size) {
        result[i++] = static_cast<uint8_t>(stream.get());
    }

    return static_cast<int32_t>(node.size - i);
}

void ebml_parser::set_current_pos(int64_t pos)
{
    position = pos;
}

void ebml_parser::down_to_sub_element(int64_t pos)
{
    stack.push(position);
    position = pos;
}

void ebml_parser::up_to_parent()
{
    position = stack.top();
    stack.pop();
}
