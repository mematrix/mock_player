include_directories(include)
include_directories(../../common)

message(STATUS "env: $ENV{CPLUS_INCLUDE_PATH}, $ENV{CPLUS_INCLUDE_PATH}")

find_path(FFMPEG_INCLUDE libavcodec/avcodec.h $ENV{CPLUS_INCLUDE_PATH})
find_library(FFMPEG_LIBRARY avcodec.lib $ENV{LIBRARY_PATH})

message(STATUS "ffmpeg include path: ${FFMPEG_INCLUDE}, ffmpeg library path: ${FFMPEG_LIBRARY}")
if (NOT FFMPEG_INCLUDE OR NOT FFMPEG_LIBRARY)
    message(FATAL_ERROR "ffmpeg not found.")
endif ()

include_directories(${FFMPEG_INCLUDE})

aux_source_directory(src MatroskaSRC)
message(STATUS ${MatroskaSRC})

if (BUILD_SEPERATE_LIB)
    add_library(matroska ${MatroskaSRC})
    target_include_directories(matroska PUBLIC include)
    target_link_libraries(matroska ${FFMPEG_LIBRARY})
endif (BUILD_SEPERATE_LIB)

aux_source_directory(test MatroskaTestSRC)
set(SuppressWaring include/matroska_demux.hpp include/matroska_track.hpp include/matroska_common.hpp)  # add to suppress clion warning, and enable clion analyze the header file
add_executable(matroska_test ${MatroskaSRC} ${MatroskaTestSRC} ${SuppressWaring})
target_link_libraries(matroska_test ${FFMPEG_LIBRARY})