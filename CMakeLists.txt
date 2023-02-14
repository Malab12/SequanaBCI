cmake_minimum_required(VERSION 3.0.0)
project(Sequana VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Sequana src/main.c)
target_include_directories(Sequana PRIVATE lib/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
