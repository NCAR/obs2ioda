
function(add_memcheck_ctest target)
    find_program(VALGRIND "valgrind")
    if (VALGRIND)
        message(STATUS "Valgrind found: ${VALGRIND}")
        message(STATUS "Adding memory check for test: ${target}")
        set(VALGRIND_COMMAND valgrind --leak-check=full --error-exitcode=1 --undef-value-errors=no)
        add_test(NAME ${target}_memcheck
                COMMAND ${VALGRIND_COMMAND} $<TARGET_FILE:${target}>)
    else ()
        message(STATUS "Valgrind not found")
        message(STATUS "Memory check for test: ${target} will not be added")
    endif ()
endfunction()

function(add_fortran_ctest test_name test_sources library_deps)
    add_executable("Test_${test_name}" ${test_sources})
    message(STATUS "Adding test: ${test_name} with sources: ${test_sources} and dependencies: ${library_deps}")
    target_link_libraries("Test_${test_name}" ${library_deps})
    add_test(NAME ${test_name}
            COMMAND ${CMAKE_BINARY_DIR}/bin/Test_${test_name})
endfunction()

function(add_cxx_ctest name sources include_dirs library_deps)
    add_executable(${name} ${sources})
    target_include_directories(${name} PUBLIC ${include_dirs})
    target_link_libraries(${name} PUBLIC ${library_deps})
    add_test(
            NAME ${name}
            COMMAND ${name} --gtest_filter=*
    )
endfunction()
