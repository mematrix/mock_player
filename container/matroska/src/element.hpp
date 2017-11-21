//
// Created by Charles on 2017/11/17.
//

#ifndef MOCK_PLAYER_ELEMENT_HPP
#define MOCK_PLAYER_ELEMENT_HPP


#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <bits/unique_ptr.h>

#define ELEMENT_PROPERTY(ma, mu, def, type, ver)


namespace player {
namespace container {
namespace matroska {

using binary = std::vector<uint8_t>;

template<typename T>
struct value_set_helper
{
    const T &cast() const { return static_cast<const T &>(*this); }

    bool is_set(int32_t element) const
    {
        return (element & (cast().get_mask() | T::DEFAULT_VALUE_SET)) != 0;
    }

    bool is_user_set(int32_t element) const
    {
        return (element & cast().get_mask()) != 0;
    }

    bool is_all_set(int32_t elements) const
    {
        return (elements & (cast().get_mask() | T::DEFAULT_VALUE_SET)) == elements;
    }
};

#define USING_VALUE_METHOD(value_set) \
    enum { DEFAULT_VALUE_SET = (value_set) }; \
    int32_t get_mask() const { return mask; } \
    using value_set_helper::is_set; \
    using value_set_helper::is_user_set; \
    using value_set_helper::is_all_set


struct ebml_header : public value_set_helper<ebml_header>
{
    enum
    {
        VERSION = 1 << 0,
        READ_VERSION = 1 << 1,
        MAX_ID_LENGTH = 1 << 2,
        MAX_SIZE_LENGTH = 1 << 3,
        DOC_TYPE = 1 << 4,
        DOC_TYPE_VERSION = 1 << 5,
        DOC_TYPE_READ_VERSION = 1 << 6
    };

    ebml_header() : doc_type("matroska") { }

    uint64_t get_version() const { return version; }

    void set_version(uint64_t v)
    {
        version = v;
        mask |= VERSION;
    }

    uint64_t get_read_version() const { return read_version; }

    void set_read_version(uint64_t v)
    {
        read_version = v;
        mask |= READ_VERSION;
    }

    uint64_t get_max_id_length() const { return max_id_length; }

    void set_max_id_length(uint64_t v)
    {
        max_id_length = v;
        mask |= MAX_ID_LENGTH;
    }

    uint64_t get_max_size_length() const { return max_size_length; }

    void set_max_size_length(uint64_t v)
    {
        max_size_length = v;
        mask |= MAX_SIZE_LENGTH;
    }

    const std::string &get_doc_type() const { return doc_type; }

    void set_doc_type(std::string &&v)
    {
        doc_type = std::move(v);
        mask |= DOC_TYPE;
    }

    uint64_t get_doc_type_version() const { return doc_type_version; }

    void set_doc_type_version(uint64_t v)
    {
        doc_type_version = v;
        mask |= DOC_TYPE_VERSION;
    }

    uint64_t get_doc_type_read_version() const { return doc_type_read_version; }

    void set_doc_type_read_version(uint64_t v)
    {
        doc_type_read_version = v;
        mask |= DOC_TYPE_READ_VERSION;
    }

    USING_VALUE_METHOD(VERSION | READ_VERSION | MAX_ID_LENGTH | MAX_SIZE_LENGTH | DOC_TYPE | DOC_TYPE_VERSION | DOC_TYPE_READ_VERSION);

    /*bool is_set(int32_t element)
    {
        return (element & (mask | DEFAULT_VALUE_SET)) != 0;
    }

    bool is_user_set(int32_t element)
    {
        return (element & mask) != 0;
    }

    bool is_all_set(int32_t elements)
    {
        return (elements & (mask | DEFAULT_VALUE_SET)) == elements;
    }*/

private:
    uint64_t version = 1;
    uint64_t read_version = 1;
    uint64_t max_id_length = 4;
    uint64_t max_size_length = 8;
    std::string doc_type;
    uint64_t doc_type_version = 1;
    uint64_t doc_type_read_version = 1;

    int32_t mask = 0;
};


struct meta_seek : public value_set_helper<meta_seek>
{
    enum
    {
        ID = 1 << 0,
        POSITION = 1 << 1
    };

    meta_seek() = default;

    const binary &get_id() const { return id; }

    void set_id(binary &&v)
    {
        id = std::move(v);
        mask |= ID;
    }

    const uint64_t get_position() const { return position; }

    void set_position(uint64_t v)
    {
        position = v;
        mask |= POSITION;
    }

    USING_VALUE_METHOD(0);

private:
    binary id;
    uint64_t position = 0;

    int32_t mask = 0;
};


struct meta_seek_head : public value_set_helper<meta_seek_head>
{
    enum
    {
        SEEK = 1 << 0
    };

    meta_seek_head() = default;

    const std::list<meta_seek> &get_seeks() const { return seeks; }

    void set_seeks(std::list<meta_seek> &&v)
    {
        seeks = std::move(v);
        mask |= SEEK;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<meta_seek> seeks;

    int32_t mask = 0;
};


struct segment_chapter_translate : public value_set_helper<segment_chapter_translate>
{
    enum
    {
        EDITION_UID = 1 << 0,
        CODEC = 1 << 1,
        ID = 1 << 2
    };

    segment_chapter_translate() = default;

    const std::vector<uint64_t> &get_edition_uids() const { return edition_uids; }

    void set_edition_uids(std::vector<uint64_t> &&v)
    {
        edition_uids = std::move(v);
        mask |= EDITION_UID;
    }

    uint64_t get_codec() const { return codec; }

    void set_codec(uint64_t v)
    {
        codec = v;
        mask |= CODEC;
    }

    const binary &get_id() const { return id; }

    void set_id(binary &&v)
    {
        id = std::move(v);
        mask |= ID;
    }

    USING_VALUE_METHOD(0);

private:
    std::vector<uint64_t> edition_uids;
    uint64_t codec = 0;
    binary id;

    int32_t mask = 0;
};


struct segment_info : public value_set_helper<segment_info>
{
    enum
    {
        UID = 1 << 0,
        FILENAME = 1 << 1,
        PREV_UID = 1 << 2,
        PREV_FILENAME = 1 << 3,
        NEXT_UID = 1 << 4,
        NEXT_FILENAME = 1 << 5,
        FAMILY = 1 << 6,
        CHAPTER_TRANSLATE = 1 << 7,
        TIMECODE_SCALE = 1 << 8,
        DURATION = 1 << 9,
        DATE_UTC = 1 << 10,
        TITLE = 1 << 11,
        MUXING_APP = 1 << 12,
        WRITING_APP = 1 << 13
    };

    segment_info() = default;

    const binary &get_uid() const { return uid; }

    void set_uid(binary &&v)
    {
        uid = std::move(v);
        mask |= UID;
    }

    const std::string &get_filename() const { return filename; }

    void set_filename(std::string &&v)
    {
        filename = std::move(v);
        mask |= FILENAME;
    }

    const binary &get_prev_uid() const { return prev_uid; }

    void set_prev_uid(binary &&v)
    {
        prev_uid = std::move(v);
        mask |= PREV_UID;
    }

    const std::string &get_prev_filename() const { return prev_filename; }

    void set_prev_filename(std::string &&v)
    {
        prev_filename = std::move(v);
        mask |= PREV_FILENAME;
    }

    const binary &get_next_uid() const { return next_uid; }

    void set_next_uid(binary &&v)
    {
        next_uid = std::move(v);
        mask |= NEXT_UID;
    }

    const std::string &get_next_filename() const { return next_filename; }

    void set_next_filename(std::string &&v)
    {
        next_filename = std::move(v);
        mask |= NEXT_FILENAME;
    }

    const std::list<binary> &get_families() const { return families; }

    void set_families(std::list<binary> &&v)
    {
        families = std::move(v);
        mask |= FAMILY;
    }

    const std::list<segment_chapter_translate> &get_chapter_translates() const { return chapter_translates; }

    void set_chapter_translates(std::list<segment_chapter_translate> &&v)
    {
        chapter_translates = std::move(v);
        mask |= CHAPTER_TRANSLATE;
    }

    uint64_t get_timecode_scale() const { return timecode_scale; }

    void set_timecode_scale(uint64_t v)
    {
        timecode_scale = v;
        mask |= TIMECODE_SCALE;
    }

    double get_duration() const { return duration; }

    void set_duration(double v)
    {
        duration = v;
        mask |= DURATION;
    }

    int64_t get_date_utc() const { return date_utc; }

    void set_date_uti(int64_t v)
    {
        date_utc = v;
        mask |= DATE_UTC;
    }

    const std::string &get_title() const { return title; }

    void set_title(std::string &&v)
    {
        title = std::move(v);
        mask |= TITLE;
    }

    const std::string &get_muxing_app() const { return muxing_app; }

    void set_muxing_app(std::string &&v)
    {
        muxing_app = std::move(v);
        mask |= MUXING_APP;
    }

    const std::string &get_writing_app() const { return writing_app; }

    void set_writing_app(std::string &&v)
    {
        writing_app = std::move(v);
        mask |= WRITING_APP;
    }

    USING_VALUE_METHOD(TIMECODE_SCALE);

private:
    binary uid;
    std::string filename;
    binary prev_uid;
    std::string prev_filename;
    binary next_uid;
    std::string next_filename;

    std::list<binary> families;
    std::list<segment_chapter_translate> chapter_translates;

    uint64_t timecode_scale = 1000000;
    double duration = 0.0;
    int64_t date_utc = 0;
    std::string title;
    std::string muxing_app;
    std::string writing_app;

