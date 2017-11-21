//
// Created by Charles on 2017/11/16.
//

#ifndef MOCK_PLAYER_ENDIAN_UTIL_HPP
#define MOCK_PLAYER_ENDIAN_UTIL_HPP


#include <cstdint>
#include <cassert>


namespace player {
namespace util {

static int64_t from_big_endian_variable(const char *source, unsigned int size)
{
    assert(size <= 8);

    int64_t result = 0;
    for (int i = 0; i < size; ++i) {
        result |= source[i] << (8 - i);
    }

    return result >> (8 - size);
}

static uint64_t from_big_endian_variable_unsigned(const char *source, unsigned int size)
{
    assert(size <= 8);

    uint64_t result = 0;
    for (int i = 0; i < size; ++i) {
        result |= source[i] << (size - i);
    }

    return result;
}

static int64_t from_big_endian(const char *source)
{
    int64_t result = 1;
    if (((char *)(&result))[0] == 0) {
        // big endian
        return *(int64_t *)(source);
    } else {
        // little endian
        auto pc = (char *)(&result);
        for (int i = 0; i < 8; ++i) {
            pc[7 - i] = source[i];
        }

        return result;
    }
}

static uint64_t from_big_endian_unsigned(const char *source)
{
    uint64_t result = 1;
    if (((char *)(&result))[0] == 0) {
        // big endian
        return *(uint64_t *)(source);
    } else {
        // little endian
        auto pc = (char *)(&result);
        for (int i = 0; i < 8; ++i) {
            pc[7 - i] = source[i];
        }

        return result;
    }
}

static double from_big_endian_double(const char *source)
{
    // 8 bytes
    int64_t result = 1;
    if (((char *)(&result))[0] == 0) {
        return *(double *)(source);
    } else {
        auto pc = (char *)(&result);
        for (int i = 0; i < 8; ++i) {
            pc[7 - i] = source[i];
        }

        return *(double *)(pc);
    }
}

static float from_big_endian_float(const char *source)
{
    // 4 bytes
    int32_t result = 1;
    if (((char *)(&result))[0] == 0) {
        return *(float *)(source);
    } else {
        auto pc = (char *)(&result);
        for (int i = 0; i < 4; ++i) {
            pc[3 - i] = source[i];
        }

        return *(float *)(pc);
    }
}

}
}

#endif //MOCK_PLAYER_ENDIAN_UTIL_HPP
