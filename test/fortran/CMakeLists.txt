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

set(test_set_datetime_SOURCES
        set_datetime.test.f90
)
set(test_set_datetime_LIBRARY_DEPENDENCIES
        v3
)
add_executable(test_set_datetime ${test_set_datetime_SOURCES})
target_link_libraries(test_set_datetime
        PRIVATE
        ${test_set_datetime_LIBRARY_DEPENDENCIES}
)
add_test(NAME test_set_datetime
        COMMAND test_set_datetime
)
