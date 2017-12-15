//
// Created by Charles on 2017/12/1.
//

#include "matroska_demux.hpp"
#include "parser.hpp"

#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_set>


namespace player {
namespace container {
namespace matroska {


constexpr size_t min_buffer_size = 5 * 1024 * 1024;     // min buffer size


struct matroska_demux_t
{
    size_t max_buffer_size = 20 * 1024 * 1024;  // 20MB
    uint64_t min_duration_ms = 5 * 1000;    // minimum duration of cache in millisecond

    std::thread inner_thread;
    bool thread_terminate = false;
    bool thread_running = false;

    std::mutex demux_mutex;
    std::condition_variable demux_cond;

    // segment start position.
    int64_t segment_start = 0;
    // whether under stream is eof.
    bool eof = false;

    std::unordered_set<int64_t> parsed_header;
    std::vector<ebml_node> delay_parse_header;
    bool can_play = false;

    track track_infos;
    matroska_demux_info demux_info;
    size_t total_size = 0;      // total cache size of all track streams
    size_t forward_size = 0;    // unread cache size of all track streams

    matroska_demux_t() = default;
};


class matroska_demux_impl : public parser_callback
{
public:
    matroska_demux_impl(std::istream &stream, demux_callback &cb);

    void set_buffer_max_size(size_t max_size);

    bool start_thread();

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
    static void demux_thread(matroska_demux_impl *demux_impl) noexcept;

    bool thread_work();

    bool read_packet();

    void read_delay_header();

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

bool matroska_demux_impl::start_thread()
{
    if (!demux.thread_running) {
        demux.thread_terminate = false;
        try {
            demux.inner_thread = std::thread(matroska_demux_impl::demux_thread, this);
            demux.thread_running = true;
        } catch (...) {
            return false;
        }
    }

    return demux.thread_running;
}

void matroska_demux_impl::stop_thread_async()
{
    if (demux.thread_running) {
        demux.thread_terminate = true;
        demux.demux_cond.notify_one();
    }
}

void matroska_demux_impl::stop_thread()
{
    if (demux.thread_running) {
        stop_thread_async();
        demux.inner_thread.join();
    }
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
    if (EBML_CLUSTER_ID == node.id) {
        return true;    // always parse cluster
    }

    if (!demux.can_play && EBML_CUEING_DATA_ID == node.id) {
        demux.delay_parse_header.push_back(node);
        return false;   // delay parse cue data
    }

    auto result = demux.parsed_header.insert(node.position).second;     // if exists position, do not parse again
    return result;
}

// the thread model refers to the implementation of the mpv-player.
void matroska_demux_impl::demux_thread(matroska_demux_impl *demux_impl) noexcept
{
    class status_raii
    {
        bool &running;

    public:
        explicit status_raii(bool &r) : running(r) { running = true; }

        ~status_raii() { running = false; }
    };

    auto impl = demux_impl;
    auto &demux = impl->demux;
    status_raii sr(demux.thread_running);

    // if stream is not a matroska format stream, return.
    ebml_header header;
    if (impl->parser.parse_ebml_header(header) != 0) {
        impl->callback.notify_event(demux_notify_event::stream_error);
        return;
    }
    if (header.doc_type_read_version > 4) {
        impl->callback.notify_event(demux_notify_event::stream_not_support);
        return;
    }
    demux.demux_info.file_info.doc_type = std::move(header.doc_type);

    ebml_node node;
    if (impl->parser.parse_segment(node) != 0) {
        impl->callback.notify_event(demux_notify_event::stream_error);
        return;
    }
    demux.segment_start = node.position;

    std::unique_lock lock(demux.demux_mutex);
    while (!demux.thread_terminate) {
        if (impl->thread_work()) {
            continue;
        }

        demux.demux_cond.notify_one();
        demux.demux_cond.wait(lock);
    }
}

bool matroska_demux_impl::thread_work()
{
    if (!demux.eof) {
        return read_packet();
    }

    return false;
}

bool matroska_demux_impl::read_packet()
{
    bool read_more = false;
    bool prefetch_more = false;
    for (const auto &track_info : demux.demux_info.track_list) {
        read_more |= track_info.eager && track_info.packet_buffer.empty();
        if (track_info.eager && track_info.base_ts != AV_NOPTS_VALUE &&
            demux.min_duration_ms > 0 && track_info.last_ts != AV_NOPTS_VALUE &&
            track_info.last_ts >= track_info.base_ts) {
            prefetch_more |= track_info.last_ts - track_info.base_ts < demux.min_duration_ms;
        }
    }
    if (demux.total_size > demux.max_buffer_size) {
        read_delay_header();
        if (!read_more) {
            return false;
        }
        // warning buffer overflow
        callback.notify_event(demux_notify_event::buffer_overflow_warning);
        return false;
    }

    if (!read_more && !prefetch_more) {
        read_delay_header();
        return false;
    }

    if (parser.parse_next_element() == ~0U) {
        demux.eof = true;
    }

    return true;
}

void matroska_demux_impl::read_delay_header()
{
    if (demux.can_play && !demux.delay_parse_header.empty()) {
        for (const auto &node : demux.delay_parse_header) {
            parser.parse_element(node);
        }
        demux.delay_parse_header.clear();
        demux.delay_parse_header.shrink_to_fit();
    }
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
    impl->set_buffer_max_size(max_size);
}

bool matroska_demux::start_thread()
{
    return impl->start_thread();
}

void matroska_demux::stop_thread_async()
{
    impl->stop_thread_async();
}

void matroska_demux::stop_thread()
{
    impl->stop_thread();
}

void matroska_demux::select_track(uint32_t id)
{
    impl->select_track(id);
}

void matroska_demux::unselect_track(uint32_t id)
{
    impl->unselect_track(id);
}

std::shared_ptr<demux_packet> matroska_demux::read_packet(uint32_t track_id)
{
    return impl->read_packet(track_id);
}

int32_t matroska_demux::read_packet_async(uint32_t track_id, std::shared_ptr<demux_packet> &out_packet)
{
    return impl->read_packet_async(track_id, out_packet);
}

int matroska_demux::seek(uint64_t millisecond)
{
    return impl->seek(millisecond);
}
