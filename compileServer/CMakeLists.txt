cmake_minimum_required(VERSION 3.15)
project(CompileServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



set(COMMON_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

set(SRC_FILE
    ${COMMON_INCLUDE}/losFile.cc
    ${COMMON_INCLUDE}/losPath.cc
    ${COMMON_INCLUDE}/losTime.cc
    compiler.cc
    runner.cc
    compileAndRun.cc
)

add_executable(CompileServer
    main.cc
    ${SRC_FILE}
)

target_include_directories(CompileServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${COMMON_INCLUDE}
)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(CompileServer PRIVATE JsonCpp::JsonCpp)

find_package(httplib CONFIG REQUIRED)
target_link_libraries(CompileServer PRIVATE httplib::httplib)
