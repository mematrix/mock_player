//
// Created by Charles on 2017/11/17.
//

#ifndef MOCK_PLAYER_ELEMENT_HPP
#define MOCK_PLAYER_ELEMENT_HPP


#include <cstdint>
#include <memory>

#include "matroska_common.hpp"
#include "util/pointer_util.hpp"
#include "matroska_track.hpp"

#define ELEMENT_PROPERTY(ma, mu, def, type, ver)


namespace player {
namespace container {
namespace matroska {


enum class element_type
{
    UNSIGNED_INTEGER = 0,
    SIGNED_INTEGER,
    FLOAT,
    STRING,
    BINARY,
    UNSIGNED_INTEGER_LIST,
    SIGNED_INTEGER_LIST,
    STRING_LIST,
    BINARY_LIST,
    MASTER,
    MULTIPLE_MASTER,
    OPTIONAL_MASTER
};

struct element_identify
{
    element_type type;
    int32_t name_mask;
    uint32_t ebml_id;
    util::any_pointer_data class_member_ptr;

    template<typename Tp>
    element_identify(element_type t, Tp ptr, int32_t mask, uint32_t id) noexcept : type(t), name_mask(mask), ebml_id(id)
    {
        class_member_ptr.access<Tp>() = ptr;
    }
};


class master_element_identify
{
    using entry = std::pair<uint32_t, const element_identify *>;
    std::vector<std::pair<uint32_t, const element_identify *>> ebml_map;

public:
    master_element_identify(const element_identify *, uint32_t);

    const element_identify *get(uint32_t) const;

    uint32_t size() const { return static_cast<uint32_t>(ebml_map.size()); }
};

template<typename T>
const master_element_identify &get_identify();


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
    utf8_string doc_type;
    uint64_t doc_type_version = 1;
    uint64_t doc_type_read_version = 1;
};

template<>
const master_element_identify &get_identify<ebml_header>();


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

template<>
const master_element_identify &get_identify<meta_seek>();


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

    multiple_master<meta_seek> seeks;
};

template<>
const master_element_identify &get_identify<meta_seek_head>();


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

    uint64_list edition_uids;
    uint64_t codec = 0;
    binary id;
};

template<>
const master_element_identify &get_identify<segment_chapter_translate>();


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
    utf8_string filename;
    binary prev_uid;
    utf8_string prev_filename;
    binary next_uid;
    utf8_string next_filename;

    binary_list families;
    multiple_master<segment_chapter_translate> chapter_translates;

    uint64_t timecode_scale = 1000000;
    double duration = 0.0;
    int64_t date_utc = 0;
    utf8_string title;
    utf8_string muxing_app;
    utf8_string writing_app;
};

template<>
const master_element_identify &get_identify<segment_info>();


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

    uint64_list numbers;
};

template<>
const master_element_identify &get_identify<cluster_silent_tracks>();


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

template<>
const master_element_identify &get_identify<cluster_block_more>();


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

    multiple_master<cluster_block_more> block_more;
};

template<>
const master_element_identify &get_identify<cluster_block_additions>();


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

template<>
const master_element_identify &get_identify<cluster_time_slice>();


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

    multiple_master<cluster_time_slice> time_slices;
};

template<>
const master_element_identify &get_identify<cluster_slices>();


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
    int64_list reference_block;
    binary codec_state;
    int64_t discard_padding = 0;
    cluster_slices slices;
};

template<>
const master_element_identify &get_identify<cluster_block>();


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
    binary_list simple_blocks;
    multiple_master<cluster_block> block_group;
};

template<>
const master_element_identify &get_identify<cluster>();


template<>
const master_element_identify &get_identify<track_translate>();

template<>
const master_element_identify &get_identify<track_video_color_metadata>();

template<>
const master_element_identify &get_identify<track_video_color>();

template<>
const master_element_identify &get_identify<track_video>();

template<>
const master_element_identify &get_identify<track_audio>();

template<>
const master_element_identify &get_identify<track_operation_plane>();

