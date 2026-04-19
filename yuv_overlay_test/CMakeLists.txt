cmake_minimum_required(VERSION 3.10)
project(yuv_overlay_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(yuv_overlay
    main.cpp
    bmp_reader.cpp
    yuv420.cpp
    rgb_to_yuv.cpp
    overlay.cpp
    support_function.cpp
)