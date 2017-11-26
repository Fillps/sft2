cmake_minimum_required(VERSION 3.8)
project(t2fs)

set(CMAKE_CXX_STANDARD 98)

include_directories(include)

set(SOURCE_FILES
        include/apidisk.h
        include/t2fs.h
        include/logic.h
        include/util.h
        src/t2fs.c
        src/logic.c
        lib/apidisk.o
        include/data.h src/data.c)


add_library(libt2fs.a ${SOURCE_FILES})

target_link_libraries(libt2fs.a)

set_target_properties(libt2fs.a PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

add_executable(t2fstst exemplo/t2fstst.c ${SOURCE_FILES})
set_target_properties(t2fstst PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

add_subdirectory(teste)