    int32_t mask = 0;
};


struct cluster_silent_tracks : public value_set_helper<cluster_silent_tracks>
{
    enum
    {
        NUMBER = 1 << 0
    };

    cluster_silent_tracks() = default;

    cluster_silent_tracks(cluster_silent_tracks &&) = default;

    cluster_silent_tracks &operator=(cluster_silent_tracks &&) = default;

    const std::vector<uint64_t> &get_numbers() const { return numbers; }

    void set_numbers(std::vector<uint64_t> &&v)
    {
        numbers = std::move(v);
        mask |= NUMBER;
    }

    USING_VALUE_METHOD(0);

private:
    std::vector<uint64_t> numbers;

    int32_t mask = 0;
};


struct cluster_block_more : public value_set_helper<cluster_block_more>
{
    enum
    {
        ADD_ID = 1 << 0,
        ADDITIONAL = 1 << 1
    };

    cluster_block_more() = default;

    uint64_t get_add_id() const { return add_id; }

    void set_add_id(uint64_t v)
    {
        add_id = v;
        mask |= ADD_ID;
    }

    const binary &get_additional() const { return additional; }

    void set_additional(binary &&v)
    {
        additional = std::move(v);
        mask |= ADDITIONAL;
    }

    USING_VALUE_METHOD(ADD_ID);

private:
    uint64_t add_id = 1;
    binary additional;

    int32_t mask = 0;
};


struct cluster_block_additions : public value_set_helper<cluster_block_additions>
{
    enum
    {
        BLOCK_MORE = 1 << 0
    };

    cluster_block_additions() = default;

    cluster_block_additions(cluster_block_additions &&) = default;

    cluster_block_additions &operator=(const cluster_block_additions &) = default;

    cluster_block_additions &operator=(cluster_block_additions &&) = default;

    const std::list<cluster_block_more> &get_block_more() const { return block_more; }

    void set_block_more(std::list<cluster_block_more> &&v)
    {
        block_more = std::move(v);
        mask |= BLOCK_MORE;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<cluster_block_more> block_more;

    int32_t mask = 0;
};


struct cluster_time_slice : public value_set_helper<cluster_time_slice>
{
    enum
    {
        LACE_NUMBER = 1 << 0
    };

    cluster_time_slice() = default;

    uint64_t get_lace_number() const { return lace_number; }

    void set_lace_number(uint64_t v)
    {
        lace_number = v;
        mask |= LACE_NUMBER;
    }

    USING_VALUE_METHOD(LACE_NUMBER);

private:
    uint64_t lace_number = 0;

    int32_t mask = 0;
};


struct cluster_slices : public value_set_helper<cluster_slices>
{
    enum
    {
        TIME_SLICE = 1 << 0
    };

    cluster_slices() = default;

    cluster_slices(cluster_slices &&) = default;

    cluster_slices &operator=(const cluster_slices &) = default;

    cluster_slices &operator=(cluster_slices &&) = default;

    const std::list<cluster_time_slice> &get_time_slices() const { return time_slices; }

    void set_time_slices(std::list<cluster_time_slice> &&v)
    {
        time_slices = std::move(v);
        mask |= TIME_SLICE;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<cluster_time_slice> time_slices;

    int32_t mask = 0;
};


struct cluster_block : public value_set_helper<cluster_block>
{
    enum
    {
        BLOCK = 1 << 0,
        ADDITIONS = 1 << 1,
        DURATION = 1 << 2,
        REFERENCE_PRIORITY = 1 << 3,
        REFERENCE_BLOCK = 1 << 4,
        CODEC_STATE = 1 << 5,
        DISCARD_PADDING = 1 << 6,
        SLICES = 1 << 7
    };

    cluster_block() = default;

    cluster_block(cluster_block &&) = default;

    cluster_block &operator=(cluster_block &&) = default;

    const binary &get_block() const { return block; }

    void set_block(binary v)
    {
        block = std::move(v);
        mask |= BLOCK;
    }

    const cluster_block_additions &get_additions() const { return additions; }

    void set_additions(cluster_block_additions &&v)
    {
        additions = std::move(v);
        mask |= ADDITIONS;
    }

    uint64_t get_duration() const { return duration; }

    void set_duration(uint64_t v)
    {
        duration = v;
        mask |= DURATION;
    }

    uint64_t get_reference_priority() const { return reference_priority; }

    void set_reference_priority(uint64_t v)
    {
        reference_priority = v;
        mask |= REFERENCE_PRIORITY;
    }

    int64_t get_reference_block() const { return reference_block; }

    void set_reference_block(int64_t v)
    {
        reference_block = v;
        mask |= REFERENCE_BLOCK;
    }

    const binary &get_codec_state() const { return codec_state; }

    void set_codec_state(binary &&v)
    {
        codec_state = std::move(v);
        mask |= CODEC_STATE;
    }

    int64_t get_discard_padding() const { return discard_padding; }

    void set_discard_padding(int64_t v)
    {
        discard_padding = v;
        mask |= DISCARD_PADDING;
    }

    const cluster_slices &get_slices() const { return slices; }

    void set_slice(cluster_slices &&v)
    {
        slices = std::move(v);
        mask |= SLICES;
    }

    USING_VALUE_METHOD(REFERENCE_PRIORITY);

private:
    binary block;
    cluster_block_additions additions;
    uint64_t duration = 0;
    uint64_t reference_priority = 0;
    int64_t reference_block = 0;
    binary codec_state;
    int64_t discard_padding = 0;
    cluster_slices slices;

    int32_t mask = 0;
};


struct cluster : public value_set_helper<cluster>
{
    enum
    {
        TIMECODE = 1 << 0,
        SILENT_TRACKS = 1 << 1,
        POSITION = 1 << 2,
        PREV_SIZE = 1 << 3,
        SIMPLE_BLOCK = 1 << 4,
        BLOCK_GROUP = 1 << 5
    };

    cluster() = default;

    cluster(cluster &&) = default;

    cluster &operator=(cluster &&)= default;

    uint64_t get_timecode() const { return timecode; }

    void set_timecode(uint64_t v)
    {
        timecode = v;
        mask |= TIMECODE;
    }

    const cluster_silent_tracks &get_silent_tracks() const { return silent_tracks; }

    void set_silent_tracks(cluster_silent_tracks &&v)
    {
        silent_tracks = std::move(v);
        mask |= SILENT_TRACKS;
    }

    uint64_t get_position() const { return position; }

    void set_position(uint64_t v)
    {
        position = v;
        mask |= POSITION;
    }

    uint64_t get_prev_size() const { return prev_size; }

    void set_prev_size(uint64_t v)
    {
        prev_size = v;
        mask |= PREV_SIZE;
    }

    const std::list<binary> &get_simple_blocks() const { return simple_blocks; }

    void set_simple_blocks(std::list<binary> &&v)
    {
        simple_blocks = std::move(v);
        mask |= SIMPLE_BLOCK;
    }

    const std::list<cluster_block> &get_block_group() const { return block_group; }

    void set_block_group(std::list<cluster_block> &&v)
    {
        block_group = std::move(v);
        mask |= BLOCK_GROUP;
    }

    USING_VALUE_METHOD(0);

private:
    uint64_t timecode = 0;
    cluster_silent_tracks silent_tracks;
    uint64_t position = 0;
    uint64_t prev_size = 0;
    std::list<binary> simple_blocks;
    std::list<cluster_block> block_group;

    int32_t mask = 0;
};


struct track_translate : public value_set_helper<track_translate>
{
    enum
    {
        EDITION_UID = 1 << 0,
        CODEC = 1 << 1,
        TRACK_ID = 1 << 2
    };

    track_translate() = default;

    track_translate(track_translate &&) = default;

    track_translate &operator=(track_translate &&)= default;

    const std::vector<uint64_t> &get_edition_uids() const { return edition_uids; }

    void set_edition_uids(std::vector<uint64_t> &&v)
    {
        edition_uids = std::move(v);
        mask |= EDITION_UID;
    }

    uint64_t get_codec() const { return codec; }

    void set_codec(uint64_t v)
    {
        codec = v;
        mask |= CODEC;
    }

    const binary &get_track_id() const { return track_id; }

    void set_track_id(binary v)
    {
        track_id = std::move(v);
        mask |= TRACK_ID;
    }

    USING_VALUE_METHOD(0);

private:
    std::vector<uint64_t> edition_uids;
    uint64_t codec = 0;
    binary track_id;

    int32_t mask = 0;
};


struct track_video_color_metadata : public value_set_helper<track_video_color_metadata>
{
    enum
    {
        PRIMARY_R_CHROMATICITY_X = 1 << 0,
        PRIMARY_R_CHROMATICITY_Y = 1 << 1,
        PRIMARY_G_CHROMATICITY_X = 1 << 2,
        PRIMARY_G_CHROMATICITY_Y = 1 << 3,
        PRIMARY_B_CHROMATICITY_X = 1 << 4,
        PRIMARY_B_CHROMATICITY_Y = 1 << 5,
        WHITE_POINT_CHROMATICITY_X = 1 << 6,
        WHITE_POINT_CHROMATICITY_Y = 1 << 7,
        LUMINANCE_MAX = 1 << 8,
        LUMINANCE_MIN = 1 << 9
    };

    track_video_color_metadata() = default;

    double get_primary_r_chromaticity_x() const { return primary_r_chromaticity_x; }

