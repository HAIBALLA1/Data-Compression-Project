cmake_minimum_required(VERSION 3.16)
project(DM_Compression_formats_d_images LANGUAGES CXX)

# C++20 strict
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Headers dans src/
add_library(project_headers INTERFACE)
target_include_directories(project_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Exécutable de test
add_executable(dc_test
    src/test.cpp
    # si plus tard tu sépares des implés en .cpp, ajoute-les ici
)

target_link_libraries(dc_test PRIVATE project_headers)

# Warnings utiles
if (MSVC)
  target_compile_options(dc_test PRIVATE /W4 /permissive-)
else()
  target_compile_options(dc_test PRIVATE -Wall -Wextra -Wpedantic)
endif()

