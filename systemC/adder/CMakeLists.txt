cmake_minimum_required(VERSION 3.10.0)

project(adder)

#include_directories(include)

file(GLOB SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

set(SYSTEMC_HOME "/emb22109/sysc-3.0.2")
target_include_directories(${PROJECT_NAME} PUBLIC ${SYSTEMC_HOME}/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${SYSTEMC_HOME}/lib-linux64)
target_link_libraries(${PROJECT_NAME} systemc)

file(GLOB VCD_FILES "*.vcd")
set_target_properties("${PROJECT_NAME}"
    PROPERTIES ADDITIONAL_CLEAN_FILES "${VCD_FILES}")
