//
// Created by Charles on 2017/12/8.
//

#ifndef MOCK_PLAYER_VALUE_SET_HELPER_HPP
#define MOCK_PLAYER_VALUE_SET_HELPER_HPP


#include <cstdint>
#include <utility>
#include <string>
#include <vector>
#include <list>


namespace player {
namespace container {
namespace matroska {

// todo: use position & length pair to point a binary data, instead of reading data to memory.
using binary = std::pair<int64_t, uint64_t>;
using utf8_string = std::string;    // include ascii string.
using uint64_list = std::vector<uint64_t>;
using int64_list = std::vector<int64_t>;
using string_list = std::list<utf8_string>;
using binary_list = std::list<binary>;

template<typename T>
using multiple_master = std::list<T>;
template<typename T>
using optional_master = std::unique_ptr<T>;


template<typename T>
struct value_set_helper
{
    const T &cast() const { return static_cast<const T &>(*this); }

    bool is_set(int32_t element) const
    {
        return (element & (cast().get_mask() | T::DEFAULT_VALUE_SET)) != 0;
    }

    bool is_user_set(int32_t element) const
    {
        return (element & cast().get_mask()) != 0;
    }

    bool is_all_set(int32_t elements) const
    {
        return (elements & (cast().get_mask() | T::DEFAULT_VALUE_SET)) == elements;
    }
};

#define USING_VALUE_METHOD(value_set) \
    enum { DEFAULT_VALUE_SET = (value_set) }; \
    int32_t mask = 0; \
    int32_t get_mask() const { return mask; } \
    using value_set_helper::is_set; \
    using value_set_helper::is_user_set; \
    using value_set_helper::is_all_set

}
}
}


#endif //MOCK_PLAYER_VALUE_SET_HELPER_HPP
