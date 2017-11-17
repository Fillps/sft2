cmake_minimum_required(VERSION 3.8)
project(cthread)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES
        include/apidisk.h
        include/t2fs.h
        src/t2fs.c
        lib/apidisk.o)


add_library(libt2fs.a ${SOURCE_FILES})

target_link_libraries(libt2fs.a)

set_target_properties(libt2fs.a PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

add_executable(t2fstst exemplo/t2fstst.c ${SOURCE_FILES})
set_target_properties(t2fstst PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

add_subdirectory(teste)