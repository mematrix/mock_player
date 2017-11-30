//
// Created by Charles on 2017/11/15.
//

#include "../src/parser.hpp"

#include <ostream>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace player::container::matroska;


template<typename T, typename Mem>
void log_member_info(const T &obj, const Mem &m, uint32_t enum_id, std::ostream &log, const char *name)
{
    log << name << ": " << m;
    if (obj.is_user_set(enum_id)) {
        log << " [explicit value]" << std::endl;
    } else if (obj.is_set(enum_id)) {
        log << " [default value]" << std::endl;
    } else {
        log << std::endl;
    }
};

int64_t get_millisecond_since_1970()
{
    using namespace std::chrono;
    return time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count();
}

class matroska_file_parser_test : public parser_callback
{
    matroska_parser parser;
    std::ostream &log;

public:
    matroska_file_parser_test(std::istream &stream, std::ostream &output_log) : parser(stream, *this), log(output_log) { }

    void run()
    {
        constexpr auto ff = static_cast<uint32_t>(-1);

        ebml_header header;
        if (parser.parse_ebml_header(header) != 0) {
            log << "[" << get_millisecond_since_1970() << "]" << "could not read ebml header, maybe not a matroska format stream(file)?" << std::endl;
            return;
        }
        log << "[" << get_millisecond_since_1970() << "] start parse. ebml header info:" << std::endl;
        log_member_info(header, header.version, header.VERSION, log, " version");
        log_member_info(header, header.read_version, header.READ_VERSION, log, " read version");
        log_member_info(header, header.max_id_length, header.MAX_ID_LENGTH, log, " max id length");
        log_member_info(header, header.max_size_length, header.MAX_SIZE_LENGTH, log, " max size length");
        log_member_info(header, header.doc_type, header.DOC_TYPE, log, " doc type");
        log_member_info(header, header.doc_type_version, header.DOC_TYPE_VERSION, log, " doc type version");
        log_member_info(header, header.doc_type_read_version, header.DOC_TYPE_READ_VERSION, log, " doc type read version");

        ebml_node node;
        if (parser.parse_segment(node) != 0) {
            log << "[" << get_millisecond_since_1970() << "]" << "error: could not read segment." << std::endl;
            return;
        }
        log << "[" << get_millisecond_since_1970() << "]" << "segment (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;

        while (ff != parser.parse_next_element()) { }
    }

public:
    void handle_meta_seek_info(const meta_seek_head &seek_head, const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "meta seek info (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
        log << " count: " << seek_head.seeks.size() << std::endl;
        for (const auto &it : seek_head.seeks) {
            log << " - id: (binary: " << it.id.first << ", " << it.id.second << "), position: " << it.position << std::endl;
        }
    }

    void handle_segment_info(const segment_info &seg_info, const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "segment info (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
        log_member_info(seg_info, seg_info.filename, seg_info.FILENAME, log, " file name");
        log_member_info(seg_info, seg_info.timecode_scale, seg_info.TIMECODE_SCALE, log, " timecode scale");
        log_member_info(seg_info, seg_info.duration, seg_info.DURATION, log, " duration");
        log_member_info(seg_info, seg_info.date_utc, seg_info.DATE_UTC, log, " date utc");
        log_member_info(seg_info, seg_info.muxing_app, seg_info.MUXING_APP, log, " muxing app");
        log_member_info(seg_info, seg_info.writing_app, seg_info.WRITING_APP, log, " writing app");
    }

    void handle_cluster(const cluster &cluster_info, const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "cluster (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
        log_member_info(cluster_info, cluster_info.timecode, cluster_info.TIMECODE, log, " timecode");
        log_member_info(cluster_info, cluster_info.position, cluster_info.POSITION, log, " position");
        log_member_info(cluster_info, cluster_info.prev_size, cluster_info.PREV_SIZE, log, " prev size");
        log << " simple block count: " << cluster_info.simple_blocks.size() << std::endl;
        log << " block count: " << cluster_info.block_group.size() << std::endl;
        for (const auto &it : cluster_info.block_group) {
            log_member_info(it, it.duration, it.DURATION, log, " duration");
            log << "  ref block (size: " << it.reference_block.size() << "):";
            for (const auto &ref_it : it.reference_block) {
                log << " " << ref_it;
            }
            log << std::endl;
        }
    }

