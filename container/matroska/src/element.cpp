//
// Created by Charles on 2017/11/17.
//

#include "element.hpp"

#include <algorithm>


using namespace player::container::matroska;

#define ARRAY_SIZE(ary) (sizeof(ary) / sizeof(ary[0]))

template<>
const master_element_identify &::player::container::matroska::get_identify<ebml_header>()
{
    static element_identify ebml_header_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::version, ebml_header::VERSION, 0x4286),
            element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::read_version, ebml_header::READ_VERSION, 0x42F7),
            element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::max_id_length, ebml_header::MAX_ID_LENGTH, 0x42F2),
            element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::max_size_length, ebml_header::MAX_SIZE_LENGTH, 0x42F3),
            element_identify(element_type::STRING, &ebml_header::doc_type, ebml_header::DOC_TYPE, 0x4282),
            element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::doc_type_version, ebml_header::DOC_TYPE_VERSION, 0x4287),
            element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::doc_type_read_version, ebml_header::DOC_TYPE_READ_VERSION, 0x4285)
    };
    static master_element_identify map(ebml_header_identifies, ARRAY_SIZE(ebml_header_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<meta_seek>()
{
    static element_identify meta_seek_identifies[] = {
            element_identify(element_type::BINARY, &meta_seek::id, meta_seek::ID, 0x53AB),
            element_identify(element_type::UNSIGNED_INTEGER, &meta_seek::position, meta_seek::POSITION, 0x53AC)
    };
    static master_element_identify map(meta_seek_identifies, ARRAY_SIZE(meta_seek_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<meta_seek_head>()
{
    static element_identify meta_seek_head_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &meta_seek_head::seeks, meta_seek_head::SEEK, 0x4DBB)
    };
    static master_element_identify map(meta_seek_head_identifies, ARRAY_SIZE(meta_seek_head_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<segment_chapter_translate>()
{
    static element_identify segment_chapter_translate_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &segment_chapter_translate::edition_uids, segment_chapter_translate::EDITION_UID, 0x69FC),
            element_identify(element_type::UNSIGNED_INTEGER, &segment_chapter_translate::codec, segment_chapter_translate::CODEC, 0x69BF),
            element_identify(element_type::BINARY, &segment_chapter_translate::id, segment_chapter_translate::ID, 0x69A5)
    };
    static master_element_identify map(segment_chapter_translate_identifies, ARRAY_SIZE(segment_chapter_translate_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<segment_info>()
{
    static element_identify segment_info_identifies[] = {
            element_identify(element_type::BINARY, &segment_info::uid, segment_info::UID, 0x73A4),
            element_identify(element_type::STRING, &segment_info::filename, segment_info::FILENAME, 0x7384),
            element_identify(element_type::BINARY, &segment_info::prev_uid, segment_info::PREV_UID, 0x3CB923),
            element_identify(element_type::STRING, &segment_info::prev_filename, segment_info::PREV_FILENAME, 0x3C83AB),
            element_identify(element_type::BINARY, &segment_info::next_uid, segment_info::NEXT_UID, 0x3EB923),
            element_identify(element_type::STRING, &segment_info::next_filename, segment_info::NEXT_FILENAME, 0x3E83BB),

            element_identify(element_type::BINARY_LIST, &segment_info::families, segment_info::FAMILY, 0x4444),
            element_identify(element_type::MULTIPLE_MASTER, &segment_info::chapter_translates, segment_info::CHAPTER_TRANSLATE, 0x6924),

            element_identify(element_type::UNSIGNED_INTEGER, &segment_info::timecode_scale, segment_info::TIMECODE_SCALE, 0x2AD7B1),
            element_identify(element_type::FLOAT, &segment_info::duration, segment_info::DURATION, 0x4489),
            element_identify(element_type::SIGNED_INTEGER, &segment_info::date_utc, segment_info::DATE_UTC, 0x4461),
            element_identify(element_type::STRING, &segment_info::title, segment_info::TITLE, 0x7BA9),
            element_identify(element_type::STRING, &segment_info::muxing_app, segment_info::MUXING_APP, 0x4D80),
            element_identify(element_type::STRING, &segment_info::writing_app, segment_info::WRITING_APP, 0x5741)
    };
    static master_element_identify map(segment_info_identifies, ARRAY_SIZE(segment_info_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cluster_silent_tracks>()
{
    static element_identify cluster_silent_tracks_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &cluster_silent_tracks::numbers, cluster_silent_tracks::NUMBER, 0x58D7)
    };
    static master_element_identify map(cluster_silent_tracks_identifies, ARRAY_SIZE(cluster_silent_tracks_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cluster_block_more>()
{
    static element_identify cluster_block_more_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &cluster_block_more::add_id, cluster_block_more::ADD_ID, 0xEE),
            element_identify(element_type::BINARY, &cluster_block_more::additional, cluster_block_more::ADDITIONAL, 0xA5)
    };
    static master_element_identify map(cluster_block_more_identifies, ARRAY_SIZE(cluster_block_more_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cluster_block_additions>()
{
    static element_identify cluster_block_additions_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &cluster_block_additions::block_more, cluster_block_additions::BLOCK_MORE, 0xA6)
    };
    static master_element_identify map(cluster_block_additions_identifies, ARRAY_SIZE(cluster_block_additions_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cluster_time_slice>()
{
    static element_identify cluster_time_slice_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &cluster_time_slice::lace_number, cluster_time_slice::LACE_NUMBER, 0xCC)
    };
    static master_element_identify map(cluster_time_slice_identifies, ARRAY_SIZE(cluster_time_slice_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cluster_slices>()
{
    static element_identify cluster_slices_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &cluster_slices::time_slices, cluster_slices::TIME_SLICE, 0xE8)
    };
    static master_element_identify map(cluster_slices_identifies, ARRAY_SIZE(cluster_slices_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cluster_block>()
{
    static element_identify cluster_block_identifies[] = {
            element_identify(element_type::BINARY, &cluster_block::block, cluster_block::BLOCK, 0xA1),
            element_identify(element_type::MASTER, &cluster_block::additions, cluster_block::ADDITIONS, 0x75A1),
            element_identify(element_type::UNSIGNED_INTEGER, &cluster_block::duration, cluster_block::DURATION, 0x9B),
            element_identify(element_type::UNSIGNED_INTEGER, &cluster_block::reference_priority, cluster_block::REFERENCE_PRIORITY, 0xFA),
            element_identify(element_type::SIGNED_INTEGER_LIST, &cluster_block::reference_block, cluster_block::REFERENCE_BLOCK, 0xFB),
            element_identify(element_type::BINARY, &cluster_block::codec_state, cluster_block::CODEC_STATE, 0xA4),
            element_identify(element_type::SIGNED_INTEGER, &cluster_block::discard_padding, cluster_block::DISCARD_PADDING, 0x75A2),
            element_identify(element_type::MASTER, &cluster_block::slices, cluster_block::SLICES, 0x8E)
    };
    static master_element_identify map(cluster_block_identifies, ARRAY_SIZE(cluster_block_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cluster>()
{
    static element_identify cluster_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &cluster::timecode, cluster::TIMECODE, 0xE7),
            element_identify(element_type::MASTER, &cluster::silent_tracks, cluster::SILENT_TRACKS, 0x5854),
            element_identify(element_type::UNSIGNED_INTEGER, &cluster::position, cluster::POSITION, 0xA7),
            element_identify(element_type::UNSIGNED_INTEGER, &cluster::prev_size, cluster::PREV_SIZE, 0xAB),
            element_identify(element_type::BINARY_LIST, &cluster::simple_blocks, cluster::SIMPLE_BLOCK, 0xA3),
            element_identify(element_type::MULTIPLE_MASTER, &cluster::block_group, cluster::BLOCK_GROUP, 0xA0)
    };
    static master_element_identify map(cluster_identifies, ARRAY_SIZE(cluster_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_translate>()
{
    static element_identify track_translate_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &track_translate::edition_uids, track_translate::EDITION_UID, 0x66FC),
            element_identify(element_type::UNSIGNED_INTEGER, &track_translate::codec, track_translate::CODEC, 0x66BF),
            element_identify(element_type::BINARY, &track_translate::track_id, track_translate::TRACK_ID, 0x66A5)
    };
    static master_element_identify map(track_translate_identifies, ARRAY_SIZE(track_translate_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_video_color_metadata>()
{
    static element_identify track_video_color_metadata_identifies[] = {
            element_identify(element_type::FLOAT, &track_video_color_metadata::primary_r_chromaticity_x, track_video_color_metadata::PRIMARY_R_CHROMATICITY_X, 0x55D1),
            element_identify(element_type::FLOAT, &track_video_color_metadata::primary_r_chromaticity_y, track_video_color_metadata::PRIMARY_R_CHROMATICITY_Y, 0x55D2),
            element_identify(element_type::FLOAT, &track_video_color_metadata::primary_g_chromaticity_x, track_video_color_metadata::PRIMARY_G_CHROMATICITY_X, 0x55D3),
            element_identify(element_type::FLOAT, &track_video_color_metadata::primary_g_chromaticity_y, track_video_color_metadata::PRIMARY_G_CHROMATICITY_Y, 0x55D4),
            element_identify(element_type::FLOAT, &track_video_color_metadata::primary_b_chromaticity_x, track_video_color_metadata::PRIMARY_B_CHROMATICITY_X, 0x55D5),
            element_identify(element_type::FLOAT, &track_video_color_metadata::primary_b_chromaticity_y, track_video_color_metadata::PRIMARY_B_CHROMATICITY_Y, 0x55D6),
            element_identify(element_type::FLOAT, &track_video_color_metadata::white_point_chromaticity_x, track_video_color_metadata::WHITE_POINT_CHROMATICITY_X, 0x55D7),
            element_identify(element_type::FLOAT, &track_video_color_metadata::white_point_chromaticity_y, track_video_color_metadata::WHITE_POINT_CHROMATICITY_Y, 0x55D8),
            element_identify(element_type::FLOAT, &track_video_color_metadata::luminance_max, track_video_color_metadata::LUMINANCE_MAX, 0x55D9),
            element_identify(element_type::FLOAT, &track_video_color_metadata::luminance_min, track_video_color_metadata::LUMINANCE_MIN, 0x55DA)
    };
    static master_element_identify map(track_video_color_metadata_identifies, ARRAY_SIZE(track_video_color_metadata_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_video_color>()
{
    static element_identify track_video_color_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::matrix_coefficients, track_video_color::MATRIX_COEFFICIENTS, 0x55B1),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::bits_per_channel, track_video_color::BITS_PER_CHANNEL, 0x55B2),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::chroma_subsampling_horz, track_video_color::CHROMA_SUBSAMPLING_HORZ, 0x55B3),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::chroma_subsampling_vert, track_video_color::CHROMA_SUBSAMPLING_VERT, 0x55B4),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::cb_subsampling_horz, track_video_color::CB_SUBSAMPLING_HORZ, 0x55B5),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::cb_subsampling_vert, track_video_color::CB_SUBSAMPLING_VERT, 0x55B6),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::chroma_siting_horz, track_video_color::CHROMA_SITING_HORZ, 0x55B7),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::chroma_siting_vert, track_video_color::CHROMA_SITING_VERT, 0x55B8),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::range, track_video_color::RANGE, 0x55B9),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::transfer_characteristics, track_video_color::TRANSFER_CHARACTERISTICS, 0x55BA),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::primaries, track_video_color::PRIMARIES, 0x55BB),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::max_cll, track_video_color::MAX_CLL, 0x55BC),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video_color::max_fall, track_video_color::MAX_FALL, 0x55BD),
            element_identify(element_type::OPTIONAL_MASTER, &track_video_color::mastering_metadata, track_video_color::MASTERING_METADATA, 0x55D0)
    };
    static master_element_identify map(track_video_color_identifies, ARRAY_SIZE(track_video_color_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_video>()
{
    static element_identify track_video_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::flag_interlaced, track_video::FLAG_INTERLACED, 0x9A),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::field_order, track_video::FIELD_ORDER, 0x9D),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::stereo_mode, track_video::STEREO_MODE, 0x53B8),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::alpha_mode, track_video::ALPHA_MODE, 0x53C0),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::pixel_width, track_video::PIXEL_WIDTH, 0xB0),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::pixel_height, track_video::PIXEL_HEIGHT, 0xBA),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::pixel_crop_bottom, track_video::PIXEL_CROP_BOTTOM, 0x54AA),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::pixel_crop_top, track_video::PIXEL_CROP_TOP, 0x54BB),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::pixel_crop_left, track_video::PIXEL_CROP_LEFT, 0x54CC),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::pixel_crop_right, track_video::PIXEL_CROP_RIGHT, 0x54DD),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::display_width, track_video::DISPLAY_WIDTH, 0x54B0),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::display_height, track_video::DISPLAY_HEIGHT, 0x54BA),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::display_unit, track_video::DISPLAY_UNIT, 0x54B2),
            element_identify(element_type::UNSIGNED_INTEGER, &track_video::aspect_ratio_type, track_video::ASPECT_RATIO_TYPE, 0x54B3),
            element_identify(element_type::BINARY, &track_video::color_space, track_video::COLOR_SPACE, 0x2EB524),
            element_identify(element_type::OPTIONAL_MASTER, &track_video::color, track_video::COLOR, 0x55B0)
    };
    static master_element_identify map(track_video_identifies, ARRAY_SIZE(track_video_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_audio>()
{
    static element_identify track_audio_identifies[] = {
            element_identify(element_type::FLOAT, &track_audio::sampling_frequency, track_audio::SAMPLING_FREQUENCY, 0xB5),
            element_identify(element_type::FLOAT, &track_audio::output_sampling_frequency, track_audio::OUTPUT_SAMPLING_FREQUENCY, 0x78B5),
            element_identify(element_type::UNSIGNED_INTEGER, &track_audio::channels, track_audio::CHANNELS, 0x9F),
            element_identify(element_type::UNSIGNED_INTEGER, &track_audio::bit_depth, track_audio::BIT_DEPTH, 0x6264)
    };
    static master_element_identify map(track_audio_identifies, ARRAY_SIZE(track_audio_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_operation_plane>()
{
    static element_identify track_operation_plane_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &track_operation_plane::uid, track_operation_plane::UID, 0xE5),
            element_identify(element_type::UNSIGNED_INTEGER, &track_operation_plane::type, track_operation_plane::TYPE, 0xE6)
    };
    static master_element_identify map(track_operation_plane_identifies, ARRAY_SIZE(track_operation_plane_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_operation_combine_planes>()
{
    static element_identify track_operation_combine_planes_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &track_operation_combine_planes::track_planes, track_operation_combine_planes::TRACK_PLANE, 0xE4)
    };
    static master_element_identify map(track_operation_combine_planes_identifies, ARRAY_SIZE(track_operation_combine_planes_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_operation_join_blocks>()
{
    static element_identify track_operation_join_blocks_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &track_operation_join_blocks::uids, track_operation_join_blocks::UID, 0xED)
    };
    static master_element_identify map(track_operation_join_blocks_identifies, ARRAY_SIZE(track_operation_join_blocks_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_operation>()
{
    static element_identify track_operation_identifies[] = {
            element_identify(element_type::MASTER, &track_operation::combine_planes, track_operation::TRACK_COMBINE_PLANES, 0xE3),
            element_identify(element_type::MASTER, &track_operation::join_blocks, track_operation::TRACK_JOIN_BLOCKS, 0xE9)
    };
    static master_element_identify map(track_operation_identifies, ARRAY_SIZE(track_operation_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_content_compression>()
{
    static element_identify track_content_compression_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &track_content_compression::algorithm, track_content_compression::ALGORITHM, 0x4254),
            element_identify(element_type::BINARY, &track_content_compression::settings, track_content_compression::SETTINGS, 0x4255)
    };
    static master_element_identify map(track_content_compression_identifies, ARRAY_SIZE(track_content_compression_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_content_encryption>()
{
    static element_identify track_content_encryption_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &track_content_encryption::algorithm, track_content_encryption::ALGORITHM, 0x47E1),
            element_identify(element_type::BINARY, &track_content_encryption::key_id, track_content_encryption::KEY_ID, 0x47E2),
            element_identify(element_type::BINARY, &track_content_encryption::signature, track_content_encryption::SIGNATURE, 0x47E3),
            element_identify(element_type::BINARY, &track_content_encryption::sig_key_id, track_content_encryption::SIG_KEY_ID, 0x47E4),
            element_identify(element_type::UNSIGNED_INTEGER, &track_content_encryption::sig_algorithm, track_content_encryption::SIG_ALGORITHM, 0x47E5),
            element_identify(element_type::UNSIGNED_INTEGER, &track_content_encryption::sig_hash_algorithm, track_content_encryption::SIG_HASH_ALGORITHM, 0x47E6)
    };
    static master_element_identify map(track_content_encryption_identifies, ARRAY_SIZE(track_content_encryption_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_content_encoding>()
{
    static element_identify track_content_encoding_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &track_content_encoding::order, track_content_encoding::ORDER, 0x5031),
            element_identify(element_type::UNSIGNED_INTEGER, &track_content_encoding::scope, track_content_encoding::SCOPE, 0x5032),
            element_identify(element_type::UNSIGNED_INTEGER, &track_content_encoding::type, track_content_encoding::TYPE, 0x5033),
            element_identify(element_type::OPTIONAL_MASTER, &track_content_encoding::compression, track_content_encoding::COMPRESSION, 0x5034),
            element_identify(element_type::OPTIONAL_MASTER, &track_content_encoding::encryption, track_content_encoding::ENCRYPTION, 0x5035)
    };
    static master_element_identify map(track_content_encoding_identifies, ARRAY_SIZE(track_content_encoding_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_content_encodings>()
{
    static element_identify track_content_encodings_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &track_content_encodings::content_encodings, track_content_encodings::CONTENT_ENCODING, 0x6240)
    };
    static master_element_identify map(track_content_encodings_identifies, ARRAY_SIZE(track_content_encodings_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track_entry>()
{
    static element_identify track_entry_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::number, track_entry::NUMBER, 0xD7),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::uid, track_entry::UID, 0x73C5),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::type, track_entry::TYPE, 0x83),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::flag_enabled, track_entry::FLAG_ENABLED, 0xB9),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::flag_default, track_entry::FLAG_DEFAULT, 0x88),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::flag_forced, track_entry::FLAG_FORCED, 0x55AA),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::flag_lacing, track_entry::FLAG_LACING, 0x9C),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::min_cache, track_entry::MIN_CACHE, 0x6DE7),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::max_cache, track_entry::MAX_CACHE, 0x6DF8),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::default_duration, track_entry::DEFAULT_DURATION, 0x23E383),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::default_decoded_field_duration, track_entry::DEFAULT_DECODED_FIELD_DURATION, 0x234E7A),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::max_block_addition_id, track_entry::MAX_BLOCK_ADDITION_ID, 0x55EE),
            element_identify(element_type::STRING, &track_entry::name, track_entry::NAME, 0x536E),
            element_identify(element_type::STRING, &track_entry::language, track_entry::LANGUAGE, 0x22B59C),
            element_identify(element_type::STRING, &track_entry::codec_id, track_entry::CODEC_ID, 0x86),
            element_identify(element_type::BINARY, &track_entry::codec_private, track_entry::CODEC_PRIVATE, 0x63A2),
            element_identify(element_type::STRING, &track_entry::codec_name, track_entry::CODEC_NAME, 0x258688),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::attachment_link, track_entry::ATTACHMENT_LINK, 0x7446),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::codec_decode_all, track_entry::CODEC_DECODE_ALL, 0xAA),
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &track_entry::overlays, track_entry::OVERLAY, 0x6FAB),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::codec_delay, track_entry::CODEC_DELAY, 0x56AA),
            element_identify(element_type::UNSIGNED_INTEGER, &track_entry::seek_pre_roll, track_entry::SEEK_PRE_ROLL, 0x56BB),
            element_identify(element_type::MULTIPLE_MASTER, &track_entry::translate, track_entry::TRANSLATE, 0x6624),
            element_identify(element_type::OPTIONAL_MASTER, &track_entry::video, track_entry::VIDEO, 0xE0),
            element_identify(element_type::OPTIONAL_MASTER, &track_entry::audio, track_entry::AUDIO, 0xE1),
            element_identify(element_type::OPTIONAL_MASTER, &track_entry::operation, track_entry::OPERATION, 0xE2),
            element_identify(element_type::OPTIONAL_MASTER, &track_entry::content_encodings, track_entry::CONTENT_ENCODINGS, 0x6D80)
    };
    static master_element_identify map(track_entry_identifies, ARRAY_SIZE(track_entry_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<track>()
{
    static element_identify track_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &track::entries, track::TRACK_ENTRY, 0xAE)
    };
    static master_element_identify map(track_identifies, ARRAY_SIZE(track_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cue_track_reference>()
{
    static element_identify cue_track_reference_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &cue_track_reference::ref_time, cue_track_reference::REF_TIME, 0x96)
    };
    static master_element_identify map(cue_track_reference_identifies, ARRAY_SIZE(cue_track_reference_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cue_track_position>()
{
    static element_identify cue_track_position_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &cue_track_position::track, cue_track_position::TRACK, 0xF7),
            element_identify(element_type::UNSIGNED_INTEGER, &cue_track_position::cluster_position, cue_track_position::CLUSTER_POSITION, 0xF1),
            element_identify(element_type::UNSIGNED_INTEGER, &cue_track_position::relative_position, cue_track_position::RELATIVE_POSITION, 0xF0),
            element_identify(element_type::UNSIGNED_INTEGER, &cue_track_position::duration, cue_track_position::DURATION, 0xB2),
            element_identify(element_type::UNSIGNED_INTEGER, &cue_track_position::block_number, cue_track_position::BLOCK_NUMBER, 0x5378),
            element_identify(element_type::UNSIGNED_INTEGER, &cue_track_position::codec_state, cue_track_position::CODEC_STATE, 0xEA),
            element_identify(element_type::MULTIPLE_MASTER, &cue_track_position::references, cue_track_position::REFERENCE, 0xDB)
    };
    static master_element_identify map(cue_track_position_identifies, ARRAY_SIZE(cue_track_position_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cue_point>()
{
    static element_identify cue_point_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &cue_point::time, cue_point::TIME, 0xB3),
            element_identify(element_type::MULTIPLE_MASTER, &cue_point::track_positions, cue_point::TRACK_POSITIONS, 0xB7)
    };
    static master_element_identify map(cue_point_identifies, ARRAY_SIZE(cue_point_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<cue>()
{
    static element_identify cue_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &cue::cue_points, cue::CUE_POINT, 0xBB)
    };
    static master_element_identify map(cue_identifies, ARRAY_SIZE(cue_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<attached_file>()
{
    static element_identify attached_file_identifies[] = {
            element_identify(element_type::STRING, &attached_file::description, attached_file::DESCRIPTION, 0x467E),
            element_identify(element_type::STRING, &attached_file::name, attached_file::NAME, 0x466E),
            element_identify(element_type::STRING, &attached_file::mine_type, attached_file::MINE_TYPE, 0x4660),
            element_identify(element_type::BINARY, &attached_file::data, attached_file::DATA, 0x465C),
            element_identify(element_type::UNSIGNED_INTEGER, &attached_file::uid, attached_file::UID, 0x46AE),
    };
    static master_element_identify map(attached_file_identifies, ARRAY_SIZE(attached_file_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<attachment>()
{
    static element_identify attachment_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &attachment::attached_files, attachment::ATTACHED_FILE, 0x61A7)
    };
    static master_element_identify map(attachment_identifies, ARRAY_SIZE(attachment_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<chapter_track>()
{
    static element_identify chapter_track_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &chapter_track::track_numbers, chapter_track::TRACK_NUMBER, 0x89)
    };
    static master_element_identify map(chapter_track_identifies, ARRAY_SIZE(chapter_track_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<chapter_display>()
{
    static element_identify chapter_display_identifies[] = {
            element_identify(element_type::STRING, &chapter_display::string, chapter_display::STRING, 0x85),
            element_identify(element_type::STRING_LIST, &chapter_display::languages, chapter_display::LANGUAGE, 0x437C),
            element_identify(element_type::STRING_LIST, &chapter_display::countries, chapter_display::COUNTRY, 0x437E)
    };
    static master_element_identify map(chapter_display_identifies, ARRAY_SIZE(chapter_display_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<chapter_process_command>()
{
    static element_identify chapter_process_command_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_process_command::time, chapter_process_command::TIME, 0x6922),
            element_identify(element_type::BINARY, &chapter_process_command::data, chapter_process_command::DATA, 0x6933)
    };
    static master_element_identify map(chapter_process_command_identifies, ARRAY_SIZE(chapter_process_command_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<chapter_process>()
{
    static element_identify chapter_process_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_process::codec_id, chapter_process::CODEC_ID, 0x6955),
            element_identify(element_type::BINARY, &chapter_process::process_private, chapter_process::PRIVATE, 0x450D),
            element_identify(element_type::MULTIPLE_MASTER, &chapter_process::commands, chapter_process::COMMAND, 0x6911)
    };
    static master_element_identify map(chapter_process_identifies, ARRAY_SIZE(chapter_process_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<chapter_atom>()
{
    static element_identify chapter_atom_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &chapter_atom::atoms, chapter_atom::CHAPTER_ATOM, 0xB6),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_atom::uid, chapter_atom::UID, 0x73C4),
            element_identify(element_type::STRING, &chapter_atom::string_uid, chapter_atom::STRING_UID, 0x5654),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_atom::time_start, chapter_atom::TIME_START, 0x91),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_atom::time_end, chapter_atom::TIME_END, 0x92),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_atom::flag_hidden, chapter_atom::FLAG_HIDDEN, 0x98),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_atom::flag_enabled, chapter_atom::FLAG_ENABLED, 0x4598),
            element_identify(element_type::BINARY, &chapter_atom::segment_uid, chapter_atom::SEGMENT_UID, 0x6E67),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_atom::segment_edition_uid, chapter_atom::SEGMENT_EDITION_UID, 0x6EBC),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_atom::physical_equiv, chapter_atom::PHYSICAL_EQUIV, 0x63C3),
            element_identify(element_type::MASTER, &chapter_atom::track, chapter_atom::TRACK, 0x8F),
            element_identify(element_type::MULTIPLE_MASTER, &chapter_atom::displays, chapter_atom::DISPLAY, 0x80),
            element_identify(element_type::MULTIPLE_MASTER, &chapter_atom::processes, chapter_atom::PROCESS, 0x6944)
    };
    static master_element_identify map(chapter_atom_identifies, ARRAY_SIZE(chapter_atom_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<chapter_edition_entry>()
{
    static element_identify chapter_edition_entry_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_edition_entry::uid, chapter_edition_entry::UID, 0x45BC),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_edition_entry::flag_hidden, chapter_edition_entry::FLAG_HIDDEN, 0x45BD),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_edition_entry::flag_default, chapter_edition_entry::FLAG_DEFAULT, 0x45DB),
            element_identify(element_type::UNSIGNED_INTEGER, &chapter_edition_entry::flag_ordered, chapter_edition_entry::FLAG_ORDERED, 0x45DD),
            element_identify(element_type::MULTIPLE_MASTER, &chapter_edition_entry::chapter_atoms, chapter_edition_entry::CHAPTER_ATOM, 0xB6),
    };
    static master_element_identify map(chapter_edition_entry_identifies, ARRAY_SIZE(chapter_edition_entry_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<chapter>()
{
    static element_identify chapter_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &chapter::edition_entries, chapter::EDITION_ENTRY, 0x45B9)
    };
    static master_element_identify map(chapter_identifies, ARRAY_SIZE(chapter_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<tag_target>()
{
    static element_identify tag_target_identifies[] = {
            element_identify(element_type::UNSIGNED_INTEGER, &tag_target::type_value, tag_target::TYPE_VALUE, 0x68CA),
            element_identify(element_type::STRING, &tag_target::type, tag_target::TYPE, 0x63CA),
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &tag_target::uids, tag_target::TRACK_UID, 0x63C5),
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &tag_target::uids, tag_target::EDITION_UID, 0x63C9),
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &tag_target::uids, tag_target::CHAPTER_UID, 0x63C4),
            element_identify(element_type::UNSIGNED_INTEGER_LIST, &tag_target::uids, tag_target::ATTACHMENT_UID, 0x63C6)
    };
    static master_element_identify map(tag_target_identifies, ARRAY_SIZE(tag_target_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<simple_tag>()
{
    static element_identify simple_tag_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &simple_tag::tags, simple_tag::SIMPLE_TAG, 0x67C8),
            element_identify(element_type::STRING, &simple_tag::name, simple_tag::NAME, 0x45A3),
            element_identify(element_type::STRING, &simple_tag::language, simple_tag::LANGUAGE, 0x447A),
            element_identify(element_type::UNSIGNED_INTEGER, &simple_tag::tag_default, simple_tag::DEFAULT, 0x4484),
            element_identify(element_type::STRING, &simple_tag::string, simple_tag::STRING, 0x4487),
            element_identify(element_type::BINARY, &simple_tag::bin, simple_tag::BINARY, 0x4485),
    };
    static master_element_identify map(simple_tag_identifies, ARRAY_SIZE(simple_tag_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<tag>()
{
    static element_identify tag_identifies[] = {
            element_identify(element_type::MASTER, &tag::target, tag::TARGETS, 0x63C0),
            element_identify(element_type::MULTIPLE_MASTER, &tag::simple_tags, tag::SIMPLE_TAG, 0x67C8)
    };
    static master_element_identify map(tag_identifies, ARRAY_SIZE(tag_identifies));

    return map;
}

template<>
const master_element_identify &::player::container::matroska::get_identify<tags>()
{
    static element_identify tags_identifies[] = {
            element_identify(element_type::MULTIPLE_MASTER, &tags::tag_list, tags::TAG, 0x7373)
    };
    static master_element_identify map(tags_identifies, ARRAY_SIZE(tags_identifies));

    return map;
}

/*const master_element_identify *::player::container::matroska::get_id_map(uint32_t id)
{
    switch (id) {
        case 0x1A45DFA3:
            return &get_identify<ebml_header>();
        case 0x4DBB:
            return &get_identify<meta_seek>();
        case 0x114D9B74:
            return &get_identify<meta_seek_head>();
        case 0x6924:
            return &get_identify<segment_chapter_translate>();
        case 0x1549A966:
            return &get_identify<segment_info>();
        case 0x5854:
            return &get_identify<cluster_silent_tracks>();
        case 0xA0:
            return &get_identify<cluster_block>();
        case 0x75A1:
            return &get_identify<cluster_block_additions>();
        case 0xA6:
            return &get_identify<cluster_block_more>();
        case 0xE8:
            return &get_identify<cluster_time_slice>();
        case 0x8E:
            return &get_identify<cluster_slices>();
        case 0x1F43B675:
            return &get_identify<cluster>();
        case 0x6624:
            return &get_identify<track_translate>();
        case 0x55D0:
            return &get_identify<track_video_color_metadata>();
        case 0x55B0:
            return &get_identify<track_video_color>();
        case 0xE0:
            return &get_identify<track_video>();
        case 0xE1:
            return &get_identify<track_audio>();
        case 0xE4:
            return &get_identify<track_operation_plane>();
        case 0xE3:
            return &get_identify<track_operation_combine_planes>();
        case 0xE9:
            return &get_identify<track_operation_join_blocks>();
        case 0xE2:
            return &get_identify<track_operation>();
        case 0x5034:
            return &get_identify<track_content_compression>();
        case 0x5035:
            return &get_identify<track_content_encryption>();
        case 0x6240:
            return &get_identify<track_content_encoding>();
        case 0x6D80:
            return &get_identify<track_content_encodings>();
    }
    return nullptr;
}*/

master_element_identify::master_element_identify(const element_identify *elements, uint32_t size) : ebml_map()
{
    ebml_map.reserve(size);
    for (int i = 0; i < size; ++i) {
        ebml_map.emplace_back(elements[i].ebml_id, elements + i);
    }

    std::sort(ebml_map.begin(), ebml_map.end(), [](const entry &left, const entry &right) {
        return left.first < right.first;
    });
}

const element_identify *master_element_identify::get(uint32_t id) const
{
    auto it = std::lower_bound(ebml_map.begin(), ebml_map.end(), id, [](const entry &map_value, uint32_t val) {
        return map_value.first < val;
    });

    if (it == ebml_map.end() || (*it).first != id) {
        return nullptr;
    }
    return (*it).second;
}
