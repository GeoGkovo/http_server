cmake_minimum_required(VERSION 3.10)
project(RestfulApi)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system filesystem)
set(CMAKE_PREFIX_PATH "~/mylibs") 
find_package(tab_converter REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(RestfulApi src/main.cpp)
target_link_libraries(RestfulApi ${Boost_LIBRARIES} tab_converter::tab_converter)