cmake_minimum_required(VERSION 3.0)

project(capnp)

set(SOURCE src/main.cpp)
set(LIBS ${LIBS}  -lcapnp -lkj)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${LIBS}) 