    void set_primary_r_chromaticity_x(double v)
    {
        primary_r_chromaticity_x = v;
        mask |= PRIMARY_R_CHROMATICITY_X;
    }

    double get_primary_r_chromaticity_y() const { return primary_r_chromaticity_y; }

    void set_primary_r_chromaticity_y(double v)
    {
        primary_r_chromaticity_y = v;
        mask |= PRIMARY_R_CHROMATICITY_Y;
    }

    double get_primary_g_chromaticity_x() const { return primary_g_chromaticity_x; }

    void set_primary_g_chromaticity_x(double v)
    {
        primary_g_chromaticity_x = v;
        mask |= PRIMARY_G_CHROMATICITY_X;
    }

    double get_primary_g_chromaticity_y() const { return primary_g_chromaticity_y; }

    void set_primary_g_chromaticity_y(double v)
    {
        primary_g_chromaticity_y = v;
        mask |= PRIMARY_G_CHROMATICITY_Y;
    }

    double get_primary_b_chromaticity_x() const { return primary_b_chromaticity_x; }

    void set_primary_b_chromaticity_x(double v)
    {
        primary_b_chromaticity_x = v;
        mask |= PRIMARY_B_CHROMATICITY_X;
    }

    double get_primary_b_chromaticity_y() const { return primary_b_chromaticity_y; }

    void set_primary_b_chromaticity_y(double v)
    {
        primary_b_chromaticity_y = v;
        mask |= PRIMARY_B_CHROMATICITY_Y;
    }

    double get_white_point_chromaticity_x() const { return white_point_chromaticity_x; }

    void set_white_point_chromaticity_x(double v)
    {
        white_point_chromaticity_x = v;
        mask |= WHITE_POINT_CHROMATICITY_X;
    }

    double get_white_point_chromaticity_y() const { return white_point_chromaticity_y; }

    void set_white_point_chromaticity_y(double v)
    {
        white_point_chromaticity_y = v;
        mask |= WHITE_POINT_CHROMATICITY_Y;
    }

    double get_luminance_max() const { return luminance_max; }

    void set_luminance_max(double v)
    {
        luminance_max = v;
        mask |= LUMINANCE_MAX;
    }

    double get_luminance_min() const { return luminance_min; }

    void set_luminance_min(double v)
    {
        luminance_min = v;
        mask |= LUMINANCE_MIN;
    }

    USING_VALUE_METHOD(0);

private:
    double primary_r_chromaticity_x = 0.0;
    double primary_r_chromaticity_y = 0.0;
    double primary_g_chromaticity_x = 0.0;
    double primary_g_chromaticity_y = 0.0;
    double primary_b_chromaticity_x = 0.0;
    double primary_b_chromaticity_y = 0.0;
    double white_point_chromaticity_x = 0.0;
    double white_point_chromaticity_y = 0.0;
    double luminance_max = 0.0;
    double luminance_min = 0.0;

    int32_t mask = 0;
};


struct track_video_color : public value_set_helper<track_video_color>
{
    enum
    {
        MATRIX_COEFFICIENTS = 1 << 0,
        BITS_PER_CHANNEL = 1 << 1,
        CHROMA_SUBSAMPLING_HORZ = 1 << 2,
        CHROMA_SUBSAMPLING_VERT = 1 << 3,
        CB_SUBSAMPLING_HORZ = 1 << 4,
        CB_SUBSAMPLING_VERT = 1 << 5,
        CHROMA_SITING_HORZ = 1 << 6,
        CHROMA_SITING_VERT = 1 << 7,
        RANGE = 1 << 8,
        TRANSFER_CHARACTERISTICS = 1 << 9,
        PRIMARIES = 1 << 10,
        MAX_CLL = 1 << 11,
        MAX_FALL = 1 << 12,
        MASTERING_METADATA = 1 << 13
    };

    track_video_color() = default;

    track_video_color(track_video_color &&) = default;

    track_video_color &operator=(track_video_color &&)= default;

    uint64_t get_matrix_coefficients() const
    {
        return matrix_coefficients;
    }

    void set_matrix_coefficients(uint64_t matrix_coefficients)
    {
        track_video_color::matrix_coefficients = matrix_coefficients;
        mask |= MATRIX_COEFFICIENTS;
    }

    uint64_t get_bits_per_channel() const
    {
        return bits_per_channel;
    }

    void set_bits_per_channel(uint64_t bits_per_channel)
    {
        track_video_color::bits_per_channel = bits_per_channel;
        mask |= BITS_PER_CHANNEL;
    }

    uint64_t get_chroma_subsampling_horz() const
    {
        return chroma_subsampling_horz;
    }

    void set_chroma_subsampling_horz(uint64_t chroma_subsampling_horz)
    {
        track_video_color::chroma_subsampling_horz = chroma_subsampling_horz;
        mask |= CHROMA_SUBSAMPLING_HORZ;
    }

    uint64_t get_chroma_subsampling_vert() const
    {
        return chroma_subsampling_vert;
    }

    void set_chroma_subsampling_vert(uint64_t chroma_subsampling_vert)
    {
        track_video_color::chroma_subsampling_vert = chroma_subsampling_vert;
        mask |= CHROMA_SUBSAMPLING_VERT;
    }

    uint64_t get_cb_subsampling_horz() const
    {
        return cb_subsampling_horz;
    }

    void set_cb_subsampling_horz(uint64_t cb_subsampling_horz)
    {
        track_video_color::cb_subsampling_horz = cb_subsampling_horz;
        mask |= CB_SUBSAMPLING_HORZ;
    }

    uint64_t get_cb_subsampling_vert() const
    {
        return cb_subsampling_vert;
    }

    void set_cb_subsampling_vert(uint64_t cb_subsampling_vert)
    {
        track_video_color::cb_subsampling_vert = cb_subsampling_vert;
        mask |= CB_SUBSAMPLING_VERT;
    }

    uint64_t get_chroma_siting_horz() const
    {
        return chroma_siting_horz;
    }

    void set_chroma_siting_horz(uint64_t chroma_siting_horz)
    {
        track_video_color::chroma_siting_horz = chroma_siting_horz;
        mask |= CHROMA_SITING_HORZ;
    }

    uint64_t get_chroma_siting_vert() const
    {
        return chroma_siting_vert;
    }

    void set_chroma_siting_vert(uint64_t chroma_siting_vert)
    {
        track_video_color::chroma_siting_vert = chroma_siting_vert;
        mask |= CHROMA_SITING_VERT;
    }

    uint64_t get_range() const
    {
        return range;
    }

    void set_range(uint64_t range)
    {
        track_video_color::range = range;
        mask |= RANGE;
    }

    uint64_t get_transfer_characteristics() const
    {
        return transfer_characteristics;
    }

    void set_transfer_characteristics(uint64_t transfer_characteristics)
    {
        track_video_color::transfer_characteristics = transfer_characteristics;
        mask |= TRANSFER_CHARACTERISTICS;
    }

    uint64_t get_primaries() const
    {
        return primaries;
    }

    void set_primaries(uint64_t primaries)
    {
        track_video_color::primaries = primaries;
        mask |= PRIMARIES;
    }

    uint64_t get_max_cll() const
    {
        return max_cll;
    }

    void set_max_cll(uint64_t max_cll)
    {
        track_video_color::max_cll = max_cll;
        mask |= MAX_CLL;
    }

    uint64_t get_max_fall() const
    {
        return max_fall;
    }

    void set_max_fall(uint64_t max_fall)
    {
        track_video_color::max_fall = max_fall;
        mask |= MAX_FALL;
    }

    const std::unique_ptr<track_video_color_metadata> &get_mastering_metadata() const
    {
        return mastering_metadata;
    }

    void set_mastering_metadata(std::unique_ptr<track_video_color_metadata> &&mastering_metadata)
    {
        track_video_color::mastering_metadata = std::move(mastering_metadata);
        mask |= MASTERING_METADATA;
    }

    USING_VALUE_METHOD(MATRIX_COEFFICIENTS | BITS_PER_CHANNEL | CHROMA_SITING_HORZ | CHROMA_SITING_VERT | RANGE | TRANSFER_CHARACTERISTICS | PRIMARIES);

private:
    uint64_t matrix_coefficients = 2;
    uint64_t bits_per_channel = 0;
    uint64_t chroma_subsampling_horz = 0;
    uint64_t chroma_subsampling_vert = 0;
    uint64_t cb_subsampling_horz = 0;
    uint64_t cb_subsampling_vert = 0;
    uint64_t chroma_siting_horz = 0;
    uint64_t chroma_siting_vert = 0;
    uint64_t range = 0;
    uint64_t transfer_characteristics = 2;
    uint64_t primaries = 2;
    uint64_t max_cll = 0;
    uint64_t max_fall = 0;
    std::unique_ptr<track_video_color_metadata> mastering_metadata;

    int32_t mask = 0;
};


struct track_video : public value_set_helper<track_video>
{
    enum
    {
        FLAG_INTERLACED = 1 << 0,
        FIELD_ORDER = 1 << 1,
        STEREO_MODE = 1 << 2,
        ALPHA_MODE = 1 << 3,
        PIXEL_WIDTH = 1 << 4,
        PIXEL_HEIGHT = 1 << 5,
        PIXEL_CROP_BOTTOM = 1 << 6,
        PIXEL_CROP_TOP = 1 << 7,
        PIXEL_CROP_LEFT = 1 << 8,
        PIXEL_CROP_RIGHT = 1 << 9,
        DISPLAY_WIDTH = 1 << 10,
        DISPLAY_HEIGHT = 1 << 11,
        DISPLAY_UNIT = 1 << 12,
        ASPECT_RATIO_TYPE = 1 << 13,
        COLOR_SPACE = 1 << 14,
        COLOR = 1 << 15
    };