    void handle_track(const track &track_info, const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "track (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
        log << " track count: " << track_info.entries.size() << std::endl;
        for (const auto &it : track_info.entries) {
            log_member_info(it, it.number, it.NUMBER, log, " number");
            log_member_info(it, it.type, it.TYPE, log, " type");
            log_member_info(it, it.default_duration, it.DEFAULT_DURATION, log, " default duration");
            log_member_info(it, it.name, it.NAME, log, " name");
            log_member_info(it, it.codec_id, it.CODEC_ID, log, " codec id");
            log_member_info(it, it.codec_name, it.CODEC_NAME, log, " codec name");
            if (it.video) {
                const auto &video = *it.video;
                log << " video:" << std::endl;
                log_member_info(video, video.flag_interlaced, video.FLAG_INTERLACED, log, "  interlaced");
                log_member_info(video, video.pixel_width, video.PIXEL_WIDTH, log, "  pixel width");
                log_member_info(video, video.pixel_height, video.PIXEL_HEIGHT, log, "  pixel height");
                log_member_info(video, video.pixel_crop_bottom, video.PIXEL_CROP_BOTTOM, log, "  pixel crop bottom");
                log_member_info(video, video.pixel_crop_top, video.PIXEL_CROP_TOP, log, "  pixel crop top");
                log_member_info(video, video.pixel_crop_left, video.PIXEL_CROP_LEFT, log, "  pixel crop left");
                log_member_info(video, video.pixel_crop_right, video.PIXEL_CROP_RIGHT, log, "  pixel crop right");
                log_member_info(video, video.display_width, video.DISPLAY_WIDTH, log, "  display width");
                log_member_info(video, video.display_height, video.DISPLAY_HEIGHT, log, "  display height");
                log_member_info(video, video.display_unit, video.DISPLAY_UNIT, log, "  display unit");
                log_member_info(video, video.aspect_ratio_type, video.ASPECT_RATIO_TYPE, log, "  aspect ratio type");
            }
            if (it.audio) {
                const auto &audio = *it.audio;
                log << " audio:" << std::endl;
                log_member_info(audio, audio.sampling_frequency, audio.SAMPLING_FREQUENCY, log, "  sample frequency");
                log_member_info(audio, audio.output_sampling_frequency, audio.OUTPUT_SAMPLING_FREQUENCY, log, "  output sample frequency");
                log_member_info(audio, audio.channels, audio.CHANNELS, log, "  channels");
                log_member_info(audio, audio.bit_depth, audio.BIT_DEPTH, log, "  bit depth");
            }
            log << std::endl;
        }
    }

    void handle_cueing_data(const cue &cue_data, const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "cueing data (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
        log << " cue count: " << cue_data.cue_points.size() << std::endl;
        for (const auto &it : cue_data.cue_points) {
            log_member_info(it, it.time, it.TIME, log, " time");
        }
    }

    void handle_attachment(const attachment &attach, const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "attachment (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
        log << " attach count: " << attach.attached_files.size() << std::endl;
        for (const auto &it : attach.attached_files) {
            log_member_info(it, it.name, it.NAME, log, " name");
            log_member_info(it, it.mine_type, it.MINE_TYPE, log, " mime type");
            log_member_info(it, it.description, it.DESCRIPTION, log, " description");
            log << std::endl;
        }
    }

    static void print_chapter_atom(const multiple_master<chapter_atom> &atom_list, unsigned level, std::ostream &log)
    {
        for (const auto &atom : atom_list) {
            log << " " << level << " string uid: " << atom.string_uid << ", start time: " << atom.time_start << ", end time: " << atom.time_end;
            log << " (displays:";
            for (const auto &display : atom.displays) {
                log << display.string << ",";
            }
            log << ")" << std::endl;

            print_chapter_atom(atom.atoms, level + 1, log);
        }
    }

    void handle_chapters(const chapter &chapter_info, const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "chapters (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
        log << " chapters count: " << chapter_info.edition_entries.size() << std::endl;
        for (const auto &entry : chapter_info.edition_entries) {
            log << " uid: " << entry.uid << ", hidden: " << entry.flag_hidden << ", default: " << entry.flag_default << ", ordered: " << entry.flag_ordered << std::endl;
            print_chapter_atom(entry.chapter_atoms, 1, log);
            log << std::endl;
        }
    }

    static void print_simple_tag(const multiple_master<simple_tag> &tag_list, unsigned level, std::ostream &log)
    {
        for (const auto &tag : tag_list) {
            log << " " << level << " name: " << tag.name << ", language: " << tag.language << ", default: " << tag.tag_default << std::endl;

            print_simple_tag(tag.tags, level + 1, log);
        }
    }

    void handle_tagging(const tags &tags_info, const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "tag (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
        log << " tag count: " << tags_info.tag_list.size() << std::endl;
        for (const auto &tag : tags_info.tag_list) {
            log << " type value: " << tag.target.type_value << ", type: " << tag.target.type << ", uid count: " << tag.target.uids.size() << std::endl;
            print_simple_tag(tag.simple_tags, 1, log);
            log << std::endl;
        }
    }

    void handle_unknown(const ebml_node &node) override
    {
        log << "[" << get_millisecond_since_1970() << "]" << "unknown (id: " << node.id << ", position: " << node.position << ", size: " << node.size << ")" << std::endl;
    }

    bool need_parse_node(const ebml_node &node) override
    {
        return true;
    }
};


int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cerr << "argument list too short. usage: [exe file] <mkv file path> <out log file path>" << std::endl;
        return -1;
    }

    std::ifstream ifs(argv[1], std::ifstream::in | std::ifstream::binary);
    if (!ifs) {
        std::cerr << "could not open file: " << argv[1] << std::endl;
        return -2;
    }
    std::ofstream ofs(argv[2], std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    if (!ofs) {
        std::cerr << "could not open file to write: " << argv[2] << std::endl;
        return -3;
    }

    matroska_file_parser_test test(ifs, ofs);
    test.run();

    return 0;
}
