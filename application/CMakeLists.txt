include_directories(${PROJECT_SOURCE_DIR}/common/include)
include_directories(${PROJECT_SOURCE_DIR}/hal/include)
aux_source_directory(../common/source common)
aux_source_directory(../hal/source hal)

add_executable(rservices ./main.cpp ${hal} ${common})

set(CMAKE_CXX_STANDARD_LIBRARIES "-lpthread -lrt")