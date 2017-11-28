//
// Created by Charles on 2017/11/20.
//

#include "parser.hpp"

#include <istream>

using namespace player::container::matroska;


class ebml_parser_walker
{
public:
    ebml_parser_walker(ebml_parser &p, int64_t pos) : parser(p), position(pos) { }

    ~ebml_parser_walker()
    {
        parser.set_stream_pos(position);
    }

private:
    ebml_parser &parser;
    int64_t position;
};


template<typename T>
void read_master(T &result, ebml_parser &parser, const ebml_node &master_node);

template<typename T>
void parse_sub_master(T &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) { }

template<>
void parse_sub_master<meta_seek_head>(meta_seek_head &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x4DBB) {
        result.seeks.emplace_back();
        read_master(result.seeks.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<segment_info>(segment_info &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node){
    if (node.id == 0x6924) {
        result.chapter_translates.emplace_back();
        read_master(result.chapter_translates.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<cluster_block_additions>(cluster_block_additions &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0xA6) {
        result.block_more.emplace_back();
        read_master(result.block_more.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<cluster_slices>(cluster_slices &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0xE8) {
        result.time_slices.emplace_back();
        read_master(result.time_slices.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<cluster_block>(cluster_block &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x75A1) {
        read_master(result.additions, parser, node);
        result.mask |= identify.name_mask;
    } else if(node.id == 0x8E) {
        read_master(result.slices, parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<cluster>(cluster &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x5854) {
        read_master(result.silent_tracks, parser, node);
        result.mask |= identify.name_mask;
    } else if (node.id == 0xA0) {
        result.block_group.emplace_back();
        read_master(result.block_group.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<track_video_color>(track_video_color &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x55D0) {
        result.mastering_metadata.reset(new track_video_color_metadata());
        read_master(*result.mastering_metadata, parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<track_video>(track_video &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x55B0) {
        result.color.reset(new track_video_color());
        read_master(*result.color, parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<track_operation_combine_planes>(track_operation_combine_planes &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0xE4) {
        result.track_planes.emplace_back();
        read_master(result.track_planes.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<track_operation>(track_operation &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0xE3) {
        read_master(result.combine_planes, parser, node);
        result.mask |= identify.name_mask;
    } else if(node.id == 0xE9) {
        read_master(result.join_blocks, parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<track_content_encoding>(track_content_encoding &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x5034) {
        result.compression.reset(new track_content_compression());
        read_master(*result.compression, parser, node);
        result.mask |= identify.name_mask;
    } else if (node.id == 0x5035) {
        result.encryption.reset(new track_content_encryption());
        read_master(*result.encryption, parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<track_content_encodings>(track_content_encodings &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x6240) {
        result.content_encodings.emplace_back();
        read_master(result.content_encodings.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<track_entry>(track_entry &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x6624) {
        result.translate.emplace_back();
        read_master(result.translate.back(), parser, node);
    } else if(node.id == 0xE0) {
        result.video.reset(new track_video());
        read_master(*result.video, parser, node);
    } else if (node.id == 0xE1) {
        result.audio.reset(new track_audio());
        read_master(*result.audio, parser, node);
    } else if (node.id == 0xE2) {
        result.operation.reset(new track_operation());
        read_master(*result.operation, parser, node);
    } else if (node.id == 0x6D80){
        result.content_encodings.reset(new track_content_encodings());
        read_master(*result.content_encodings, parser, node);
    } else {
        return;
    }

    result.mask |= identify.name_mask;
}

template<>
void parse_sub_master<track>(track &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node){
    if (node.id == 0xAE){
        result.entries.emplace_back();
        read_master(result.entries.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<cue_track_position>(cue_track_position &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0xDB) {
        result.references.emplace_back();
        read_master(result.references.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<cue_point>(cue_point &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0xB7) {
        result.track_positions.emplace_back();
        read_master(result.track_positions.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<cue>(cue &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node){
    if (node.id == 0xBB) {
        result.cue_points.emplace_back();
        read_master(result.cue_points.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<attachment>(attachment &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x61A7) {
        result.attached_files.emplace_back();
        read_master(result.attached_files.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<chapter_process>(chapter_process &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x6911) {
        result.commands.emplace_back();
        read_master(result.commands.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<chapter_atom>(chapter_atom &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0xB6) {
        result.atoms.emplace_back();
        read_master(result.atoms.back(), parser, node);
    } else if (node.id == 0x8F) {
        read_master(result.track, parser, node);
    } else if (node.id == 0x80) {
        result.displays.emplace_back();
        read_master(result.displays.back(), parser, node);
    } else if (node.id == 0x6944) {
        result.processes.emplace_back();
        read_master(result.processes.back(), parser, node);
    } else {
        return;
    }

    result.mask |= identify.name_mask;
}

template<>
void parse_sub_master<chapter_edition_entry>(chapter_edition_entry &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0xB6) {
        result.chapter_atoms.emplace_back();
        read_master(result.chapter_atoms.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<chapter>(chapter &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x45B9) {
        result.edition_entries.emplace_back();
        read_master(result.edition_entries.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<simple_tag>(simple_tag &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x67C8) {
        result.tags.emplace_back();
        read_master(result.tags.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<tag>(tag &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x63C0) {
        read_master(result.target, parser, node);
        result.mask |= identify.name_mask;
    } else if(node.id == 0x67C8) {
        result.simple_tags.emplace_back();
        read_master(result.simple_tags.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<>
void parse_sub_master<tags>(tags &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node) {
    if (node.id == 0x7373) {
        result.tag_list.emplace_back();
        read_master(result.tag_list.back(), parser, node);
        result.mask |= identify.name_mask;
    }
}

template<typename T>
static void read_value_to_element(T &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node)
{
    typedef uint64_t T::* UnsignedMemPtr;
    typedef int64_t T::* SignedMemPtr;
    typedef double T::* FloatMemPtr;
    typedef utf8_string T::* StringMemPtr;
    typedef binary T::* BinaryMemPtr;
    typedef uint64_list T::* UIntListMemPtr;
    typedef int64_list T::* IntListMemPtr;
    typedef string_list T::* StringListMemPtr;
    typedef binary_list T::* BinaryListMemPtr;

    switch (identify.type) {
        case element_type::UNSIGNED_INTEGER: {
            uint64_t unsigned_value;
            if (parser.read_unsigned_integer(node.size, unsigned_value) == 0) {
                result.*(identify.class_member_ptr.access<UnsignedMemPtr>()) = unsigned_value;
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::SIGNED_INTEGER: {
            int64_t int_value;
            if (parser.read_integer(node.size, int_value) == 0) {
                result.*(identify.class_member_ptr.access<SignedMemPtr>()) = int_value;
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::FLOAT: {
            double float_value;
            if (parser.read_float(node.size, float_value) == 0) {
                result.*(identify.class_member_ptr.access<FloatMemPtr>()) = float_value;
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::STRING: {
            std::string str_value;
            if (parser.read_string(node.size, str_value) == 0) {
                result.*(identify.class_member_ptr.access<StringMemPtr>()) = std::move(str_value);
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::BINARY: {
            binary bin_value(node.position, node.size);
            result.*(identify.class_member_ptr.access<BinaryMemPtr>()) = bin_value;
            result.mask |= identify.name_mask;
            break;
        }
        case element_type::UNSIGNED_INTEGER_LIST: {
            uint64_t unsigned_value;
            if (parser.read_unsigned_integer(node.size, unsigned_value) == 0) {
                auto &list_value = result.*(identify.class_member_ptr.access<UIntListMemPtr>());
                list_value.push_back(unsigned_value);
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::SIGNED_INTEGER_LIST: {
            int64_t signed_value;
            if (parser.read_integer(node.size, signed_value) == 0) {
                auto &list_value = result.*(identify.class_member_ptr.access<IntListMemPtr>());
                list_value.emplace_back(signed_value);
                result.mask |= identify.name_mask;
            }
        }
        case element_type::STRING_LIST:
        {
            std::string str_value;
            if (parser.read_string(node.size, str_value) == 0) {
                auto &list_value = result.*(identify.class_member_ptr.access<StringListMemPtr>());
                list_value.emplace_back(std::move(str_value));
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::BINARY_LIST:
        {
            binary bin_value(node.position, node.size);
            (result.*(identify.class_member_ptr.access<BinaryListMemPtr>())).emplace_back(std::move(bin_value));
            result.mask |= identify.name_mask;
            break;
        }
        case element_type::MASTER:
        case element_type::MULTIPLE_MASTER:
        case element_type::OPTIONAL_MASTER: {
            parse_sub_master<T>(result, identify, parser, node);
            break;
        }
    }
}

template<typename T>
void read_master(T &result, ebml_parser &parser, const ebml_node &master_node)
{
    ebml_parser_walker walker(parser, master_node.position + master_node.size);
    auto size = master_node.size;
    auto &id_map = get_identify<T>();

    while (size > 0) {
        ebml_node node;
        if (parser.parse_next(node) != 0) {
            return;
        }

        size -= node.size;
        auto identify = id_map.get(node.id);
        if (nullptr != identify) {
            read_value_to_element<T>(result, *identify, parser, node);
        }

        parser.set_stream_pos(node.position + node.size);
    }
}


parser::parser(std::unique_ptr<std::istream> &&stream, parser_callback &cb) : in_stream(std::move(stream)), callback(cb), ep(*in_stream)
{
}

int32_t parser::parse_ebml_header(ebml_header &result)
{
    ebml_node root_node;
    if (ep.parse_next(root_node) != 0) {
        return -1;
    }

    if (root_node.id != 0x1A45DFA3) {
        return -1;
    }

    read_master(result, ep, root_node);

    return 0;
}

int32_t parser::parse_segment(ebml_node &result)
{
    ebml_node segment_node;
    if (ep.parse_next(segment_node) != 0) {
        return -1;
    }

    if (segment_node.id != 0x18538067) {
        return -1;
    }

    return 0;
}

uint32_t parser::parse_next_element()
{
    ebml_node node;
    if (ep.parse_next(node) != 0) {
        return static_cast<uint32_t>(-1);
    }

    if (!callback.need_parse_node(node)) {
        ep.set_stream_pos(node.position + node.size);
        return node.id;
    }

    switch (node.id) {
        case EBML_META_SEEK_INFO_ID:
            do_meta_seek_info_parse(node);
            break;
        case EBML_SEGMENT_INFO_ID:
            do_segment_info_parse(node);
            break;
        case EBML_CLUSTER_ID:
            do_cluster_parse(node);
            break;
        case EBML_TRACK_ID:
            do_track_parse(node);
            break;
        case EBML_CUEING_DATA_ID:
            do_cueing_data_parse(node);
            break;
        case EBML_ATTACHMENT_ID:
            do_attachment_parse(node);
            break;
        case EBML_CHAPTERS_ID:
            do_chapters_parse(node);
            break;
        case EBML_TAGGING_ID:
            do_tagging_parse(node);
            break;
        default:
            do_skip_parse(node);
            break;
    }

    return node.id;
}

void parser::seek(int64_t position)
{
    ep.set_stream_pos(position);
}

int32_t parser::resync_to_cluster()
{
    // this operation consumes that byte_sizeof(EBML_CLUSTER_ID) == 4
    return ep.sync_to_ebml_id(EBML_CLUSTER_ID);
}

int32_t parser::skip_to_cluster()
{
    auto curr_pos = ep.get_stream_pos();
    auto pos = curr_pos;

    while (true) {
        ebml_node node;
        if (ep.parse_next(node) != 0) {
            ep.set_stream_pos(curr_pos);
            return -1;
        }

        if (node.id == EBML_CLUSTER_ID) {
            ep.set_stream_pos(pos);
            return 0;
        }

        pos = node.position + node.size;
        ep.set_stream_pos(pos);
    }
}

void parser::do_meta_seek_info_parse(const ebml_node &node)
{
    meta_seek_head meta_seek_info;
    read_master(meta_seek_info, ep, node);
    callback.handle_meta_seek_info(meta_seek_info, node);
}

void parser::do_segment_info_parse(const ebml_node &node)
{
    segment_info seg_info;
    read_master(seg_info, ep, node);
    callback.handle_segment_info(seg_info, node);
}

void parser::do_cluster_parse(const ebml_node &node)
{
    cluster cluster_info;
    read_master(cluster_info, ep, node);
    callback.handle_cluster(cluster_info, node);
}

void parser::do_track_parse(const ebml_node &node)
{
    track track_info;
    read_master(track_info, ep, node);
    callback.handle_track(track_info, node);
}

void parser::do_cueing_data_parse(const ebml_node &node)
{
    cue cue_data;
    read_master(cue_data, ep, node);
    callback.handle_cueing_data(cue_data, node);
}

void parser::do_attachment_parse(const ebml_node &node)
{
    attachment attach;
    read_master(attach, ep, node);
    callback.handle_attachment(attach, node);
}

void parser::do_chapters_parse(const ebml_node &node)
{
    chapter chapter_info;
    read_master(chapter_info, ep, node);
    callback.handle_chapters(chapter_info, node);
}

void parser::do_tagging_parse(const ebml_node &node)
{
    tags tags_info;
    read_master(tags_info, ep, node);
    callback.handle_tagging(tags_info, node);
}

void parser::do_skip_parse(const ebml_node &node)
{
    callback.handle_unknown(node);
}
