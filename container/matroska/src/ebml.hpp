//
// Created by Charles on 2017/11/15.
//

#ifndef MOCK_PLAYER_EBML_HPP
#define MOCK_PLAYER_EBML_HPP


#include <cstdint>
#include <string>
#include <vector>
#include <istream>
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
    explicit ebml_parser(std::istream &stream) : stream(stream) { }

    int32_t parse_next(ebml_node &result);

    bool is_eof() const { return stream.eof(); }

    int64_t get_stream_pos() const { return stream.tellg(); }

    void set_stream_pos(int64_t pos) { stream.seekg(pos); }

    /**
     * sync stream position to a special ebml, of which the id length is 4
     *
     * @param id ebml id. the length of the id must be 4
     * @return zero if success, otherwise non-zero
     */
    int32_t sync_to_ebml_id(uint32_t id);

    /**
     * sync stream position to a special ebml.
     *
     * @param id ebml id
     * @param id_size length of the id in bytes
     * @return zero if success, otherwise non-zero
     */
    int32_t sync_to_ebml_id(uint32_t id, uint32_t id_size);

    int32_t read_integer(size_t size, int64_t &result);

    int32_t read_unsigned_integer(size_t size, uint64_t &result);

    int32_t read_float(size_t size, double &result);

    int32_t read_string(size_t size, std::string &result);

    int32_t read_date(int64_t &result);

    int32_t read_binary(size_t size, std::vector<uint8_t> &result);

private:
    std::istream &stream;
};

} // namespace matroska
} // namespace container
} // namespace player


#endif //MOCK_PLAYER_EBML_HPP
