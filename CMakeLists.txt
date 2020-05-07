cmake_minimum_required(VERSION 3.10)

#-----------------------------------------------------------------------------
# Options
#-----------------------------------------------------------------------------
option(BOOST_ASTRONOMY_BUILD_TEST "Build tests" ON)
option(BOOST_ASTRONOMY_USE_CLANG_TIDY "Set CMAKE_CXX_CLANG_TIDY property on targets to enable clang-tidy linting" OFF)
set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard version to use (default is 14)")

#-----------------------------------------------------------------------------
# Project
#-----------------------------------------------------------------------------
project(Boost.Astronomy
  LANGUAGES CXX
  DESCRIPTION "Boost.Astronomy - Astronomy Library for C++")

message(STATUS "Boost.Astronomy: Version ${PROJECT_VERSION}")
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_BINARY_DIR}/cmake)

#-----------------------------------------------------------------------------
# C++ language version and compilation flags
#-----------------------------------------------------------------------------
message(STATUS "Boost.Astronomy: Require C++${CMAKE_CXX_STANDARD}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(astronomy_compile_options INTERFACE)

# See https://cmake.org/pipermail/cmake/2018-December/068716.html
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REGEX REPLACE "-W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

# See https://svn.boost.org/trac10/wiki/Guidelines/WarningsGuidelines

target_compile_options(astronomy_compile_options
  INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:-W4>
    $<$<CXX_COMPILER_ID:MSVC>:-bigobj>
    $<$<CXX_COMPILER_ID:MSVC>:-FC> # Need absolute path for __FILE__ used in tests
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fstrict-aliasing -pedantic>)

# Do not mix warnings due to strict compilation level with linter warnings
if(NOT CMAKE_CXX_CLANG_TIDY)
  target_compile_options(astronomy_compile_options
    INTERFACE
      $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wconversion -Wextra -Wfloat-equal -Wshadow -Wsign-promo -Wstrict-aliasing -Wunused-parameter>)
endif()

target_compile_definitions(astronomy_compile_options
  INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_NONSTDC_NO_DEPRECATE>
    $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_DEPRECATE>
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>
    $<$<CXX_COMPILER_ID:MSVC>:BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE>)

#-----------------------------------------------------------------------------
# Dependency target
#-----------------------------------------------------------------------------
add_library(astronomy_dependencies INTERFACE)

#-----------------------------------------------------------------------------
# Dependency: Boost
# - look for stage Build
# - look for default installation location
# - look for location specified with BOOST_ROOT
#-----------------------------------------------------------------------------

if(NOT DEFINED BOOST_ROOT AND NOT DEFINED ENV{BOOST_ROOT})
  message(STATUS "Boost.Astronomy: Looking for Boost from current source tree and libraries from stage.")
  message(STATUS "Boost.Astronomy: Disable stage look-up with passing -DBOOST_ROOT=/path/to/your/boost.")
  get_filename_component(_boost_root ../../ ABSOLUTE)
  if(EXISTS ${_boost_root}/boost-build.jam)
    set(BOOST_ROOT ${_boost_root})
    message(STATUS "Boost.Astronomy: Using Boost libraries from stage directory in BOOST_ROOT=${Boost_ROOT}")
  endif()
endif()

set(Boost_DETAILED_FAILURE_MSG ON)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()

find_package(Boost 1.67.0 REQUIRED
  COMPONENTS
	date_time
    unit_test_framework)
message(STATUS "Boost.Astronomy: Using Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")
message(STATUS "Boost.Astronomy: Using Boost_LIBRARY_DIRS=${Boost_LIBRARY_DIRS}")

target_link_libraries(astronomy_dependencies
  INTERFACE
	Boost::date_time
    Boost::unit_test_framework)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  target_link_libraries(astronomy_dependencies INTERFACE Boost::disable_autolinking)
endif()

target_compile_definitions(astronomy_dependencies
  INTERFACE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:BOOST_TEST_DYN_LINK>)

#-----------------------------------------------------------------------------
# clang-tidy
# - default checks specified in .clang-tidy configuration file
#-----------------------------------------------------------------------------
if(BOOST_ASTRONOMY_USE_CLANG_TIDY AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.6)
  find_program(_clang_tidy
    NAMES clang-tidy-7 clang-tidy-6.0 clang-tidy-5.0 clang-tidy-4.0 clang-tidy
    DOC "Path to clang-tidy executable")

  if(_clang_tidy)
    message(STATUS "Boost.Astronomy: Configuring ${_clang_tidy} to run linting analysis for targets")
    set(CMAKE_CXX_CLANG_TIDY ${_clang_tidy})
  endif()
  unset(_clang_tidy)
endif()

#-----------------------------------------------------------------------------
# Common include directories
#
# The boostorg/astronomy repository includes must come first,
# before Boost includes from cloned Boost superproject or installed distribution.
# Otherwise IDEs may see the wrong file (ie. due to boost/ symlinks or
# Astronomy headers from installed Boost instead of this clone of boostog/astronomy1).
#-----------------------------------------------------------------------------
add_library(astronomy_include_directories INTERFACE)

target_include_directories(astronomy_include_directories
  BEFORE
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

#-----------------------------------------------------------------------------
# Tests
#-----------------------------------------------------------------------------
enable_testing()
if(BOOST_ASTRONOMY_BUILD_TEST)
	add_subdirectory(test)
endif()

#-----------------------------------------------------------------------------
# SOFA
#-----------------------------------------------------------------------------
set(sofa_library /Users/alihasan/Documents/GSOC/sofa/20190722/c/src)

add_library(sofa SHARED IMPORTED GLOBAL helper.hpp)
set_property(TARGET sofa PROPERTY IMPORTED_LOCATION ${sofa_library}/libsofa_c.a)
target_include_directories(sofa BEFORE INTERFACE ${sofa_library})

target_link_libraries(astronomy_dependencies INTERFACE sofa)