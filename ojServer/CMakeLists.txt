cmake_minimum_required(VERSION 3.15)
project(OjServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(OjServer
    main.cc
    ojController.cc
    ojModel.cc
    ojView.cc
    common/losFile.cc
    common/losPath.cc
    common/losTime.cc
    common/losStr.cc
)

target_include_directories(OjServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${MySQL_INCLUDE_DIRS}
)

find_package(Boost REQUIRED)
find_package(inja CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(unofficial-libmysql REQUIRED)
target_link_libraries(OjServer PRIVATE
    Boost::boost
    pantor::inja
    nlohmann_json::nlohmann_json
    JsonCpp::JsonCpp
    # unofficial::mysql-connector-cpp::connector
    unofficial::libmysql::libmysql
    ${MySQL_LIBRARIES}
)
