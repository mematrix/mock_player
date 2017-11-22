//
// Created by Charles on 2017/11/17.
//

#ifndef MOCK_PLAYER_ELEMENT_HPP
#define MOCK_PLAYER_ELEMENT_HPP


#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <memory>

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
    int32_t mask = 0; \
    int32_t get_mask() const { return mask; } \
    using value_set_helper::is_set; \
    using value_set_helper::is_user_set; \
    using value_set_helper::is_all_set


enum class element_type
{
    UNSIGNED_INTEGER = 0,
    SIGNED_INTEGER,
    FLOAT,
    STRING,
    BINARY,
    GENERIC_TYPE
};

class undefined_class_suffix;

union pointer_type_detect
{
    void *_t_pointer;
    const void *_t_const_pointer;
    int undefined_class_suffix::* _t_class_member_pointer;
};

union any_pointer_data
{
    // inspired by mingw gcc. source header: <std_function.h>

    void *access() noexcept { return &_pod_data[0]; }

    const void *access() const noexcept { return &_pod_data[0]; }

    template<typename Tp>
    Tp &access() noexcept { return *static_cast<Tp *>(access()); }

    template<typename Tp>
    const Tp &access() const noexcept { return *static_cast<const Tp *>(access()); }

    pointer_type_detect _unused_value;
    char _pod_data[sizeof(pointer_type_detect)];
};

struct element_identify
{
    element_type type;
    int32_t name_mask;
    uint32_t ebml_id;
    any_pointer_data class_member_ptr;

    template<typename Tp>
    element_identify(element_type t, Tp ptr, int32_t mask, uint32_t id) noexcept : type(t), name_mask(mask), ebml_id(id)
    {
        class_member_ptr.access<Tp>() = ptr;
    }
};


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

    ebml_header(ebml_header &&) = default;

    ebml_header &operator=(ebml_header &&) = default;

    USING_VALUE_METHOD(VERSION | READ_VERSION | MAX_ID_LENGTH | MAX_SIZE_LENGTH | DOC_TYPE | DOC_TYPE_VERSION | DOC_TYPE_READ_VERSION);

    uint64_t version = 1;
    uint64_t read_version = 1;
    uint64_t max_id_length = 4;
    uint64_t max_size_length = 8;
    std::string doc_type;
    uint64_t doc_type_version = 1;
    uint64_t doc_type_read_version = 1;
};

const element_identify *get_ebml_header_identifies();

constexpr uint32_t ebml_header_identifies_size = 7;


struct meta_seek : public value_set_helper<meta_seek>
{
    enum
    {
        ID = 1 << 0,
        POSITION = 1 << 1
    };

    meta_seek() = default;

    meta_seek(meta_seek &&) = default;

    meta_seek &operator=(meta_seek &&) = default;

    USING_VALUE_METHOD(0);

    binary id;
    uint64_t position = 0;
};

const element_identify *get_meta_seek_identifies();

constexpr uint32_t meta_seek_identifies_size = 2;


struct meta_seek_head : public value_set_helper<meta_seek_head>
{
    enum
    {
        SEEK = 1 << 0
    };

    meta_seek_head() = default;

    meta_seek_head(meta_seek_head &&) = default;

    meta_seek_head &operator=(meta_seek_head &&) = default;

    USING_VALUE_METHOD(0);

    std::list<meta_seek> seeks;
};

const element_identify *get_meta_seek_head_identifies();

constexpr uint32_t meta_seek_head_identifies_size = 1;

struct segment_chapter_translate : public value_set_helper<segment_chapter_translate>
{
    enum
    {
        EDITION_UID = 1 << 0,
        CODEC = 1 << 1,
        ID = 1 << 2
    };

    segment_chapter_translate() = default;

    segment_chapter_translate(segment_chapter_translate &&) = default;

    segment_chapter_translate &operator=(segment_chapter_translate &&) = default;

    USING_VALUE_METHOD(0);

    std::vector<uint64_t> edition_uids;
    uint64_t codec = 0;
    binary id;
};

const element_identify *get_segment_chapter_translate_identifies();

constexpr uint32_t segment_chapter_translate_identifies_size = 3;


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

    segment_info(segment_info &&) = default;

    segment_info &operator=(segment_info &&) = default;

    USING_VALUE_METHOD(TIMECODE_SCALE);

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
};

const element_identify *get_segment_info_identifies();

constexpr uint32_t segment_info_identifies_size = 14;


struct cluster_silent_tracks : public value_set_helper<cluster_silent_tracks>
{
    enum
    {
        NUMBER = 1 << 0
    };

    cluster_silent_tracks() = default;

    cluster_silent_tracks(cluster_silent_tracks &&) = default;

    cluster_silent_tracks &operator=(cluster_silent_tracks &&) = default;

