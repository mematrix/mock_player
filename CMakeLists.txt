cmake_minimum_required(VERSION 3.8)
project(mock_player)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(container)

# adding hpp file to source list, to suppress clion warning
set(MOCK_HPP_FILES common/util/endian_util.hpp common/util/pointer_util.hpp common/util/number_util.hpp)

set(SOURCE_FILES library.cpp library.h)
add_library(mock_player ${SOURCE_FILES} ${MOCK_HPP_FILES})