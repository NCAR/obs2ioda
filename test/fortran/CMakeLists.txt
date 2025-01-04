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

set(Test_f_c_string_1D_SOURCES
    Test_f_c_string_i_mod.f90
    Test_f_c_string_t_1D_mod.f90
    Test_f_c_string.c
)
list(TRANSFORM Test_f_c_string_1D_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/f_c_string/")
set(Test_f_c_string_1D_LIBRARY_DEPENDENCIES v2)
add_library(Test_f_c_string_1D SHARED "${Test_f_c_string_1D_SOURCES}")
target_link_libraries(Test_f_c_string_1D PRIVATE "${Test_f_c_string_1D_LIBRARY_DEPENDENCIES}")

set(Test_f_c_string_1D_double_string_single_instance_SOURCES
    Test_f_c_string_t_1D_double_string_single_instance_driver.f90
)
list(TRANSFORM Test_f_c_string_1D_double_string_single_instance_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/f_c_string/")
set(Test_f_c_string_1D_double_string_single_instance_LIBRARY_DEPENDENCIES Test_f_c_string_1D)
add_fortran_ctest(f_c_string_1D_double_string_single_instance
                  "${Test_f_c_string_1D_double_string_single_instance_SOURCES}"
                  "${Test_f_c_string_1D_double_string_single_instance_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_f_c_string_1D_double_string_single_instance)

set(Test_f_c_string_1D_single_string_single_instance_SOURCES
    Test_f_c_string_t_1D_single_string_single_instance_driver.f90
)
list(TRANSFORM Test_f_c_string_1D_single_string_single_instance_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/f_c_string/")
set(Test_f_c_string_1D_single_string_single_instance_LIBRARY_DEPENDENCIES Test_f_c_string_1D)
add_fortran_ctest(f_c_string_1D_single_string_single_instance
                  "${Test_f_c_string_1D_single_string_single_instance_SOURCES}"
                  "${Test_f_c_string_1D_single_string_single_instance_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_f_c_string_1D_single_string_single_instance)

set(Test_f_c_string_1D_single_string_double_instance_SOURCES
    Test_f_c_string_t_1D_single_string_double_instance_driver.f90
)
list(TRANSFORM Test_f_c_string_1D_single_string_double_instance_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/f_c_string/")
set(Test_f_c_string_1D_single_string_double_instance_LIBRARY_DEPENDENCIES Test_f_c_string_1D)
add_fortran_ctest(f_c_string_1D_single_string_double_instance
                  "${Test_f_c_string_1D_single_string_double_instance_SOURCES}"
                  "${Test_f_c_string_1D_single_string_double_instance_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_f_c_string_1D_single_string_double_instance)

set(Test_f_c_string_1D_double_string_double_instance_mixed_SOURCES
    Test_f_c_string_t_1D_double_string_double_instance_mixed_driver.f90
)
list(TRANSFORM Test_f_c_string_1D_double_string_double_instance_mixed_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/f_c_string/")
set(Test_f_c_string_1D_double_string_double_instance_mixed_LIBRARY_DEPENDENCIES Test_f_c_string_1D)
add_fortran_ctest(f_c_string_1D_double_string_double_instance_mixed
                  "${Test_f_c_string_1D_double_string_double_instance_mixed_SOURCES}"
                  "${Test_f_c_string_1D_double_string_double_instance_mixed_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_f_c_string_1D_double_string_double_instance_mixed)