    USING_VALUE_METHOD(0);

    std::vector<uint64_t> numbers;
};

const element_identify *get_cluster_silent_tracks_identifies();

constexpr uint32_t cluster_silent_tracks_identifies_size = 1;


struct cluster_block_more : public value_set_helper<cluster_block_more>
{
    enum
    {
        ADD_ID = 1 << 0,
        ADDITIONAL = 1 << 1
    };

    cluster_block_more() = default;

    cluster_block_more(cluster_block_more &&) = default;

    cluster_block_more &operator=(cluster_block_more &&) = default;

    USING_VALUE_METHOD(ADD_ID);

    uint64_t add_id = 1;
    binary additional;
};

const element_identify *get_cluster_block_more_identifies();

constexpr uint32_t cluster_block_more_identifies_size = 2;


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

    USING_VALUE_METHOD(0);

    std::list<cluster_block_more> block_more;
};

const element_identify *get_cluster_block_additions_identifies();

constexpr uint32_t cluster_block_additions_identifies_size = 1;


struct cluster_time_slice : public value_set_helper<cluster_time_slice>
{
    enum
    {
        LACE_NUMBER = 1 << 0
    };

    cluster_time_slice() = default;

    USING_VALUE_METHOD(LACE_NUMBER);

    uint64_t lace_number = 0;
};

const element_identify *get_cluster_time_slice_identifies();

constexpr uint32_t cluster_time_slice_identifies_size = 1;


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

    USING_VALUE_METHOD(0);

    std::list<cluster_time_slice> time_slices;
};

const element_identify *get_cluster_slices_identifies();

constexpr uint32_t cluster_slices_identifies_size = 1;


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

    USING_VALUE_METHOD(REFERENCE_PRIORITY);

    binary block;
    cluster_block_additions additions;
    uint64_t duration = 0;
    uint64_t reference_priority = 0;
    int64_t reference_block = 0;
    binary codec_state;
    int64_t discard_padding = 0;
    cluster_slices slices;
};

const element_identify *get_cluster_block_identifies();

constexpr uint32_t cluster_block_identifies_size = 8;


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

    USING_VALUE_METHOD(0);

    uint64_t timecode = 0;
    cluster_silent_tracks silent_tracks;
    uint64_t position = 0;
    uint64_t prev_size = 0;
    std::list<binary> simple_blocks;
    std::list<cluster_block> block_group;
};

const element_identify *get_cluster_identifies();

constexpr uint32_t cluster_identifies_size = 6;


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

    USING_VALUE_METHOD(0);

    std::vector<uint64_t> edition_uids;
    uint64_t codec = 0;
    binary track_id;
};

const element_identify *get_track_translate_identifies();

constexpr uint32_t track_translate_identifies_size = 3;


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

    USING_VALUE_METHOD(0);

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
};

const element_identify *get_track_video_color_metadata_identifies();

constexpr uint32_t track_video_color_metadata_identifies_size = 10;


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

    USING_VALUE_METHOD(MATRIX_COEFFICIENTS | BITS_PER_CHANNEL | CHROMA_SITING_HORZ | CHROMA_SITING_VERT | RANGE | TRANSFER_CHARACTERISTICS | PRIMARIES);

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
};

const element_identify *get_track_video_color_identifies();

constexpr uint32_t track_video_color_identifies_size = 14;


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

    USING_VALUE_METHOD(FLAG_INTERLACED | FIELD_ORDER | STEREO_MODE | ALPHA_MODE | PIXEL_CROP_BOTTOM | PIXEL_CROP_TOP | PIXEL_CROP_LEFT | PIXEL_CROP_RIGHT | DISPLAY_UNIT | ASPECT_RATIO_TYPE);

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
};

const element_identify *get_track_video_identifies();

constexpr uint32_t track_video_identifies_size = 16;


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

    USING_VALUE_METHOD(SAMPLING_FREQUENCY | CHANNELS);

    double sampling_frequency = 8000.0;
    double output_sampling_frequency = 8000.0;
    uint64_t channels = 1;
    uint64_t bit_depth = 0;
};

const element_identify *get_track_audio_identifies();

constexpr uint32_t track_audio_identifies_size = 4;


struct track_operation_plane : public value_set_helper<track_operation_plane>
{
    enum
    {
        UID = 1 << 0,
        TYPE = 1 << 1
    };

    track_operation_plane() = default;

    USING_VALUE_METHOD(0);

    uint64_t uid = 0;
    uint64_t type = 0;
};

const element_identify *get_track_operation_plane_identifies();

constexpr uint32_t track_operation_plane_identifies_size = 2;


struct track_operation_combine_planes : public value_set_helper<track_operation_combine_planes>
{
    enum
    {
        TRACK_PLANE = 1 << 0
    };

    track_operation_combine_planes() = default;

