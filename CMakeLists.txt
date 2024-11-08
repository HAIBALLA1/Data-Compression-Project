cmake_minimum_required(VERSION 3.29)
project(DM_Compression_formats_d_images CXX)  # Spécifie CXX pour C++

set(CMAKE_CXX_STANDARD 11)  # Pour le C++, utilise CXX_STANDARD

include_directories(.)
include_directories(src)

add_executable(DM_Compression_formats_d-images
        src/encoding_lossly.cpp
        src/encoding_lossly.h

)
