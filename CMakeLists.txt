cmake_minimum_required(VERSION 2.8.11)
project(ttbinEditor C CXX)

# debug mode
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include(TestBigEndian)
test_big_endian(is_big_endian)
if(is_big_endian)
  add_definitions(-DTT_BIG_ENDIAN)
else()
  add_definitions(-DTT_LITTLE_ENDIAN)
endif(is_big_endian)

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(CURL)

include_directories(${CURL_INCLUDE_DIRS})

add_subdirectory(src)
