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

set(netcdf_file_test_SOURCES
        netcdf_file_test.f90
)
list(TRANSFORM netcdf_file_test_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(netcdf_file_test_LIBRARY_DEPENDENCIES
        v2
        test_utils
        v2_ext
)
add_fortran_ctest(netcdf_file_test
        "${netcdf_file_test_SOURCES}"
        "${netcdf_file_test_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_netcdf_file_test)

set(netcdf_variable_test_SOURCES
        netcdf_variable_test.f90
)
list(TRANSFORM netcdf_variable_test_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(netcdf_variable_test_LIBRARY_DEPENDENCIES
        v2 test_utils v2_ext)
add_fortran_ctest(netcdf_variable_test
        "${netcdf_variable_test_SOURCES}"
        "${netcdf_variable_test_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_netcdf_variable_test)
set(netcdf_attribute_test_SOURCES
        netcdf_attribute_test.f90
)
list(TRANSFORM netcdf_attribute_test_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(netcdf_attribute_test_LIBRARY_DEPENDENCIES
        v2 test_utils v2_ext)
add_fortran_ctest(netcdf_attribute_test
        "${netcdf_attribute_test_SOURCES}"
        "${netcdf_attribute_test_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(Test_netcdf_attribute_test)


