set(Test_fortran_test_framework_SOURCES
    Test_fortran_test_framework_mod.f90
    Test_fortran_test_framework_driver.f90
)
list(TRANSFORM Test_fortran_test_framework_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/fortran_test_framework/")
set(Test_fortran_test_framework_LIBRARY_DEPENDENCIES v2)
add_fortran_ctest(fortran_test_framework
                  "${Test_fortran_test_framework_SOURCES}"
                  "${Test_fortran_test_framework_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_fortran_test_framework)

set(Test_f_c_string_SOURCES
    Test_f_c_string_i_mod.f90
    Test_f_c_string_t_mod.f90
    Test_f_c_string_t_1D_mod.f90
    Test_f_c_string_t_driver.f90
    Test_f_c_string.c
)
list(TRANSFORM Test_f_c_string_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/f_c_string/")
set(Test_f_c_string_LIBRARY_DEPENDENCIES v2)
add_fortran_ctest(f_c_string
                  "${Test_f_c_string_SOURCES}"
                  "${Test_f_c_string_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_f_c_string)


