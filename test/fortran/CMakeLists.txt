set(Test_test_utils_SOURCES
        test_utils_test.f90
)
list(TRANSFORM Test_test_utils_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(Test_test_utils_LIBRARY_DEPENDENCIES
        test_utils
)
add_fortran_ctest(test_utils
        "${Test_test_utils_SOURCES}"
        "${Test_test_utils_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_test_utils)

set(Test_f_c_string_trim_SOURCES
        f_c_string_trim_test.f90
)
list(TRANSFORM Test_f_c_string_trim_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(Test_f_c_string_trim_LIBRARY_DEPENDENCIES
        test_utils
        v2
)
add_fortran_ctest(f_c_string_trim
        "${Test_f_c_string_trim_SOURCES}"
        "${Test_f_c_string_trim_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_f_c_string_trim)

