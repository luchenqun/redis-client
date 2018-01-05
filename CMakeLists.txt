cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)

project(redis-client)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./test TEST_SRCS)
add_executable(${PROJECT_NAME} ${DIR_SRCS} ${TEST_SRCS})

INCLUDE_DIRECTORIES(redis)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hiredis pthread)





