cmake_minimum_required(VERSION 3.1)
project(librsfs)
include (GenerateExportHeader)

# Boost configuration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_SOURCE_DIR ./)
set(Boost_INCLUDE_DIR C:\\boost_1_58_0)
set(Boost_LIBRARY_DIR C:\\boost_1_58_0\\stage\\lib)

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

# C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")

# Our list of source files to include in this build
set(SOURCE_FILES src/main.cpp src/FileSystem.cpp src/FileSystem.h src/librsfs.h)

add_executable(librsfs ${SOURCE_FILES}) # Define executable for command-line usage
add_library(rsfs SHARED ${SOURCE_FILES}) # Define the shared library (.dll or .dylib or .a)

# Define how the exported library will come out
GENERATE_EXPORT_HEADER(rsfs
                       BASE_NAME librsfs # Reference name to avoid conflicts with the executable
                       EXPORT_MACRO_NAME RSFS_EXPORT # The exported compiler directive that's set if we build the library
                       EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/src/librsfs.h # Output .h file containing library defines
                       STATIC_DEFINE RSFS_BUILT_AS_STATIC)

# If it is built as static then define that as cflag.
set_target_properties(librsfs PROPERTIES
                      COMPILE_FLAGS -DRSFS_BUILT_AS_STATIC)

# Define dependencies of our build
target_link_libraries(librsfs ${Boost_LIBRARIES})
target_link_libraries(rsfs ${Boost_LIBRARIES})