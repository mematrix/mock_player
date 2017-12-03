//
// Created by Charles on 2017/12/1.
//

#include "matroska_demux.hpp"
#include "parser.hpp"


namespace player {
namespace container {
namespace matroska {


constexpr size_t min_buffer_size = 5 * 1024 * 1024;     // min buffer size


struct matroska_demux_t
{
    size_t max_buffer_size = 20 * 1024 * 1024;  // 20MB

    matroska_demux_t() = default;
};


class matroska_demux_impl : public parser_callback
{
public:
    matroska_demux_impl(std::istream &stream, demux_callback &cb);

    void set_buffer_max_size(size_t max_size);

    void start_thread();

    void stop_thread_async();

    void stop_thread();

    void select_track(uint32_t id);

    void unselect_track(uint32_t id);

    std::shared_ptr<demux_packet> read_packet(uint32_t track_id);

    int32_t read_packet_async(uint32_t track_id, std::shared_ptr<demux_packet> &out_packet);

    int seek(uint64_t millisecond);

    void handle_meta_seek_info(const meta_seek_head &seek_head, const ebml_node &node) override;

    void handle_segment_info(const segment_info &seg_info, const ebml_node &node) override;

    void handle_cluster(const cluster &cluster_info, const ebml_node &node) override;

    void handle_track(const track &track_info, const ebml_node &node) override;

    void handle_cueing_data(const cue &cue_data, const ebml_node &node) override;

    void handle_attachment(const attachment &attach, const ebml_node &node) override;

    void handle_chapters(const chapter &chapter_info, const ebml_node &node) override;

    void handle_tagging(const tags &tags_info, const ebml_node &node) override;

    void handle_unknown(const ebml_node &node) override;

    bool need_parse_node(const ebml_node &node) override;

private:
    matroska_parser parser;
    demux_callback &callback;
    matroska_demux_t demux;
};

matroska_demux_impl::matroska_demux_impl(std::istream &stream, demux_callback &cb) : parser(stream, *this), callback(cb), demux()
{
}

void matroska_demux_impl::set_buffer_max_size(size_t max_size)
{
    if (max_size >= min_buffer_size) {
        demux.max_buffer_size = max_size;
    }
}

void matroska_demux_impl::start_thread()
{

}

void matroska_demux_impl::stop_thread_async()
{

}

void matroska_demux_impl::stop_thread()
{

}

void matroska_demux_impl::select_track(uint32_t id)
{

}

void matroska_demux_impl::unselect_track(uint32_t id)
{

}

std::shared_ptr<demux_packet> matroska_demux_impl::read_packet(uint32_t track_id)
{
    return std::shared_ptr<demux_packet>();
}

int32_t matroska_demux_impl::read_packet_async(uint32_t track_id, std::shared_ptr<demux_packet> &out_packet)
{
    return 0;
}

int matroska_demux_impl::seek(uint64_t millisecond)
{
    return 0;
}

void matroska_demux_impl::handle_meta_seek_info(const meta_seek_head &seek_head, const ebml_node &node)
{

}

void matroska_demux_impl::handle_segment_info(const segment_info &seg_info, const ebml_node &node)
{

}

void matroska_demux_impl::handle_cluster(const cluster &cluster_info, const ebml_node &node)
{

}

void matroska_demux_impl::handle_track(const track &track_info, const ebml_node &node)
{

}

void matroska_demux_impl::handle_cueing_data(const cue &cue_data, const ebml_node &node)
{

}

void matroska_demux_impl::handle_attachment(const attachment &attach, const ebml_node &node)
{

}

void matroska_demux_impl::handle_chapters(const chapter &chapter_info, const ebml_node &node)
{

}

void matroska_demux_impl::handle_tagging(const tags &tags_info, const ebml_node &node)
{

}

void matroska_demux_impl::handle_unknown(const ebml_node &node)
{

}

bool matroska_demux_impl::need_parse_node(const ebml_node &node)
{
    return false;
}


}
}
}


using namespace player::container::matroska;

matroska_demux::matroska_demux(std::istream &stream, demux_callback &cb) : impl(new matroska_demux_impl(stream, cb))
{
}

void matroska_demux::set_buffer_max_size(size_t max_size)
{
    (*impl).set_buffer_max_size(max_size);  // clion bug. it can not interpret the operator-> of std::unique_ptr
}

void matroska_demux::start_thread()
{
    (*impl).start_thread();
}

void matroska_demux::stop_thread_async()
{
    (*impl).stop_thread_async();
}

void matroska_demux::stop_thread()
{
    (*impl).stop_thread();
}

void matroska_demux::select_track(uint32_t id)
{
    (*impl).select_track(id);
}

void matroska_demux::unselect_track(uint32_t id)
{
    (*impl).unselect_track(id);
}

std::shared_ptr<demux_packet> matroska_demux::read_packet(uint32_t track_id)
{
    return (*impl).read_packet(track_id);
}

int32_t matroska_demux::read_packet_async(uint32_t track_id, std::shared_ptr<demux_packet> &out_packet)
{
    return (*impl).read_packet_async(track_id, out_packet);
}

int matroska_demux::seek(uint64_t millisecond)
{
    return (*impl).seek(millisecond);
}
