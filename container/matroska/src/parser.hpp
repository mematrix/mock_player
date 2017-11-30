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
using master_handler = void (*)(T &, ebml_parser &, const ebml_node &);


/**
 * Level 1 elements:
 *
 * Meta Seek Information    [11][4D][9B][74]
 * Segment Information      [15][49][A9][66]
 * Cluster                  [1F][43][B6][75]
 * Track                    [16][54][AE][6B]
 * Cueing Data              [1C][53][BB][6B]
 * Attachment               [19][41][A4][69]
 * Chapters                 [10][43][A7][70]
 * Tagging                  [12][54][C3][67]
 */
enum : uint32_t
{
    EBML_META_SEEK_INFO_ID = 0x114D9B74,
    EBML_SEGMENT_INFO_ID = 0x1549A966,
    EBML_CLUSTER_ID = 0x1F43B675,
    EBML_TRACK_ID = 0x1654AE6B,
    EBML_CUEING_DATA_ID = 0x1C53BB6B,
    EBML_ATTACHMENT_ID = 0x1941A469,
    EBML_CHAPTERS_ID = 0x1043A770,
    EBML_TAGGING_ID = 0x1254C367
};


class parser_callback
{
public:
    virtual void handle_meta_seek_info(const meta_seek_head &seek_head, const ebml_node &node) = 0;

    virtual void handle_segment_info(const segment_info &seg_info, const ebml_node &node) = 0;

    virtual void handle_cluster(const cluster &cluster_info, const ebml_node &node) = 0;

    virtual void handle_track(const track &track_info, const ebml_node &node) = 0;

    virtual void handle_cueing_data(const cue &cue_data, const ebml_node &node) = 0;

    virtual void handle_attachment(const attachment &attach, const ebml_node &node) = 0;

    virtual void handle_chapters(const chapter &chapter_info, const ebml_node &node) = 0;

    virtual void handle_tagging(const tags &tags_info, const ebml_node &node) = 0;

    virtual void handle_unknown(const ebml_node &node) = 0;

    virtual bool need_parse_node(const ebml_node &node) = 0;

    virtual ~parser_callback() = default;
};

class matroska_parser
{
public:
    matroska_parser(std::istream &stream, parser_callback &cb);

    matroska_parser(const matroska_parser &) = delete;

    matroska_parser &operator=(const matroska_parser &) = delete;

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
     * @return ebml id if success, otherwise -1
     */
    uint32_t parse_next_element();

    /**
     * seek stream position
     *
     * @param position stream read position
     * @return zero if success, otherwise non-zero
     */
    void seek(int64_t position);

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
    void do_meta_seek_info_parse(const ebml_node &node);

    void do_segment_info_parse(const ebml_node &node);

    void do_cluster_parse(const ebml_node &node);

    void do_track_parse(const ebml_node &node);

    void do_cueing_data_parse(const ebml_node &node);

    void do_attachment_parse(const ebml_node &node);

    void do_chapters_parse(const ebml_node &node);

    void do_tagging_parse(const ebml_node &node);

    void do_skip_parse(const ebml_node &node);

private:
    parser_callback &callback;
    ebml_parser ep;
};

}
}
}


#endif //MOCK_PLAYER_PARSER_HPP
