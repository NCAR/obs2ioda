# Define test executable sources
set(TEST_NETCDF_CREATE_SOURCES
    Test_netcdf_fixture_t_mod.f90
    Test_netcdf_create_mod.f90
    Test_netcdf_create.f90
)
list(TRANSFORM TEST_NETCDF_CREATE_SOURCES PREPEND "${CMAKE_SOURCE_DIR}/test/fortran/")

# Define the libraries to link
set(TEST_NETCDF_CREATE_LIBRARY_DEPENDENCIES
    v2
    fortran_test_framework
)

add_fortran_ctest(NetcdfCreate
                  "${TEST_NETCDF_CREATE_SOURCES}"
                  "${TEST_NETCDF_CREATE_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_NetcdfCreate)