    track_video() = default;

    track_video(track_video &&) = default;

    track_video &operator=(track_video &&)= default;

    uint64_t get_flag_interlaced() const
    {
        return flag_interlaced;
    }

    void set_flag_interlaced(uint64_t flag_interlaced)
    {
        track_video::flag_interlaced = flag_interlaced;
        mask |= FLAG_INTERLACED;
    }

    uint64_t get_field_order() const
    {
        return field_order;
    }

    void set_field_order(uint64_t field_order)
    {
        track_video::field_order = field_order;
        mask |= FIELD_ORDER;
    }

    uint64_t get_stereo_mode() const
    {
        return stereo_mode;
    }

    void set_stereo_mode(uint64_t stereo_mode)
    {
        track_video::stereo_mode = stereo_mode;
        mask |= STEREO_MODE;
    }

    uint64_t get_alpha_mode() const
    {
        return alpha_mode;
    }

    void set_alpha_mode(uint64_t alpha_mode)
    {
        track_video::alpha_mode = alpha_mode;
        mask |= ALPHA_MODE;
    }

    uint64_t get_pixel_width() const
    {
        return pixel_width;
    }

    void set_pixel_width(uint64_t pixel_width)
    {
        track_video::pixel_width = pixel_width;
        mask |= PIXEL_WIDTH;
    }

    uint64_t get_pixel_height() const
    {
        return pixel_height;
    }

    void set_pixel_height(uint64_t pixel_height)
    {
        track_video::pixel_height = pixel_height;
        mask |= PIXEL_HEIGHT;
    }

    uint64_t get_pixel_crop_bottom() const
    {
        return pixel_crop_bottom;
    }

    void set_pixel_crop_bottom(uint64_t pixel_crop_bottom)
    {
        track_video::pixel_crop_bottom = pixel_crop_bottom;
        mask |= PIXEL_CROP_BOTTOM;
    }

    uint64_t get_pixel_crop_top() const
    {
        return pixel_crop_top;
    }

    void set_pixel_crop_top(uint64_t pixel_crop_top)
    {
        track_video::pixel_crop_top = pixel_crop_top;
        mask |= PIXEL_CROP_TOP;
    }

    uint64_t get_pixel_crop_left() const
    {
        return pixel_crop_left;
    }

    void set_pixel_crop_left(uint64_t pixel_crop_left)
    {
        track_video::pixel_crop_left = pixel_crop_left;
        mask |= PIXEL_CROP_LEFT;
    }

    uint64_t get_pixel_crop_right() const
    {
        return pixel_crop_right;
    }

    void set_pixel_crop_right(uint64_t pixel_crop_right)
    {
        track_video::pixel_crop_right = pixel_crop_right;
        mask |= PIXEL_CROP_RIGHT;
    }

    uint64_t get_display_width() const
    {
        return display_width;
    }

    void set_display_width(uint64_t display_width)
    {
        track_video::display_width = display_width;
        mask |= DISPLAY_WIDTH;
    }

    uint64_t get_display_height() const
    {
        return display_height;
    }

    void set_display_height(uint64_t display_height)
    {
        track_video::display_height = display_height;
        mask |= DISPLAY_HEIGHT;
    }

    uint64_t get_display_unit() const
    {
        return display_unit;
    }

    void set_display_unit(uint64_t display_unit)
    {
        track_video::display_unit = display_unit;
        mask |= DISPLAY_UNIT;
    }

    uint64_t get_aspect_ratio_type() const
    {
        return aspect_ratio_type;
    }

    void set_aspect_ratio_type(uint64_t aspect_ratio_type)
    {
        track_video::aspect_ratio_type = aspect_ratio_type;
        mask |= ASPECT_RATIO_TYPE;
    }

    const binary &get_color_space() const
    {
        return color_space;
    }

    void set_color_space(binary &&color_space)
    {
        track_video::color_space = std::move(color_space);
        mask |= COLOR_SPACE;
    }

    const std::unique_ptr<track_video_color> &get_color() const
    {
        return color;
    }

    void set_color(std::unique_ptr<track_video_color> &&color)
    {
        track_video::color = std::move(color);
        mask |= COLOR;
    }

    USING_VALUE_METHOD(FLAG_INTERLACED | FIELD_ORDER | STEREO_MODE | ALPHA_MODE | PIXEL_CROP_BOTTOM | PIXEL_CROP_TOP | PIXEL_CROP_LEFT | PIXEL_CROP_RIGHT | DISPLAY_UNIT | ASPECT_RATIO_TYPE);

private:
    uint64_t flag_interlaced = 0;
    uint64_t field_order = 2;
    uint64_t stereo_mode = 0;
    uint64_t alpha_mode = 0;
    uint64_t pixel_width = 0;
    uint64_t pixel_height = 0;
    uint64_t pixel_crop_bottom = 0;
    uint64_t pixel_crop_top = 0;
    uint64_t pixel_crop_left = 0;
    uint64_t pixel_crop_right = 0;
    uint64_t display_width = 0;
    uint64_t display_height = 0;
    uint64_t display_unit = 0;
    uint64_t aspect_ratio_type = 0;
    binary color_space;
    std::unique_ptr<track_video_color> color;

    int32_t mask = 0;
};


struct track_audio : public value_set_helper<track_audio>
{
    enum
    {
        SAMPLING_FREQUENCY = 1 << 0,
        OUTPUT_SAMPLING_FREQUENCY = 1 << 1,
        CHANNELS = 1 << 2,
        BIT_DEPTH = 1 << 3
    };

    track_audio() = default;

    double get_sampling_frequency() const
    {
        return sampling_frequency;
    }

    void set_sampling_frequency(double sampling_frequency)
    {
        track_audio::sampling_frequency = sampling_frequency;
        mask |= SAMPLING_FREQUENCY;
    }

    double get_output_sampling_frequency() const
    {
        return output_sampling_frequency;
    }

    void set_output_sampling_frequency(double output_sampling_frequency)
    {
        track_audio::output_sampling_frequency = output_sampling_frequency;
        mask |= OUTPUT_SAMPLING_FREQUENCY;
    }

    uint64_t get_channels() const
    {
        return channels;
    }

    void set_channels(uint64_t channels)
    {
        track_audio::channels = channels;
        mask |= CHANNELS;
    }

    uint64_t get_bit_depth() const
    {
        return bit_depth;
    }

    void set_bit_depth(uint64_t bit_depth)
    {
        track_audio::bit_depth = bit_depth;
        mask |= BIT_DEPTH;
    }

    USING_VALUE_METHOD(SAMPLING_FREQUENCY | CHANNELS);

private:
    double sampling_frequency = 8000.0;
    double output_sampling_frequency = 8000.0;
    uint64_t channels = 1;
    uint64_t bit_depth = 0;

    int32_t mask = 0;
};


struct track_operation_plane : public value_set_helper<track_operation_plane>
{
    enum
    {
        UID = 1 << 0,
        TYPE = 1 << 1
    };

    track_operation_plane() = default;

    uint64_t get_uid() const
    {
        return uid;
    }

    void set_uid(uint64_t uid)
    {
        track_operation_plane::uid = uid;
        mask |= UID;
    }

    uint64_t get_type() const
    {
        return type;
    }

    void set_type(uint64_t type)
    {
        track_operation_plane::type = type;
        mask |= TYPE;
    }

    USING_VALUE_METHOD(0);

private:
    uint64_t uid = 0;
    uint64_t type = 0;

    int32_t mask = 0;
};


struct track_operation_combine_planes : public value_set_helper<track_operation_combine_planes>
{
    enum
    {
        TRACK_PLANE = 1 << 0
    };

    track_operation_combine_planes() = default;

    track_operation_combine_planes(track_operation_combine_planes &&) = default;

    track_operation_combine_planes &operator=(track_operation_combine_planes &&)= default;

    const std::vector<track_operation_plane> &get_track_planes() const { return track_planes; }

    void set_track_planes(std::vector<track_operation_plane> &&v)
    {
        track_planes = std::move(v);
        mask |= TRACK_PLANE;
    }

    USING_VALUE_METHOD(0);

private:
    std::vector<track_operation_plane> track_planes;

    int32_t mask = 0;
};

struct track_operation_join_blocks : public value_set_helper<track_operation_join_blocks>
{
    enum
    {
        UID = 1 << 0
    };

    track_operation_join_blocks() = default;

    track_operation_join_blocks(track_operation_join_blocks &&) = default;

    track_operation_join_blocks &operator=(track_operation_join_blocks &&)= default;

    const std::vector<uint64_t> &get_uids() const { return uids; }

    void set_uids(std::vector<uint64_t> &&v)
    {
        uids = std::move(v);
        mask |= UID;
    }

    USING_VALUE_METHOD(0);

private:
    std::vector<uint64_t> uids;

    int32_t mask = 0;
};


struct track_operation : public value_set_helper<track_operation>
{
    enum
    {
        TRACK_COMBINE_PLANES = 1 << 0,
        TRACK_JOIN_BLOCKS = 1 << 1
    };

    track_operation() = default;

    track_operation(track_operation &&) = default;

    track_operation &operator=(track_operation &&)= default;

    const track_operation_combine_planes &get_combine_planes() const
    {
        return combine_planes;
    }

