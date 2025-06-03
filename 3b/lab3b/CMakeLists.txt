cmake_minimum_required(VERSION 3.24)

project(myprog C)

set(SOURCE_FILES main.c input.c hashStuff.c)

add_executable(myprog ${SOURCE_FILES})
target_link_libraries(myprog readline)
