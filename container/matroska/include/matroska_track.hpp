//
// Created by Charles on 2017/12/8.
//

#ifndef MOCK_PLAYER_MATROSKA_TRACK_HPP
#define MOCK_PLAYER_MATROSKA_TRACK_HPP


#include "matroska_common.hpp"


namespace player {
namespace container {
namespace matroska {


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

    uint64_list edition_uids;
    uint64_t codec = 0;
    binary track_id;
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
    optional_master<track_video_color_metadata> mastering_metadata;
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
    optional_master<track_video_color> color;
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

    USING_VALUE_METHOD(SAMPLING_FREQUENCY | CHANNELS);

    double sampling_frequency = 8000.0;
    double output_sampling_frequency = 8000.0;
    uint64_t channels = 1;
    uint64_t bit_depth = 0;
};


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

    multiple_master<track_operation_plane> track_planes;
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

    USING_VALUE_METHOD(0);

    uint64_list uids;
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

    USING_VALUE_METHOD(0);

    track_operation_combine_planes combine_planes;
    track_operation_join_blocks join_blocks;
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

    USING_VALUE_METHOD(ALGORITHM);

    uint64_t algorithm = 0;
    binary settings;
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

    USING_VALUE_METHOD(ALGORITHM | SIG_ALGORITHM | SIG_HASH_ALGORITHM);

    uint64_t algorithm = 0;
    binary key_id;
    binary signature;
    binary sig_key_id;
    uint64_t sig_algorithm = 0;
    uint64_t sig_hash_algorithm = 0;
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

    USING_VALUE_METHOD(ORDER | SCOPE | TYPE);

    uint64_t order = 0;
    uint64_t scope = 1;
    uint64_t type = 0;
    optional_master<track_content_compression> compression;
    optional_master<track_content_encryption> encryption;
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

    USING_VALUE_METHOD(0);

    multiple_master<track_content_encoding> content_encodings;
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
    utf8_string name;
    utf8_string language;
    utf8_string codec_id;
    binary codec_private;
    utf8_string codec_name;
    uint64_t attachment_link = 0;
    uint64_t codec_decode_all = 1;
    uint64_list overlays;
    uint64_t codec_delay = 0;
    uint64_t seek_pre_roll = 0;
    multiple_master<track_translate> translate;
    optional_master<track_video> video;
    optional_master<track_audio> audio;
    optional_master<track_operation> operation;
    optional_master<track_content_encodings> content_encodings;
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

    USING_VALUE_METHOD(0);

    multiple_master<track_entry> entries;
};


}
}
}


#endif //MOCK_PLAYER_MATROSKA_TRACK_HPP
