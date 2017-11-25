cmake_minimum_required(VERSION 3.7)
project(tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")

include_directories(../include)
#link_directories(../lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES
        unit_tests.cpp
        ../include/apidisk.h
        ../include/t2fs.h
        ../include/util.h
        ../include/logic.h
        ../lib/apidisk.o
        ../src/t2fs.c
        ../src/logic.c
        ../include/logic_extra.h)

add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests gtest gtest_main)

set_target_properties(tests PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

add_subdirectory(gtest)
