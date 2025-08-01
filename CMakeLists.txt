cmake_minimum_required(VERSION 3.30)
project(asd)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(asd
    asd.hpp
    main.cpp)
