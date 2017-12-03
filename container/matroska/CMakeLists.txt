include_directories(include)
include_directories(../../common)

aux_source_directory(src MatroskaSRC)
message(${MatroskaSRC})

if (BUILD_SEPERATE_LIB)
    add_library(matroska ${MatroskaSRC})
    target_include_directories(matroska PUBLIC include)
endif (BUILD_SEPERATE_LIB)

aux_source_directory(test MatroskaTestSRC)
set(SuppressWaring include/matroska_demux.hpp)  # add to suppress clion warning, and enable clion analyze the header file
add_executable(matroska_test ${MatroskaSRC} ${MatroskaTestSRC} ${SuppressWaring})