    void set_combine_planes(track_operation_combine_planes &&combine_planes)
    {
        track_operation::combine_planes = std::move(combine_planes);
        mask |= TRACK_COMBINE_PLANES;
    }

    const track_operation_join_blocks &get_join_blocks() const
    {
        return join_blocks;
    }

    void set_join_blocks(track_operation_join_blocks &&join_blocks)
    {
        track_operation::join_blocks = std::move(join_blocks);
        mask |= TRACK_JOIN_BLOCKS;
    }

    USING_VALUE_METHOD(0);

private:
    track_operation_combine_planes combine_planes;
    track_operation_join_blocks join_blocks;

    int32_t mask = 0;
};


struct track_content_compression : public value_set_helper<track_content_compression>
{
    enum
    {
        ALGORITHM = 1 << 0,
        SETTINGS = 1 << 1
    };

    track_content_compression() = default;

    track_content_compression(track_content_compression &&) = default;

    track_content_compression &operator=(track_content_compression &&)= default;

public:
    uint64_t get_algorithm() const
    {
        return algorithm;
    }

    void set_algorithm(uint64_t algorithm)
    {
        track_content_compression::algorithm = algorithm;
        mask |= ALGORITHM;
    }

    const binary &get_settings() const
    {
        return settings;
    }

    void set_settings(binary &&settings)
    {
        track_content_compression::settings = std::move(settings);
        mask |= SETTINGS;
    }

    USING_VALUE_METHOD(ALGORITHM);

private:
    uint64_t algorithm = 0;
    binary settings;

    int32_t mask = 0;
};


struct track_content_encryption : public value_set_helper<track_content_encryption>
{
    enum
    {
        ALGORITHM = 1 << 0,
        KEY_ID = 1 << 1,
        SIGNATURE = 1 << 2,
        SIG_KEY_ID = 1 << 3,
        SIG_ALGORITHM = 1 << 4,
        SIG_HASH_ALGORITHM = 1 << 5
    };

    track_content_encryption() = default;

    track_content_encryption(track_content_encryption &&) = default;

    track_content_encryption &operator=(track_content_encryption &&)= default;

public:
    uint64_t get_algorithm() const
    {
        return algorithm;
    }

    void set_algorithm(uint64_t algorithm)
    {
        track_content_encryption::algorithm = algorithm;
        mask |= ALGORITHM;
    }

    const binary &get_key_id() const
    {
        return key_id;
    }

    void set_key_id(binary &&key_id)
    {
        track_content_encryption::key_id = std::move(key_id);
        mask |= KEY_ID;
    }

    const binary &get_signature() const
    {
        return signature;
    }

    void set_signature(binary &&signature)
    {
        track_content_encryption::signature = std::move(signature);
        mask |= SIGNATURE;
    }

    const binary &get_sig_key_id() const
    {
        return sig_key_id;
    }

    void set_sig_key_id(binary &&sig_key_id)
    {
        track_content_encryption::sig_key_id = std::move(sig_key_id);
        mask |= SIG_KEY_ID;
    }

    uint64_t get_sig_algorithm() const
    {
        return sig_algorithm;
    }

    void set_sig_algorithm(uint64_t sig_algorithm)
    {
        track_content_encryption::sig_algorithm = sig_algorithm;
        mask |= SIG_ALGORITHM;
    }

    uint64_t get_sig_hash_algorithm() const
    {
        return sig_hash_algorithm;
    }

    void set_sig_hash_algorithm(uint64_t sig_hash_algorithm)
    {
        track_content_encryption::sig_hash_algorithm = sig_hash_algorithm;
        mask |= SIG_HASH_ALGORITHM;
    }

    USING_VALUE_METHOD(ALGORITHM | SIG_ALGORITHM | SIG_HASH_ALGORITHM);

private:
    uint64_t algorithm = 0;
    binary key_id;
    binary signature;
    binary sig_key_id;
    uint64_t sig_algorithm = 0;
    uint64_t sig_hash_algorithm = 0;

    int32_t mask = 0;
};


struct track_content_encoding : public value_set_helper<track_content_encoding>
{
    enum
    {
        ORDER = 1 << 0,
        SCOPE = 1 << 1,
        TYPE = 1 << 2,
        COMPRESSION = 1 << 3,
        ENCRYPTION = 1 << 4
    };

    track_content_encoding() = default;

    track_content_encoding(track_content_encoding &&) = default;

    track_content_encoding &operator=(track_content_encoding &&)= default;

public:
    uint64_t get_order() const
    {
        return order;
    }

    void set_order(uint64_t order)
    {
        track_content_encoding::order = order;
        mask |= ORDER;
    }

    uint64_t get_scope() const
    {
        return scope;
    }

    void set_scope(uint64_t scope)
    {
        track_content_encoding::scope = scope;
        mask |= SCOPE;
    }

    uint64_t get_type() const
    {
        return type;
    }

    void set_type(uint64_t type)
    {
        track_content_encoding::type = type;
        mask |= TYPE;
    }

    const std::unique_ptr<track_content_compression> &get_compression() const
    {
        return compression;
    }

    void set_compression(std::unique_ptr<track_content_compression> &&compression)
    {
        track_content_encoding::compression = std::move(compression);
        mask |= COMPRESSION;
    }

    const std::unique_ptr<track_content_encryption> &get_encryption() const
    {
        return encryption;
    }

    void set_encryption(std::unique_ptr<track_content_encryption> &&encryption)
    {
        track_content_encoding::encryption = std::move(encryption);
        mask |= ENCRYPTION;
    }

    USING_VALUE_METHOD(ORDER | SCOPE | TYPE);

private:
    uint64_t order = 0;
    uint64_t scope = 1;
    uint64_t type = 0;
    std::unique_ptr<track_content_compression> compression;
    std::unique_ptr<track_content_encryption> encryption;

    int32_t mask = 0;
};


struct track_content_encodings : public value_set_helper<track_content_encodings>
{
    enum
    {
        CONTENT_ENCODING = 1 << 0
    };

    track_content_encodings() = default;

    track_content_encodings(track_content_encodings &&) = default;

    track_content_encodings &operator=(track_content_encodings &&)= default;

    const ::std::list<track_content_encoding> &get_content_encodings() const { return content_encodings; }

    void set_content_encodings(std::list<track_content_encoding> &&v)
    {
        content_encodings = std::move(v);
        mask |= CONTENT_ENCODING;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<track_content_encoding> content_encodings;

    int32_t mask = 0;
};


struct track_entry : public value_set_helper<track_entry>
{
    enum
    {
        NUMBER = 1 << 0,
        UID = 1 << 1,
        TYPE = 1 << 2,
        FLAG_ENABLED = 1 << 3,
        FLAG_DEFAULT = 1 << 4,
        FLAG_FORCED = 1 << 5,
        FLAG_LACING = 1 << 6,
        MIN_CACHE = 1 << 7,
        MAX_CACHE = 1 << 8,
        DEFAULT_DURATION = 1 << 9,
        DEFAULT_DECODED_FIELD_DURATION = 1 << 10,
        MAX_BLOCK_ADDITION_ID = 1 << 11,
        NAME = 1 << 12,
        LANGUAGE = 1 << 13,
        CODEC_ID = 1 << 14,
        CODEC_PRIVATE = 1 << 15,
        CODEC_NAME = 1 << 16,
        ATTACHMENT_LINK = 1 << 17,
        CODEC_DECODE_ALL = 1 << 18,
        OVERLAY = 1 << 19,
        CODEC_DELAY = 1 << 20,
        SEEK_PRE_ROLL = 1 << 21,
        TRANSLATE = 1 << 22,
        VIDEO = 1 << 23,
        AUDIO = 1 << 24,
        OPERATION = 1 << 25,
        CONTENT_ENCODINGS = 1 << 26
    };

    track_entry() : language("eng") { }

    track_entry(track_entry &&) = default;

    track_entry &operator=(track_entry &&)= default;

public:
    uint64_t get_number() const
    {
        return number;
    }

    void set_number(uint64_t number)
    {
        track_entry::number = number;
        mask |= NUMBER;
    }

    uint64_t get_uid() const
    {
        return uid;
    }

    void set_uid(uint64_t uid)
    {
        track_entry::uid = uid;
        mask |= UID;
    }

    uint64_t get_type() const
    {
        return type;
    }

    void set_type(uint64_t type)
    {
        track_entry::type = type;
        mask |= TYPE;
    }

    uint64_t get_flag_enabled() const
    {
        return flag_enabled;
    }

    void set_flag_enabled(uint64_t flag_enabled)
    {
        track_entry::flag_enabled = flag_enabled;
        mask |= FLAG_ENABLED;
    }

    uint64_t get_flag_default() const
    {
        return flag_default;
    }

    void set_flag_default(uint64_t flag_default)
    {
        track_entry::flag_default = flag_default;
        mask |= FLAG_DEFAULT;
    }

    uint64_t get_flag_forced() const
    {
        return flag_forced;
    }

    void set_flag_forced(uint64_t flag_forced)
    {
        track_entry::flag_forced = flag_forced;
        mask |= FLAG_FORCED;
    }

    uint64_t get_flag_lacing() const
    {
        return flag_lacing;
    }

    void set_flag_lacing(uint64_t flag_lacing)
    {
        track_entry::flag_lacing = flag_lacing;
        mask |= FLAG_LACING;
    }

    uint64_t get_min_cache() const
    {
        return min_cache;
    }

    void set_min_cache(uint64_t min_cache)
    {
        track_entry::min_cache = min_cache;
        mask |= MIN_CACHE;
    }

