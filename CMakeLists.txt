cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(impactArena C)
# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")  # Debug build

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(src)

set(SRC src/ball_physics.c src/string_functions.c src/main.c)
set(TEST_SRC tests/check_ball.c src/ball_physics.c src/string_functions.c)
set(HEADERS src/main.h)
set(TEST_LIBS check subunit)
set(TEST_TARGET testGame)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Check REQUIRED)

add_executable(impactArena ${SRC} ${HEADERS})
add_executable(${TEST_TARGET} ${TEST_SRC})

target_link_libraries(${TEST_TARGET} ${CHECK_LIBRARIES} ${SDL2_LIBRARIES} m ${TEST_LIBS})
target_link_libraries(impactArena ${SDL2_LIBRARIES} m)

set_target_properties(impactArena PROPERTIES RUNTIME_OUTPUT_DIRECTORY "./build")
set_target_properties(${TEST_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "./tests")

enable_testing()
add_test(NAME tests COMMAND ${TEST_TARGET})