foreach(_name
        cartesian_representation
        cartesian_differential
        spherical_representation
        spherical_differential
        spherical_equatorial_representation
        spherical_equatorial_differential)
    set(_target test_coordinate_${_name})

    add_executable(${_target} "")
    target_sources(${_target} PRIVATE ${_name}.cpp)
    target_link_libraries(${_target}
            PRIVATE
            astronomy_compile_options
            astronomy_include_directories
            astronomy_dependencies)
    add_test(NAME test.astro.${_name} COMMAND ${_target})

    unset(_name)
    unset(_target)
endforeach()
