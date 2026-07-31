cmake_minimum_required(VERSION 4.0)
project("tiles-clock"
 VERSION 1.0
 DESCRIPTION "A gui clock program that simulates a tiles clock"
 LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE SRC_FILES
 CONFIGURE_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(tiles-clock "${SRC_FILES}")
target_include_directories(tiles-clock PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/raylib-6.0_linux_amd64/include/")
target_link_directories(tiles-clock PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/raylib-6.0_linux_amd64/lib/")
target_link_libraries(tiles-clock "raylib")
target_link_libraries(tiles-clock "m")

message(STATUS "")
message(STATUS "_*_ Build Info _*_")
message(STATUS "Configuring project: ${PROJECT_NAME}")
if(NOT "${PROJECT_DESCRIPTION}" STREQUAL "")
  message(STATUS "Project Description: ${PROJECT_DESCRIPTION}")
endif()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
if (${CMAKE_C_STANDARD_REQUIRED})
  message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
endif()
