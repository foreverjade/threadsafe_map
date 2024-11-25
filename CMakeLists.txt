cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED PROJ_NAME)
  set(PROJ_NAME "threadsafe_map")
endif()

project(${PROJ_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

# include header files
include_directories(/usr/local/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# automatically find all source files in current directory and subdirectories
file(GLOB_RECURSE DIR_SRCS *.cpp)
list(FILTER DIR_SRCS EXCLUDE REGEX "CMakeFiles/.*")

# create shared library
add_library(${PROJ_NAME} SHARED
    ${DIR_SRCS}
)

target_compile_options(${PROJ_NAME} PRIVATE -fPIC)