//
// Created by Charles on 2017/12/15.
//

#ifndef MOCK_PLAYER_BUFFER_HPP
#define MOCK_PLAYER_BUFFER_HPP


#include <cstdint>
#include <new>
#include <utility>


namespace player {
namespace io {

class bytes_buffer
{
    size_t size;
    char *buf;

public:
    // TODO: use memory pool
    explicit bytes_buffer(size_t s) noexcept : buf(new(std::nothrow) char[s])
    {
        size = buf ? s : 0;
    }

    bytes_buffer(bytes_buffer &&that) noexcept : size(that.size), buf(that.buf)
    {
        that.size = 0;
        that.buf = nullptr;
    }

    bytes_buffer &operator=(bytes_buffer &&that) noexcept
    {
        bytes_buffer tmp(std::move(that));
        swap(tmp);
        return *this;
    }

    ~bytes_buffer()
    {
        if (buf) {
            delete[] buf;
            buf = nullptr;
            size = 0;
        }
    }

    void swap(bytes_buffer &that) noexcept
    {
        using std::swap;
        swap(size, that.size);
        swap(buf, that.buf);
    }

    size_t length() { return size; }

    operator bool() const
    {
        return nullptr != buf;
    }

    operator char *()
    {
        return buf;
    }

    char *get()
    {
        return buf;
    }

    const char *get() const
    {
        return buf;
    }
};


template<unsigned int Size>
class stack_bytes_buffer
{
    char buf[Size];

public:
    constexpr static unsigned length()
    {
        return Size;
    }

    operator char *()
    {
        return buf;
    }

    char *get()
    {
        return buf;
    }

    const char *get() const
    {
        return buf;
    }
};

using stack_8_bytes_buffer = stack_bytes_buffer<8>;
using stack_256_bytes_buffer = stack_bytes_buffer<256>;

}
}


#endif //MOCK_PLAYER_BUFFER_HPP
