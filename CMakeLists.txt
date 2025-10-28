cmake_minimum_required(VERSION 3.15)
project(Pococ)

set(CMAKE_CXX_STANDARD 17)

set(COMMON_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

set(COMPILE_SERVER_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/compileServer
)

set(OJ_SERVER_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/ojServer
)

set(SRC_FILE
    ${COMMON_INCLUDE}/losFile.cc
    ${COMMON_INCLUDE}/losPath.cc
    ${COMMON_INCLUDE}/losTime.cc
    ${COMPILE_SERVER_INCLUDE}/compiler.cc
    ${COMPILE_SERVER_INCLUDE}/compileAndRun.cc
)

add_executable(Pococ 
    main.cc
    ${SRC_FILE}
)

target_include_directories(Pococ PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${COMMON_INCLUDE}
    ${COMPILE_SERVER_INCLUDE}
    ${OJ_SERVER_INCLUDE}
)