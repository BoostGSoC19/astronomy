file(GLOB_RECURSE  _hpp_coordinate RELATIVE
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy"
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy/coordinate/*.hpp")
list(APPEND _headers ${_hpp_coordinate})

file(GLOB_RECURSE  _hpp_detail RELATIVE
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy"
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy/detail/*.hpp")
list(APPEND _headers ${_hpp_detail})

file(GLOB_RECURSE  _hpp_exception RELATIVE
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy"
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy/exception/*.hpp")
list(APPEND _headers ${_hpp_exception})

file(GLOB_RECURSE  _hpp_io RELATIVE
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy"
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy/io/*.hpp")
list(APPEND _headers ${_hpp_io})

file(GLOB_RECURSE  _hpp_units RELATIVE
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy"
  "${CMAKE_SOURCE_DIR}/include/boost/astronomy/units/*.hpp")
list(APPEND _headers ${_hpp_units})

#-----------------------------------------------------------------------------
# Target: test_headers_self_contained
# Bundles all targets of self-contained header tests,
# functional equivalent to self-contained header tests defined in Jamfile.
#-----------------------------------------------------------------------------
message(STATUS "Boost.Astronomy: Configuring self-contained header tests for all headers")
add_custom_target(test_headers_self_contained)

file(READ ${CMAKE_CURRENT_LIST_DIR}/main.cpp _main_content)

foreach(_header ${_headers})
  string(REPLACE ".hpp" "" _target ${_header})
  string(REPLACE "/" "-" _target ${_target})
  set(_cpp ${CMAKE_BINARY_DIR}/test/headers/${_target}.cpp)
  set(_target test_header_${_target})

  string(REPLACE "BOOST_ASTRONOMY_TEST_HEADER" "${_header}" _content "${_main_content}")
  file(WRITE ${_cpp} "${_content}")
  unset(_content)

  add_executable(${_target})

  target_sources(${_target}
    PRIVATE
      ${_cpp}
      ${CMAKE_SOURCE_DIR}/include/boost/astronomy/${_header})
  unset(_cpp)

  target_link_libraries(${_target}
    PRIVATE
      astronomy_compile_options
      astronomy_include_directories
      astronomy_dependencies)

  add_dependencies(test_headers_self_contained ${_target})

  unset(_target)
endforeach()