    uint64_t get_max_cache() const
    {
        return max_cache;
    }

    void set_max_cache(uint64_t max_cache)
    {
        track_entry::max_cache = max_cache;
        mask |= MAX_CACHE;
    }

    uint64_t get_default_duration() const
    {
        return default_duration;
    }

    void set_default_duration(uint64_t default_duration)
    {
        track_entry::default_duration = default_duration;
        mask |= DEFAULT_DURATION;
    }

    uint64_t get_default_decoded_field_duration() const
    {
        return default_decoded_field_duration;
    }

    void set_default_decoded_field_duration(uint64_t default_decoded_field_duration)
    {
        track_entry::default_decoded_field_duration = default_decoded_field_duration;
        mask |= DEFAULT_DECODED_FIELD_DURATION;
    }

    uint64_t get_max_block_addition_id() const
    {
        return max_block_addition_id;
    }

    void set_max_block_addition_id(uint64_t max_block_addition_id)
    {
        track_entry::max_block_addition_id = max_block_addition_id;
        mask |= MAX_BLOCK_ADDITION_ID;
    }

    const std::string &get_name() const
    {
        return name;
    }

    void set_name(std::string &&name)
    {
        track_entry::name = std::move(name);
        mask |= NAME;
    }

    const std::string &get_language() const
    {
        return language;
    }

    void set_language(std::string &&language)
    {
        track_entry::language = std::move(language);
        mask |= LANGUAGE;
    }

    const std::string &get_codec_id() const
    {
        return codec_id;
    }

    void set_codec_id(std::string &&codec_id)
    {
        track_entry::codec_id = std::move(codec_id);
        mask |= CODEC_ID;
    }

    const binary &get_codec_private() const
    {
        return codec_private;
    }

    void set_codec_private(binary &&codec_private)
    {
        track_entry::codec_private = std::move(codec_private);
        mask |= CODEC_PRIVATE;
    }

    const std::string &get_codec_name() const
    {
        return codec_name;
    }

    void set_codec_name(std::string &&codec_name)
    {
        track_entry::codec_name = std::move(codec_name);
        mask |= CODEC_NAME;
    }

    uint64_t get_attachment_link() const
    {
        return attachment_link;
    }

    void set_attachment_link(uint64_t attachment_link)
    {
        track_entry::attachment_link = attachment_link;
        mask |= ATTACHMENT_LINK;
    }

    uint64_t get_codec_decode_all() const
    {
        return codec_decode_all;
    }

    void set_codec_decode_all(uint64_t codec_decode_all)
    {
        track_entry::codec_decode_all = codec_decode_all;
        mask |= CODEC_DECODE_ALL;
    }

    const std::vector<uint64_t> &get_overlays() const
    {
        return overlays;
    }

    void set_overlays(std::vector<uint64_t> &&overlays)
    {
        track_entry::overlays = std::move(overlays);
        mask |= OVERLAY;
    }

    uint64_t get_codec_delay() const
    {
        return codec_delay;
    }

    void set_codec_delay(uint64_t codec_delay)
    {
        track_entry::codec_delay = codec_delay;
        mask |= CODEC_DELAY;
    }

    uint64_t get_seek_pre_roll() const
    {
        return seek_pre_roll;
    }

    void set_seek_pre_roll(uint64_t seek_pre_roll)
    {
        track_entry::seek_pre_roll = seek_pre_roll;
        mask |= SEEK_PRE_ROLL;
    }

    const std::list<track_translate> &get_translate() const
    {
        return translate;
    }

    void set_translate(std::list<track_translate> &&translate)
    {
        track_entry::translate = std::move(translate);
        mask |= TRANSLATE;
    }

    const std::unique_ptr<track_video> &get_video() const
    {
        return video;
    }

    void set_video(std::unique_ptr<track_video> &&video)
    {
        track_entry::video = std::move(video);
        mask |= VIDEO;
    }

    const std::unique_ptr<track_audio> &get_audio() const
    {
        return audio;
    }

    void set_audio(std::unique_ptr<track_audio> &&audio)
    {
        track_entry::audio = std::move(audio);
        mask |= AUDIO;
    }

    const std::unique_ptr<track_operation> &get_operation() const
    {
        return operation;
    }

    void set_operation(std::unique_ptr<track_operation> &&operation)
    {
        track_entry::operation = std::move(operation);
        mask |= OPERATION;
    }

    const std::unique_ptr<track_content_encodings> &get_content_encodings() const
    {
        return content_encodings;
    }

    void set_content_encodings(std::unique_ptr<track_content_encodings> &&content_encodings)
    {
        track_entry::content_encodings = std::move(content_encodings);
        mask |= CONTENT_ENCODINGS;
    }

    USING_VALUE_METHOD(FLAG_ENABLED | FLAG_DEFAULT | FLAG_FORCED | FLAG_LACING | MIN_CACHE | MAX_BLOCK_ADDITION_ID | LANGUAGE | CODEC_DECODE_ALL | CODEC_DELAY | SEEK_PRE_ROLL);

private:
    uint64_t number = 0;
    uint64_t uid = 0;
    uint64_t type = 0;
    uint64_t flag_enabled = 1;
    uint64_t flag_default = 1;
    uint64_t flag_forced = 0;
    uint64_t flag_lacing = 1;
    uint64_t min_cache = 0;
    uint64_t max_cache = 0;
    uint64_t default_duration = 0;
    uint64_t default_decoded_field_duration = 0;
    uint64_t max_block_addition_id = 0;
    std::string name;
    std::string language;
    std::string codec_id;
    binary codec_private;
    std::string codec_name;
    uint64_t attachment_link = 0;
    uint64_t codec_decode_all = 1;
    std::vector<uint64_t> overlays;
    uint64_t codec_delay = 0;
    uint64_t seek_pre_roll = 0;
    std::list<track_translate> translate;
    std::unique_ptr<track_video> video;
    std::unique_ptr<track_audio> audio;
    std::unique_ptr<track_operation> operation;
    std::unique_ptr<track_content_encodings> content_encodings;

    int32_t mask = 0;
};


struct track : public value_set_helper<track>
{
    enum
    {
        TRACK_ENTRY = 1 << 0,
    };

    track() = default;

    track(track &&) = default;

    track &operator=(track &&)= default;

    const std::list<track_entry> &get_entries() const
    {
        return entries;
    }

    void set_entries(std::list<track_entry> &&entries)
    {
        track::entries = std::move(entries);
        mask |= TRACK_ENTRY;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<track_entry> entries;

    int32_t mask = 0;
};


struct cue_track_reference : public value_set_helper<cue_track_reference>
{
    enum
    {
        REF_TIME = 1 << 0
    };

    cue_track_reference() = default;

public:
    uint64_t get_ref_time() const
    {
        return ref_time;
    }

    void set_ref_time(uint64_t ref_time)
    {
        cue_track_reference::ref_time = ref_time;
        mask |= REF_TIME;
    }

    USING_VALUE_METHOD(0);

private:
    uint64_t ref_time = 0;

    int32_t mask = 0;
};


struct cue_track_position : public value_set_helper<cue_track_position>
{
    enum
    {
        TRACK = 1 << 0,
        CLUSTER_POSITION = 1 << 1,
        RELATIVE_POSITION = 1 << 2,
        DURATION = 1 << 3,
        BLOCK_NUMBER = 1 << 4,
        CODEC_STATE = 1 << 5,
        REFERENCE = 1 << 6
    };

    cue_track_position() = default;

    cue_track_position(cue_track_position &&) = default;

    cue_track_position &operator=(cue_track_position &&)= default;

public:
    uint64_t get_track() const
    {
        return track;
    }

    void set_track(uint64_t track)
    {
        cue_track_position::track = track;
        mask |= TRACK;
    }

    uint64_t get_cluster_position() const
    {
        return cluster_position;
    }

    void set_cluster_position(uint64_t cluster_position)
    {
        cue_track_position::cluster_position = cluster_position;
        mask |= CLUSTER_POSITION;
    }

    uint64_t get_relative_position() const
    {
        return relative_position;
    }

    void set_relative_position(uint64_t relative_position)
    {
        cue_track_position::relative_position = relative_position;
        mask |= RELATIVE_POSITION;
    }

    uint64_t get_duration() const
    {
        return duration;
    }

    void set_duration(uint64_t duration)
    {
        cue_track_position::duration = duration;
        mask |= DURATION;
    }

    uint64_t get_block_number() const
    {
        return block_number;
    }

    void set_block_number(uint64_t block_number)
    {
        cue_track_position::block_number = block_number;
        mask |= BLOCK_NUMBER;
    }

    uint64_t get_codec_state() const
    {
        return codec_state;
    }

    void set_codec_state(uint64_t codec_state)
    {
        cue_track_position::codec_state = codec_state;
        mask |= CODEC_STATE;
    }

    const std::list<cue_track_reference> &get_references() const
    {
        return references;
    }

    void set_references(std::list<cue_track_reference> &&references)
    {
        cue_track_position::references = std::move(references);
        mask |= REFERENCE;
    }

    USING_VALUE_METHOD(BLOCK_NUMBER | CODEC_STATE);

private:
    uint64_t track = 0;
    uint64_t cluster_position = 0;
    uint64_t relative_position = 0;
    uint64_t duration = 0;
    uint64_t block_number = 1;
    uint64_t codec_state = 0;
    std::list<cue_track_reference> references;

