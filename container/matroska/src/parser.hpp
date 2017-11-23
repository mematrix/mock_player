//
// Created by Charles on 2017/11/20.
//

#ifndef MOCK_PLAYER_PARSER_HPP
#define MOCK_PLAYER_PARSER_HPP


#include <memory>
#include <iosfwd>

#include "element.hpp"
#include "ebml.hpp"


namespace player {
namespace container {
namespace matroska {

class parser_callback
{
public:
    virtual void on_meta_seek_info(const meta_seek_head &seek_head) = 0;

    virtual void on_segment_info(const segment_info &seg_info) = 0;

    virtual ~parser_callback() = default;
};

class parser
{
public:
    parser(std::unique_ptr<std::istream> &&stream, parser_callback &cb);

    parser(const parser &) = delete;

    parser &operator=(const parser &) = delete;

    /**
     * parse matroska file header element. this method should be called firstly
     *
     * @param result if success, contains the ebml_header data
     * @return zero if success, otherwise non-zero
     */
    int32_t parse_ebml_header(ebml_header &result);

    /**
     * parse matroska segment element. this method should be called after (@see parse_ebml_header),
     * but before (@see parse_next_element).
     *
     * @param result if success, contains the segment element ebml-structure data
     * @return zero if success, otherwise non-zero
     */
    int32_t parse_segment(ebml_node &result);

    /**
     * parse next one element from current position. before call it, the stream position must align
     * with an ebml-element
     *
     * @return zero if success, otherwise non-zero
     */
    int32_t parse_next_element();

    /**
     * seek stream position
     *
     * @param position stream read position
     * @return zero if success, otherwise non-zero
     */
    int32_t seek(int64_t position);

    template<typename T>
    int32_t read_element(T &result, uint32_t id/* master func*/);

private:
    void do_meta_seek_info_parse();

    void do_segment_info_parse();

    void do_cluster_parse();

private:
    std::unique_ptr<std::istream> in_stream;
    parser_callback &callback;
    ebml_parser ep;
};

}
}
}


#endif //MOCK_PLAYER_PARSER_HPP
