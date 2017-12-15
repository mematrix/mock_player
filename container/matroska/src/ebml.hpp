//
// Created by Charles on 2017/11/15.
//

#ifndef MOCK_PLAYER_EBML_HPP
#define MOCK_PLAYER_EBML_HPP


#include <cstdint>
#include <string>
#include <istream>

#include "io/buffer.hpp"


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
    class stream_proxy
    {
        std::istream &raw;
        int64_t pos;

    public:
        explicit stream_proxy(std::istream &s) : raw(s)
        {
            pos = s.tellg();
        }

        stream_proxy(stream_proxy &&that) noexcept : raw(that.raw), pos(that.pos)
        {
            that.pos = -1;
        }

        std::istream &get()
        {
            return raw;
        }

        ~stream_proxy()
        {
            if (pos >= 0) {
                raw.seekg(pos);
            }
        }
    };

    explicit ebml_parser(std::istream &stream) : stream(stream) { }

    int32_t parse_next(ebml_node &result);

    bool is_eof() const { return stream.eof(); }

    int64_t get_stream_pos() const { return stream.tellg(); }

    void set_stream_pos(int64_t pos) { stream.seekg(pos); }

    stream_proxy get_stream() { return stream_proxy(stream); }

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

    static int32_t read_binary(std::istream &stream, size_t size, io::bytes_buffer &result);

    template<unsigned int Size>
    static int32_t read_binary(std::istream &stream, size_t size, io::stack_bytes_buffer<Size> &result);

    static uint32_t read_ebml_id(std::istream &stream, size_t force_size = 0);

private:
    std::istream &stream;
};

} // namespace matroska
} // namespace container
} // namespace player


#endif //MOCK_PLAYER_EBML_HPP
