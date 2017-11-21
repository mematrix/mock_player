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

    int32_t parse_ebml_header(ebml_header &result);

    int32_t parse_segment();

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