    int32_t mask = 0;
};


struct cue_point : public value_set_helper<cue_point>
{
    enum
    {
        TIME = 1 << 0,
        TRACK_POSITIONS = 1 << 1
    };

    cue_point() = default;

    cue_point(cue_point &&) = default;

    cue_point &operator=(cue_point &&)= default;

public:
    uint64_t get_time() const
    {
        return time;
    }

    void set_time(uint64_t time)
    {
        cue_point::time = time;
        mask |= TIME;
    }

    const std::list<cue_track_position> &get_track_positions() const
    {
        return track_positions;
    }

    void set_track_positions(std::list<cue_track_position> &&track_positions)
    {
        cue_point::track_positions = std::move(track_positions);
        mask |= TRACK_POSITIONS;
    }

    USING_VALUE_METHOD(0);

private:
    uint64_t time = 0;
    std::list<cue_track_position> track_positions;

    int32_t mask = 0;
};


struct cue : public value_set_helper<cue>
{
    enum
    {
        CUE_POINT = 1 << 0
    };

    cue() = default;

    cue(cue &&) = default;

    cue &operator=(cue &&)= default;

public:
    const std::list<cue_point> &get_cue_points() const
    {
        return cue_points;
    }

    void set_cue_points(std::list<cue_point> &&cue_points)
    {
        cue::cue_points = std::move(cue_points);
        mask |= CUE_POINT;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<cue_point> cue_points;

    int32_t mask = 0;
};


struct attached_file : public value_set_helper<attached_file>
{
    enum
    {
        DESCRIPTION = 1 << 0,
        NAME = 1 << 1,
        MINE_TYPE = 1 << 2,
        DATA = 1 << 3,
        UID = 1 << 4
    };

    attached_file() = default;

    attached_file(attached_file &&) = default;

    attached_file &operator=(attached_file &&)= default;

public:
    const std::string &get_description() const
    {
        return description;
    }

    void set_description(std::string &&description)
    {
        attached_file::description = std::move(description);
        mask |= DESCRIPTION;
    }

    const std::string &get_name() const
    {
        return name;
    }

    void set_name(std::string &&name)
    {
        attached_file::name = std::move(name);
        mask |= NAME;
    }

    const std::string &get_mine_type() const
    {
        return mine_type;
    }

    void set_mine_type(std::string &&mine_type)
    {
        attached_file::mine_type = std::move(mine_type);
        mask |= MINE_TYPE;
    }

    const binary &get_data() const
    {
        return data;
    }

    void set_data(binary &&data)
    {
        attached_file::data = std::move(data);
        mask |= DATA;
    }

    uint64_t get_uid() const
    {
        return uid;
    }

    void set_uid(uint64_t uid)
    {
        attached_file::uid = uid;
        mask |= UID;
    }

    USING_VALUE_METHOD(0);

private:
    std::string description;
    std::string name;
    std::string mine_type;
    binary data;
    uint64_t uid = 0;

    int32_t mask = 0;
};


struct attachment : public value_set_helper<attachment>
{
    enum
    {
        ATTACHED_FILE = 1 << 0
    };

    attachment() = default;

    attachment(attachment &&) = default;

    attachment &operator=(attachment &&)= default;

public:
    const std::list<attached_file> &get_attached_files() const
    {
        return attached_files;
    }

    void set_attached_files(std::list<attached_file> &&attached_files)
    {
        attachment::attached_files = std::move(attached_files);
        mask |= ATTACHED_FILE;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<attached_file> attached_files;

    int32_t mask = 0;
};


struct chapter_track : public value_set_helper<chapter_track>
{
    enum
    {
        TRACK_NUMBER = 1 << 0
    };

    chapter_track() = default;

    chapter_track(chapter_track &&) = default;

    chapter_track &operator=(chapter_track &&)= default;

public:
    const std::vector<uint64_t> &get_track_numbers() const
    {
        return track_numbers;
    }

    void set_track_numbers(std::vector<uint64_t> &&track_numbers)
    {
        chapter_track::track_numbers = std::move(track_numbers);
        mask |= TRACK_NUMBER;
    }

    USING_VALUE_METHOD(0);

private:
    std::vector<uint64_t> track_numbers;

    int32_t mask = 0;
};


struct chapter_display : public value_set_helper<chapter_display>
{
    enum
    {
        STRING = 1 << 0,
        LANGUAGE = 1 << 1,
        COUNTRY = 1 << 2
    };

    chapter_display() = default;

    chapter_display(chapter_display &&) = default;

    chapter_display &operator=(chapter_display &&)= default;

public:
    const std::string &get_string() const
    {
        return string;
    }

    void set_string(std::string &&string)
    {
        chapter_display::string = std::move(string);
        mask |= STRING;
    }

    const std::list<std::string> &get_languages() const
    {
        return languages;
    }

    void set_languages(std::list<std::string> &&languages)
    {
        chapter_display::languages = std::move(languages);
        mask |= LANGUAGE;
    }

    const std::list<std::string> &get_countries() const
    {
        return countries;
    }

    void set_countries(std::list<std::string> &&countries)
    {
        chapter_display::countries = std::move(countries);
        mask |= COUNTRY;
    }

    USING_VALUE_METHOD(0);

private:
    std::string string;
    std::list<std::string> languages;
    std::list<std::string> countries;

    int32_t mask = 0;
};


struct chapter_process_command : public value_set_helper<chapter_process_command>
{
    enum
    {
        TIME = 1 << 0,
        DATA = 1 << 1
    };

    chapter_process_command() = default;

    chapter_process_command(chapter_process_command &&) = default;

    chapter_process_command &operator=(chapter_process_command &&)= default;

public:
    uint64_t get_time() const
    {
        return time;
    }

    void set_time(uint64_t time)
    {
        chapter_process_command::time = time;
        mask |= TIME;
    }

    const binary &get_data() const
    {
        return data;
    }

    void set_data(binary &&data)
    {
        chapter_process_command::data = std::move(data);
        mask |= DATA;
    }

    USING_VALUE_METHOD(0);

private:
    uint64_t time = 0;
    binary data;

    int32_t mask = 0;
};


struct chapter_process : public value_set_helper<chapter_process>
{
    enum
    {
        CODEC_ID = 1 << 0,
        PRIVATE = 1 << 1,
        COMMAND = 1 << 2
    };

    chapter_process() = default;

    chapter_process(chapter_process &&) = default;

    chapter_process &operator=(chapter_process &&)= default;

public:
    uint64_t get_codec_id() const
    {
        return codec_id;
    }

    void set_codec_id(uint64_t codec_id)
    {
        chapter_process::codec_id = codec_id;
        mask |= CODEC_ID;
    }

    const binary &get_process_private() const
    {
        return process_private;
    }

    void set_process_private(binary &&process_private)
    {
        chapter_process::process_private = std::move(process_private);
        mask |= PRIVATE;
    }

    const std::list<chapter_process_command> &get_commands() const
    {
        return commands;
    }

    void set_commands(std::list<chapter_process_command> &&commands)
    {
        chapter_process::commands = std::move(commands);
        mask |= COMMAND;
    }

    USING_VALUE_METHOD(CODEC_ID);

private:
    uint64_t codec_id = 0;
    binary process_private;
    std::list<chapter_process_command> commands;

    int32_t mask = 0;
};


struct chapter_atom : public value_set_helper<chapter_atom>
{
    enum
    {
        CHAPTER_ATOM = 1 << 0,
        UID = 1 << 1,
        STRING_UID = 1 << 2,
        TIME_START = 1 << 3,
        TIME_END = 1 << 4,
        FLAG_HIDDEN = 1 << 5,
        FLAG_ENABLED = 1 << 6,
        SEGMENT_UID = 1 << 7,
        SEGMENT_EDITION_UID = 1 << 8,
        PHYSICAL_EQUIV = 1 << 9,
        TRACK = 1 << 10,
        DISPLAY = 1 << 11,
        PROCESS = 1 << 12
    };

    chapter_atom() = default;

    chapter_atom(chapter_atom &&) = default;

    chapter_atom &operator=(chapter_atom &&)= default;

public:
    const std::list<chapter_atom> &get_atoms() const
    {
        return atoms;
    }

    void set_atoms(std::list<chapter_atom> &&atoms)
    {
        chapter_atom::atoms = std::move(atoms);
        mask |= CHAPTER_ATOM;
    }

    uint64_t get_uid() const
    {
        return uid;
    }

    void set_uid(uint64_t uid)
    {
        chapter_atom::uid = uid;
        mask |= UID;
    }

    const std::string &get_string_uid() const
    {
        return string_uid;
    }

    void set_string_uid(std::string &&string_uid)
    {
        chapter_atom::string_uid = std::move(string_uid);
        mask |= STRING_UID;
    }

    uint64_t get_time_start() const
    {
        return time_start;
    }

    void set_time_start(uint64_t time_start)
    {
        chapter_atom::time_start = time_start;
        mask |= TIME_START;
    }

    uint64_t get_time_end() const
    {
        return time_end;
    }

    void set_time_end(uint64_t time_end)
    {
        chapter_atom::time_end = time_end;
        mask |= TIME_END;
    }

    uint64_t get_flag_hidden() const
    {
        return flag_hidden;
    }

    void set_flag_hidden(uint64_t flag_hidden)
    {
        chapter_atom::flag_hidden = flag_hidden;
        mask |= FLAG_HIDDEN;
    }

    uint64_t get_flag_enabled() const
    {
        return flag_enabled;
    }

