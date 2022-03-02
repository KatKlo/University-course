cmake_minimum_required(VERSION 3.20)
project(2_phone_mapper)

set(CMAKE_CXX_STANDARD 17)

add_executable(phone_mapper main.cpp maptel.cc maptel.h)
