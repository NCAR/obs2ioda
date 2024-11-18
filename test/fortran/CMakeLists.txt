set(Test_fortran_test_framework_SOURCES
    Test_fortran_test_framework_mod.f90
    Test_fortran_test_framework_driver.f90
)
list(TRANSFORM Test_fortran_test_framework_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(Test_fortran_test_framework_LIBRARY_DEPENDENCIES
    v2
)
add_fortran_ctest(fortran_test_framework
                  "${Test_fortran_test_framework_SOURCES}"
                  "${Test_fortran_test_framework_LIBRARY_DEPENDENCIES}"
)