cmake_minimum_required(VERSION 3.24)

project(myprog C)

set(SOURCE_FILES main.c graph_io.c graph_algorithms.c graph.c)

add_executable(myprog ${SOURCE_FILES})

