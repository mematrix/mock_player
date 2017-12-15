//
// Created by Charles on 2017/12/15.
//

#ifndef MOCK_PLAYER_NUMBER_UTIL_HPP
#define MOCK_PLAYER_NUMBER_UTIL_HPP


#include <cstdint>


namespace player {
namespace util {

__forceinline int64_t raw_ts(int64_t base, int64_t relative, int64_t scale)
{
    return (base + relative) * scale / 1000000;
}

}
}


#endif //MOCK_PLAYER_NUMBER_UTIL_HPP
