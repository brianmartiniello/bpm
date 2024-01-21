cmake_minimum_required(VERSION 3.16)

set(PATH_CORE "${PATH_CODE}/core" CACHE INTERNAL "")

find_package(GTest REQUIRED)

add_subdirectory(${PATH_CORE})
