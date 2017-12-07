//
// Created by Charles on 2017/12/8.
//

#ifndef MOCK_PLAYER_POINTER_UTIL_HPP
#define MOCK_PLAYER_POINTER_UTIL_HPP


namespace player {
namespace util {

class undefined_class_suffix;

union pointer_type_detect
{
    void *_t_pointer;
    const void *_t_const_pointer;
    int undefined_class_suffix::* _t_class_member_pointer;
};

union any_pointer_data
{
    // inspired by mingw gcc. source header: <std_function.h>

    void *access() noexcept { return &_pod_data[0]; }

    const void *access() const noexcept { return &_pod_data[0]; }

    template<typename Tp>
    Tp &access() noexcept { return *static_cast<Tp *>(access()); }

    template<typename Tp>
    const Tp &access() const noexcept { return *static_cast<const Tp *>(access()); }

    pointer_type_detect _unused_value;
    char _pod_data[sizeof(pointer_type_detect)];
};

}
}


#endif //MOCK_PLAYER_POINTER_UTIL_HPP