template<>
const master_element_identify &get_identify<track_operation_combine_planes>();

template<>
const master_element_identify &get_identify<track_operation_join_blocks>();

template<>
const master_element_identify &get_identify<track_operation>();

template<>
const master_element_identify &get_identify<track_content_compression>();

template<>
const master_element_identify &get_identify<track_content_encryption>();

template<>
const master_element_identify &get_identify<track_content_encoding>();

template<>
const master_element_identify &get_identify<track_content_encodings>();

template<>
const master_element_identify &get_identify<track_entry>();

template<>
const master_element_identify &get_identify<track>();


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

template<>
const master_element_identify &get_identify<cue_track_reference>();


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
    multiple_master<cue_track_reference> references;
};

template<>
const master_element_identify &get_identify<cue_track_position>();


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
    multiple_master<cue_track_position> track_positions;
};

template<>
const master_element_identify &get_identify<cue_point>();


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

    multiple_master<cue_point> cue_points;
};

template<>
const master_element_identify &get_identify<cue>();


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

    utf8_string description;
    utf8_string name;
    utf8_string mine_type;
    binary data;
    uint64_t uid = 0;
};

template<>
const master_element_identify &get_identify<attached_file>();


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

    multiple_master<attached_file> attached_files;
};

template<>
const master_element_identify &get_identify<attachment>();


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

    uint64_list track_numbers;
};

template<>
const master_element_identify &get_identify<chapter_track>();


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

    utf8_string string;
    string_list languages;
    string_list countries;
};

template<>
const master_element_identify &get_identify<chapter_display>();


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

template<>
const master_element_identify &get_identify<chapter_process_command>();


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
    multiple_master<chapter_process_command> commands;
};

template<>
const master_element_identify &get_identify<chapter_process>();


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

    multiple_master<chapter_atom> atoms;
    uint64_t uid = 0;
    utf8_string string_uid;
    uint64_t time_start = 0;
    uint64_t time_end = 0;
    uint64_t flag_hidden = 0;
    uint64_t flag_enabled = 1;
    binary segment_uid;
    uint64_t segment_edition_uid = 0;
    uint64_t physical_equiv = 0;
    chapter_track track;
    multiple_master<chapter_display> displays;
    multiple_master<chapter_process> processes;
};

template<>
const master_element_identify &get_identify<chapter_atom>();


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
    multiple_master<chapter_atom> chapter_atoms;
};

template<>
const master_element_identify &get_identify<chapter_edition_entry>();


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

    multiple_master<chapter_edition_entry> edition_entries;
};

template<>
const master_element_identify &get_identify<chapter>();


struct tag_target : public value_set_helper<tag_target>
{
    enum
    {
        TYPE_VALUE = 1 << 0,
        TYPE = 1 << 1,
        TRACK_UID = 1 << 2,
        EDITION_UID = 1 << 3,
        CHAPTER_UID = 1 << 4,
        ATTACHMENT_UID = 1 << 5
    };

    tag_target() = default;

    tag_target(tag_target &&) = default;

    tag_target &operator=(tag_target &&)= default;

    USING_VALUE_METHOD(TYPE_VALUE);

    uint64_t type_value = 50;
    utf8_string type;
    uint64_list uids;
};

template<>
const master_element_identify &get_identify<tag_target>();


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

    multiple_master<simple_tag> tags;
    utf8_string name;
    utf8_string language;
    uint64_t tag_default = 1;
    utf8_string string;
    binary bin;
};

template<>
const master_element_identify &get_identify<simple_tag>();


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
    multiple_master<simple_tag> simple_tags;
};

template<>
const master_element_identify &get_identify<tag>();


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

    multiple_master<tag> tag_list;
};

template<>
const master_element_identify &get_identify<tags>();


const master_element_identify *get_id_map(uint32_t id);


#undef USING_VALUE_METHOD

}
}
}


#undef ELEMENT_PROPERTY


#endif //MOCK_PLAYER_ELEMENT_HPP
