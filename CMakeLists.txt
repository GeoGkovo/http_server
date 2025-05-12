cmake_minimum_required(VERSION 3.10)
project(RestfulApi)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system filesystem)
# Find OpenSSL
find_package(OpenSSL REQUIRED)

set(CMAKE_PREFIX_PATH "~/mylibs") 
find_package(tab_converter REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(RestfulApi src/main.cpp src/tab_handler.cpp)
target_include_directories(RestfulApi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})
target_link_libraries(RestfulApi ${Boost_LIBRARIES} tab_converter::tab_converter)