    track_operation_combine_planes(track_operation_combine_planes &&) = default;

    track_operation_combine_planes &operator=(track_operation_combine_planes &&)= default;

    USING_VALUE_METHOD(0);

    std::vector<track_operation_plane> track_planes;
};

const element_identify *get_track_operation_combine_planes_identifies();

constexpr uint32_t track_operation_combine_planes_identifies_size = 1;


struct track_operation_join_blocks : public value_set_helper<track_operation_join_blocks>
{
    enum
    {
        UID = 1 << 0
    };

    track_operation_join_blocks() = default;

    track_operation_join_blocks(track_operation_join_blocks &&) = default;

    track_operation_join_blocks &operator=(track_operation_join_blocks &&)= default;

    USING_VALUE_METHOD(0);

    std::vector<uint64_t> uids;
};

const element_identify *get_track_operation_join_blocks_identifies();

constexpr uint32_t track_operation_join_blocks_identifies_size = 1;


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

    USING_VALUE_METHOD(0);

    track_operation_combine_planes combine_planes;
    track_operation_join_blocks join_blocks;
};

const element_identify *get_track_operation_identifies();

constexpr uint32_t track_operation_identifies_size = 2;


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

    USING_VALUE_METHOD(ALGORITHM);

    uint64_t algorithm = 0;
    binary settings;
};

const element_identify *get_track_content_compression_identifies();

constexpr uint32_t track_content_compression_identifies_size = 2;


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

    USING_VALUE_METHOD(ALGORITHM | SIG_ALGORITHM | SIG_HASH_ALGORITHM);

    uint64_t algorithm = 0;
    binary key_id;
    binary signature;
    binary sig_key_id;
    uint64_t sig_algorithm = 0;
    uint64_t sig_hash_algorithm = 0;
};

const element_identify *get_track_content_encryption_identifies();

constexpr uint32_t track_content_encryption_identifies_size = 6;


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

    USING_VALUE_METHOD(ORDER | SCOPE | TYPE);

    uint64_t order = 0;
    uint64_t scope = 1;
    uint64_t type = 0;
    std::unique_ptr<track_content_compression> compression;
    std::unique_ptr<track_content_encryption> encryption;
};

const element_identify *get_track_content_encoding_identifies();

constexpr uint32_t track_content_encoding_identifies_size = 5;


struct track_content_encodings : public value_set_helper<track_content_encodings>
{
    enum
    {
        CONTENT_ENCODING = 1 << 0
    };

    track_content_encodings() = default;

    track_content_encodings(track_content_encodings &&) = default;

    track_content_encodings &operator=(track_content_encodings &&)= default;

    USING_VALUE_METHOD(0);

    std::list<track_content_encoding> content_encodings;
};

const element_identify *get_track_content_encodings_identifies();

constexpr uint32_t track_content_encodings_identifies_size = 1;


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

    USING_VALUE_METHOD(FLAG_ENABLED | FLAG_DEFAULT | FLAG_FORCED | FLAG_LACING | MIN_CACHE | MAX_BLOCK_ADDITION_ID | LANGUAGE | CODEC_DECODE_ALL | CODEC_DELAY | SEEK_PRE_ROLL);

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
};

const element_identify *get_track_entry_identifies();

constexpr uint32_t track_entry_identifies_size = 27;


struct track : public value_set_helper<track>
{
    enum
    {
        TRACK_ENTRY = 1 << 0,
    };

    track() = default;

    track(track &&) = default;

    track &operator=(track &&)= default;

    USING_VALUE_METHOD(0);

    std::list<track_entry> entries;
};

const element_identify *get_track_identifies();

constexpr uint32_t track_identifies_size = 1;


struct cue_track_reference : public value_set_helper<cue_track_reference>
{
    enum
    {
        REF_TIME = 1 << 0
    };

    cue_track_reference() = default;

    USING_VALUE_METHOD(0);

    uint64_t ref_time = 0;
};

const element_identify *get_cue_track_reference_identifies();

constexpr uint32_t cue_track_reference_identifies_size = 1;


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

    USING_VALUE_METHOD(BLOCK_NUMBER | CODEC_STATE);

    uint64_t track = 0;
    uint64_t cluster_position = 0;
    uint64_t relative_position = 0;
    uint64_t duration = 0;
    uint64_t block_number = 1;
    uint64_t codec_state = 0;
    std::list<cue_track_reference> references;
};

const element_identify *get_cue_track_position_identifies();

constexpr uint32_t cue_track_position_identifies_size = 7;


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

    USING_VALUE_METHOD(0);

    uint64_t time = 0;
    std::list<cue_track_position> track_positions;
};

const element_identify *get_cue_point_identifies();

constexpr uint32_t cue_point_identifies_size = 2;


