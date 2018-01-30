cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)

project(redis-client)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./test TEST_SRCS)

include_directories(redis)

link_directories(${PROJECT_SOURCE_DIR}/lib/linux)

add_executable(${PROJECT_NAME} ${DIR_SRCS} ${TEST_SRCS})
target_link_libraries(${PROJECT_NAME} hiredis pthread)

