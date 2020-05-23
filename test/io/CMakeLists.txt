foreach(_name
        t_hdu
        t_card
        t_primary_hdu
        t_ascii_table
        t_binary_table
       )
    set(_target test_fits_${_name})

    add_executable(${_target} "")
    target_compile_definitions(${_target} PRIVATE SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")
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