struct cue : public value_set_helper<cue>
{
    enum
    {
        CUE_POINT = 1 << 0
    };

    cue() = default;

    cue(cue &&) = default;

    cue &operator=(cue &&)= default;

    USING_VALUE_METHOD(0);

    std::list<cue_point> cue_points;
};

const element_identify *get_cue_identifies();

constexpr uint32_t cue_identifies_size = 1;


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

    USING_VALUE_METHOD(0);

    std::string description;
    std::string name;
    std::string mine_type;
    binary data;
    uint64_t uid = 0;
};

const element_identify *get_attached_file_identifies();

constexpr uint32_t attached_file_identifies_size = 5;


struct attachment : public value_set_helper<attachment>
{
    enum
    {
        ATTACHED_FILE = 1 << 0
    };

    attachment() = default;

    attachment(attachment &&) = default;

    attachment &operator=(attachment &&)= default;

    USING_VALUE_METHOD(0);

    std::list<attached_file> attached_files;
};

const element_identify *get_attachment_identifies();

constexpr uint32_t attachment_identifies_size = 1;


struct chapter_track : public value_set_helper<chapter_track>
{
    enum
    {
        TRACK_NUMBER = 1 << 0
    };

    chapter_track() = default;

    chapter_track(chapter_track &&) = default;

    chapter_track &operator=(chapter_track &&)= default;

    USING_VALUE_METHOD(0);

    std::vector<uint64_t> track_numbers;
};

const element_identify *get_chapter_track_identifies();

constexpr uint32_t chapter_track_identifies_size = 1;


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

    USING_VALUE_METHOD(0);

    std::string string;
    std::list<std::string> languages;
    std::list<std::string> countries;
};

const element_identify *get_chapter_display_identifies();

constexpr uint32_t chapter_display_identifies_size = 3;


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

    USING_VALUE_METHOD(0);

    uint64_t time = 0;
    binary data;
};

const element_identify *get_chapter_process_command_identifies();

constexpr uint32_t chapter_process_command_identifies_size = 2;


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

    USING_VALUE_METHOD(CODEC_ID);

    uint64_t codec_id = 0;
    binary process_private;
    std::list<chapter_process_command> commands;
};

const element_identify *get_chapter_process_identifies();

constexpr uint32_t chapter_process_identifies_size = 3;


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

    USING_VALUE_METHOD(FLAG_HIDDEN | FLAG_ENABLED);

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
};

const element_identify *get_chapter_atom_identifies();

constexpr uint32_t chapter_atom_identifies_size = 13;


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

    USING_VALUE_METHOD(FLAG_HIDDEN | FLAG_DEFAULT | FLAG_ORDERED);

    uint64_t uid = 0;
    uint64_t flag_hidden = 0;
    uint64_t flag_default = 0;
    uint64_t flag_ordered = 0;
    std::list<chapter_atom> chapter_atoms;
};

const element_identify *get_chapter_edition_entry_identifies();

constexpr uint32_t chapter_edition_entry_identifies_size = 5;


struct chapter : public value_set_helper<chapter>
{
    enum
    {
        EDITION_ENTRY = 1 << 0
    };

    chapter() = default;

    chapter(chapter &&) = default;

    chapter &operator=(chapter &&)= default;

    USING_VALUE_METHOD(0);

    std::list<chapter_edition_entry> edition_entries;
};

const element_identify *get_chapter_identifies();

constexpr uint32_t chapter_identifies_size = 1;


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

    USING_VALUE_METHOD(TYPE_VALUE);

    uint64_t type_value = 50;
    std::string type;
    std::vector<uint64_t> uids;
};

const element_identify *get_tag_target_identifies();

constexpr uint32_t tag_target_identifies_size = 3;


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

    USING_VALUE_METHOD(LANGUAGE | DEFAULT);

    std::list<simple_tag> tags;
    std::string name;
    std::string language;
    uint64_t tag_default = 1;
    std::string string;
    binary bin;
};

const element_identify *get_simple_tag_identifies();

constexpr uint32_t simple_tag_identifies_size = 6;


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

    USING_VALUE_METHOD(0);

    tag_target target;
    std::list<simple_tag> simple_tags;
};

const element_identify *get_tag_identifies();

constexpr uint32_t tag_identifies_size = 2;


struct tags : public value_set_helper<tags>
{
    enum
    {
        TAG = 1 << 0
    };

    tags() = default;

    tags(tags &&) = default;

    tags &operator=(tags &&)= default;

    USING_VALUE_METHOD(0);

    std::list<tag> tag_list;
};

const element_identify *get_tags_identifies();

constexpr uint32_t tags_identifies_size = 1;


#undef USING_VALUE_METHOD

}
}
}


#undef ELEMENT_PROPERTY


#endif //MOCK_PLAYER_ELEMENT_HPP
