cmake_minimum_required(VERSION 2.8.9)
project(fractal)

set (PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (PROJECT_SOURCE_DIR ${PROJECT_ROOT_DIR}/src)
set (PROJECT_INCLUDE_DIR ${PROJECT_ROOT_DIR}/include)
set (PROJECT_DEPS_DIR ${PROJECT_ROOT_DIR}/deps)
set (PROJECT_LIB_DIR ${PROJECT_ROOT_DIR}/lib)

find_package(X11 REQUIRED)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	set (PROJECT_LINK_LIBS libglfw3.a ${CMAKE_DL_LIBS} ${X11_LIBRARIES})
elseif  ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") 
    set (PROJECT_LINK_LIBS glfw3.lib)
endif () 

    
link_directories(${PROJECT_LIB_DIR})


include_directories(${PROJECT_INCLUDE_DIR})


set (SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_DEPS_DIR}/glad/glad.c
    )

add_executable(fractal ${SOURCES})
set(THREAD_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(fractal ${PROJECT_LINK_LIBS} Threads::Threads)
