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


template<typename T>
using master_handler = void (*)(T &, const element_identify &, ebml_parser &, const ebml_node &);


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

    /**
     * resync stream position to (probable) next cluster element start position.
     * @warning call this method only when stream is corrupt, because this method will find the cluster
     * id in the stream sequentially.
     * @return zero if success, otherwise non-zero
     */
    int32_t resync_to_cluster();

    /**
     * skip current element until detect a cluster element. unlike @see resync_to_cluster, this
     * method will read an ebml element and test ebml_id, and forward seek by the ebml_size.
     *
     * @return zero if success, otherwise non-zero
     */
    int32_t skip_to_cluster();

    /**
     * read special type element from current position. if id reading from stream is not equal to
     * the param @see id, an error will return.
     *
     * @tparam T result type
     * @param result element entry object
     * @param id special ebml id
     * @param handler if not null, handle the sub master element reading process.
     * @return zeor if success, otherwise non-zero
     */
    template<typename T>
    int32_t read_element(T &result, uint32_t id, master_handler<T> handler = nullptr);

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
