cmake_minimum_required (VERSION 2.6)

project (rservices)

add_subdirectory(./common)

include_directories(./common/include)
add_executable(rservices ./application/main.cpp)

target_link_libraries(rservices common)

set(CMAKE_CXX_STANDARD_LIBRARIES "-lpthread -lrt")