# Define test executable sources
set(TEST_F_C_STRING_SOURCES
    f_c_string_t/Test_f_c_string_t_mod.f90
    f_c_string_t/Test_f_c_string_t_driver.f90

)
list(TRANSFORM TEST_F_C_STRING_SOURCES PREPEND ${CMAKE_SOURCE_DIR}/test/fortran/)

# Define the libraries to link
set(TEST_F_C_STRING_LIBRARY_DEPENDENCIES
    v2
    fortran_test_framework
)

add_fortran_ctest(f_c_string
                  "${TEST_F_C_STRING_SOURCES}"
                  "${TEST_F_C_STRING_LIBRARY_DEPENDENCIES}"
)
