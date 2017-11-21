//
// Created by Charles on 2017/11/15.
//

#ifndef MOCK_PLAYER_EBML_HPP
#define MOCK_PLAYER_EBML_HPP


#include <cstdint>
#include <string>
#include <vector>
#include <iosfwd>
#include <stack>


namespace player {
namespace container {
namespace matroska {

struct ebml_node
{
    uint32_t id;
    uint64_t size;
    int64_t position;
};


class ebml_parser
{
public:
    explicit ebml_parser(std::istream &stream, int64_t pos = 0) : stream(stream), position(pos), stack() { }

    int32_t parse_next(ebml_node &result);

    void set_current_pos(int64_t pos);

    void down_to_sub_element(int64_t pos);

    void up_to_parent();

    int32_t read_integer(const ebml_node &node, int64_t &result);

    int32_t read_unsigned_integer(const ebml_node &node, uint64_t &result);

    int32_t read_float(const ebml_node &node, double &result);

    int32_t read_string(const ebml_node &node, std::string &result);

    int32_t read_date(const ebml_node &node, int64_t &result);

    int32_t read_binary(const ebml_node &node, std::vector<uint8_t> &result);

private:
    std::istream &stream;
    int64_t position;
    std::stack<int64_t> stack;
};

} // namespace matroska
} // namespace container
} // namespace player


#endif //MOCK_PLAYER_EBML_HPP
