include_directories(${PROJECT_SOURCE_DIR}/common/include)
aux_source_directory(./source common)
add_library(common ${common})