    void set_flag_enabled(uint64_t flag_enabled)
    {
        chapter_atom::flag_enabled = flag_enabled;
        mask |= FLAG_ENABLED;
    }

    const binary &get_segment_uid() const
    {
        return segment_uid;
    }

    void set_segment_uid(binary &&segment_uid)
    {
        chapter_atom::segment_uid = std::move(segment_uid);
        mask |= SEGMENT_UID;
    }

    uint64_t get_segment_edition_uid() const
    {
        return segment_edition_uid;
    }

    void set_segment_edition_uid(uint64_t segment_edition_uid)
    {
        chapter_atom::segment_edition_uid = segment_edition_uid;
        mask |= SEGMENT_EDITION_UID;
    }

    uint64_t get_physical_equiv() const
    {
        return physical_equiv;
    }

    void set_physical_equiv(uint64_t physical_equiv)
    {
        chapter_atom::physical_equiv = physical_equiv;
        mask |= PHYSICAL_EQUIV;
    }

    const chapter_track &get_track() const
    {
        return track;
    }

    void set_track(chapter_track &&track)
    {
        chapter_atom::track = std::move(track);
        mask |= TRACK;
    }

    const std::list<chapter_display> &get_displays() const
    {
        return displays;
    }

    void set_displays(std::list<chapter_display> &&displays)
    {
        chapter_atom::displays = std::move(displays);
        mask |= DISPLAY;
    }

    const std::list<chapter_process> &get_processes() const
    {
        return processes;
    }

    void set_processes(std::list<chapter_process> &&processes)
    {
        chapter_atom::processes = std::move(processes);
        mask |= PROCESS;
    }

    USING_VALUE_METHOD(FLAG_HIDDEN | FLAG_ENABLED);

private:
    std::list<chapter_atom> atoms;
    uint64_t uid = 0;
    std::string string_uid;
    uint64_t time_start = 0;
    uint64_t time_end = 0;
    uint64_t flag_hidden = 0;
    uint64_t flag_enabled = 1;
    binary segment_uid;
    uint64_t segment_edition_uid = 0;
    uint64_t physical_equiv = 0;
    chapter_track track;
    std::list<chapter_display> displays;
    std::list<chapter_process> processes;

    int32_t mask = 0;
};


struct chapter_edition_entry : public value_set_helper<chapter_edition_entry>
{
    enum
    {
        UID = 1 << 0,
        FLAG_HIDDEN = 1 << 1,
        FLAG_DEFAULT = 1 << 2,
        FLAG_ORDERED = 1 << 3,
        CHAPTER_ATOM = 1 << 4
    };

    chapter_edition_entry() = default;

    chapter_edition_entry(chapter_edition_entry &&) = default;

    chapter_edition_entry &operator=(chapter_edition_entry &&)= default;

public:
    uint64_t get_uid() const
    {
        return uid;
    }

    void set_uid(uint64_t uid)
    {
        chapter_edition_entry::uid = uid;
        mask |= UID;
    }

    uint64_t get_flag_hidden() const
    {
        return flag_hidden;
    }

    void set_flag_hidden(uint64_t flag_hidden)
    {
        chapter_edition_entry::flag_hidden = flag_hidden;
        mask |= FLAG_HIDDEN;
    }

    uint64_t get_flag_default() const
    {
        return flag_default;
    }

    void set_flag_default(uint64_t flag_default)
    {
        chapter_edition_entry::flag_default = flag_default;
        mask |= FLAG_DEFAULT;
    }

    uint64_t get_flag_ordered() const
    {
        return flag_ordered;
    }

    void set_flag_ordered(uint64_t flag_ordered)
    {
        chapter_edition_entry::flag_ordered = flag_ordered;
        mask |= FLAG_ORDERED;
    }

    const std::list<chapter_atom> &get_chapter_atoms() const
    {
        return chapter_atoms;
    }

    void set_chapter_atoms(std::list<chapter_atom> &&chapter_atoms)
    {
        chapter_edition_entry::chapter_atoms = std::move(chapter_atoms);
        mask |= CHAPTER_ATOM;
    }

    USING_VALUE_METHOD(FLAG_HIDDEN | FLAG_DEFAULT | FLAG_ORDERED);

private:
    uint64_t uid = 0;
    uint64_t flag_hidden = 0;
    uint64_t flag_default = 0;
    uint64_t flag_ordered = 0;
    std::list<chapter_atom> chapter_atoms;

    int32_t mask = 0;
};


struct chapter : public value_set_helper<chapter>
{
    enum
    {
        EDITION_ENTRY = 1 << 0
    };

    chapter() = default;

    chapter(chapter &&) = default;

    chapter &operator=(chapter &&)= default;

public:
    const std::list<chapter_edition_entry> &get_edition_entries() const
    {
        return edition_entries;
    }

    void set_edition_entries(std::list<chapter_edition_entry> &&edition_entries)
    {
        chapter::edition_entries = std::move(edition_entries);
        mask |= EDITION_ENTRY;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<chapter_edition_entry> edition_entries;

    int32_t mask = 0;
};


struct tag_target : public value_set_helper<tag_target>
{
    enum
    {
        TYPE_VALUE = 1 << 0,
        TYPE = 1 << 1,
        UID = 1 << 2
    };

    tag_target() = default;

    tag_target(tag_target &&) = default;

    tag_target &operator=(tag_target &&)= default;

public:
    uint64_t get_type_value() const
    {
        return type_value;
    }

    void set_type_value(uint64_t type_value)
    {
        tag_target::type_value = type_value;
        mask |= TYPE_VALUE;
    }

    const std::string &get_type() const
    {
        return type;
    }

    void set_type(std::string &&type)
    {
        tag_target::type = std::move(type);
        mask |= TYPE;
    }

    const std::vector<uint64_t> &get_uids() const
    {
        return uids;
    }

    void set_uids(std::vector<uint64_t> &&uids)
    {
        tag_target::uids = std::move(uids);
        mask |= UID;
    }

    USING_VALUE_METHOD(TYPE_VALUE);

private:
    uint64_t type_value = 50;
    std::string type;
    std::vector<uint64_t> uids;

    int32_t mask = 0;
};


struct simple_tag : public value_set_helper<simple_tag>
{
    enum
    {
        SIMPLE_TAG = 1 << 0,
        NAME = 1 << 1,
        LANGUAGE = 1 << 2,
        DEFAULT = 1 << 3,
        STRING = 1 << 4,
        BINARY = 1 << 5
    };

    simple_tag() : language("und") { }

    simple_tag(simple_tag &&) = default;

    simple_tag &operator=(simple_tag &&)= default;

public:
    const std::list<simple_tag> &get_tags() const
    {
        return tags;
    }

    void set_tags(std::list<simple_tag> &&tags)
    {
        simple_tag::tags = std::move(tags);
        mask |= SIMPLE_TAG;
    }

    const std::string &get_name() const
    {
        return name;
    }

    void set_name(std::string &&name)
    {
        simple_tag::name = std::move(name);
        mask |= NAME;
    }

    const std::string &get_language() const
    {
        return language;
    }

    void set_language(std::string &&language)
    {
        simple_tag::language = std::move(language);
        mask |= LANGUAGE;
    }

    uint64_t get_tag_default() const
    {
        return tag_default;
    }

    void set_tag_default(uint64_t tag_default)
    {
        simple_tag::tag_default = tag_default;
        mask |= DEFAULT;
    }

    const std::string &get_string() const
    {
        return string;
    }

    void set_string(std::string &&string)
    {
        simple_tag::string = std::move(string);
        mask |= STRING;
    }

    const binary &get_bin() const
    {
        return bin;
    }

    void set_bin(binary &&bin)
    {
        simple_tag::bin = std::move(bin);
        mask |= BINARY;
    }

    USING_VALUE_METHOD(LANGUAGE | DEFAULT);

private:
    std::list<simple_tag> tags;
    std::string name;
    std::string language;
    uint64_t tag_default = 1;
    std::string string;
    binary bin;

    int32_t mask = 0;
};


struct tag : public value_set_helper<tag>
{
    enum
    {
        TARGETS = 1 << 0,
        SIMPLE_TAG = 1 << 1
    };

    tag() = default;

    tag(tag &&) = default;

    tag &operator=(tag &&)= default;

public:
    const tag_target &get_target() const
    {
        return target;
    }

    void set_target(tag_target &&target)
    {
        tag::target = std::move(target);
        mask |= TARGETS;
    }

    const std::list<simple_tag> &get_simple_tags() const
    {
        return simple_tags;
    }

    void set_simple_tags(std::list<simple_tag> &&simple_tags)
    {
        tag::simple_tags = std::move(simple_tags);
        mask |= SIMPLE_TAG;
    }

    USING_VALUE_METHOD(0);

private:
    tag_target target;
    std::list<simple_tag> simple_tags;

    int32_t mask = 0;
};


struct tags : public value_set_helper<tags>
{
    enum
    {
        TAG = 1 << 0
    };

    tags() = default;

    tags(tags &&) = default;

    tags &operator=(tags &&)= default;

public:
    const std::list<tag> &get_tag_list() const
    {
        return tag_list;
    }

    void set_tag_list(std::list<tag> &&tag_list)
    {
        tags::tag_list = std::move(tag_list);
        mask |= TAG;
    }

    USING_VALUE_METHOD(0);

private:
    std::list<tag> tag_list;

    int32_t mask = 0;
};


#undef USING_VALUE_METHOD

}
}
}


#undef ELEMENT_PROPERTY


#endif //MOCK_PLAYER_ELEMENT_HPP
