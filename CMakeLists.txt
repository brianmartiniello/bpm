cmake_minimum_required(VERSION 3.16)

project(bpm)

set(PATH_CODE "${CMAKE_SOURCE_DIR}/code" CACHE INTERNAL "")

add_subdirectory(${PATH_CODE})
