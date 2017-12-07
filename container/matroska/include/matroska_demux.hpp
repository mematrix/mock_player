//
// Created by Charles on 2017/12/1.
//

#ifndef MOCK_PLAYER_MATROSKA_DEMUX_HPP
#define MOCK_PLAYER_MATROSKA_DEMUX_HPP


#include <cstdint>
#include <memory>
#include <iosfwd>
#include <string>


namespace player {
namespace container {
namespace matroska {


enum class demux_notify_event
{
    file_info_loaded,
    cue_data_loaded,
    track_info_loaded,
    buffer_overflow_warning,
    begin_seek,
    end_seek,
    stream_not_support,
    stream_error
};


enum matroska_track_type
{
    MATROSKA_TRACK_VIDEO = 1,
    MATROSKA_TRACK_AUDIO = 2,
    MATROSKA_TRACK_COMPLEX = 3,
    MATROSKA_TRACK_LOGO = 0x10,
    MATROSKA_TRACK_SUBTITLE = 0x11,
    MATROSKA_TRACK_BUTTONS = 0x12,
    MATROSKA_TRACK_CONTROL = 0x20
};

struct matroska_demux_info
{
    struct file_info
    {
        std::string doc_type;
        double duration_ms = 0;     // duration in millisecond
        int64_t date_utc_ms = 0;    // utc date in millisecond
        std::string title;
        std::string muxing_app;
        std::string writing_app;

        file_info() = default;
    };

    struct track_info
    {
        uint64_t uid = 0;
        uint64_t type = 0;
        bool enable = false;
        bool default_track = false;
        bool force = false;
        bool valid = false;         // identify whether the track info is valid
        std::string name;
        std::string language;
        std::string codec_id;
        std::string codec_name;

        track_info() = default;
    };
};


class demux_packet;

class demux_callback
{
public:
    virtual void notify_event(demux_notify_event event) = 0;

    virtual void wakeup() { }

    virtual ~demux_callback() = default;
};


class matroska_demux_impl;

class matroska_demux
{
public:
    matroska_demux(std::istream &stream, demux_callback &cb);

    matroska_demux(matroska_demux &&) = default;

    matroska_demux &operator=(matroska_demux &&) = default;

    void set_buffer_max_size(size_t max_size);

    bool start_thread();

    void stop_thread_async();

    void stop_thread();

    void select_track(uint32_t id);

    void unselect_track(uint32_t id);

    /**
     * Read a packet from the given track stream. Might block. Returns nullptr on EOF.
     *
     * @param track_id which track stream to be read
     * @return result packet
     */
    std::shared_ptr<demux_packet> read_packet(uint32_t track_id);

    /**
     * Poll the demuxer queue, and if there's a packet, return it. Otherwise, just make
     * the demuxer thread read packets for this stream, and if there's at least one
     * packet, call the wakeup callback.
     *
     * @param track_id which track stream to be read
     * @param out_packet result packet
     * @return
     * < 0: EOF was reached, out_packet=nullptr
     * == 0: no new packet yet, but maybe later, out_packet=nullptr
     * > 0: new packet read, out_packet is set
     */
    int32_t read_packet_async(uint32_t track_id, std::shared_ptr<demux_packet> &out_packet);

    int seek(uint64_t millisecond);

private:
    std::unique_ptr<matroska_demux_impl> impl;
};


}
}
}


#endif //MOCK_PLAYER_MATROSKA_DEMUX_HPP
