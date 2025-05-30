set(test_write_ioda_v3_SOURCES
        write_ioda_v3.test.f90
)
set(test_write_ioda_v3_LIBRARY_DEPENDENCIES
        v3
)
add_executable(test_write_ioda_v3 ${test_write_ioda_v3_SOURCES})
target_link_libraries(test_write_ioda_v3
        PRIVATE
        ${test_write_ioda_v3_LIBRARY_DEPENDENCIES}
)

set(test_transpose_and_flatten_SOURCES
        transpose_and_flatten.test.f90
)
set(test_transpose_and_flatten_LIBRARY_DEPENDENCIES
        v3
)
add_executable(test_transpose_and_flatten ${test_transpose_and_flatten_SOURCES})
target_link_libraries(test_transpose_and_flatten
        PRIVATE
        ${test_transpose_and_flatten_LIBRARY_DEPENDENCIES}
)
add_test(NAME test_transpose_and_flatten
        COMMAND test_transpose_and_flatten
)