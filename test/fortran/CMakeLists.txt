
add_library(netcdf_test_utils SHARED
        netcdf_test_utils_mod.f90
)
add_pfunit_ctest(test_transpose_and_flatten_mod
        TEST_SOURCES test_transpose_and_flatten_mod.pf
        LINK_LIBRARIES v3
)

add_pfunit_ctest(test_get_julian_time_mod
        TEST_SOURCES test_get_julian_time_mod.pf
        LINK_LIBRARIES v3
)
add_pfunit_ctest(test_to_lower_mod
        TEST_SOURCES test_to_lower_mod.pf
        LINK_LIBRARIES v3
)
add_pfunit_ctest(test_set_goes_abi_out_fname_mod
        TEST_SOURCES test_set_goes_abi_out_fname_mod.pf
        LINK_LIBRARIES v3
)
add_pfunit_ctest(test_compute_bt_superob_mod
        TEST_SOURCES test_compute_bt_superob_mod.pf
        LINK_LIBRARIES v3
)
add_pfunit_ctest(test_calc_solar_zenith_angle_mod
        TEST_SOURCES test_calc_solar_zenith_angle_mod.pf
        LINK_LIBRARIES v3
)

add_library(f_c_string_cpp_test SHARED
        f_c_string.test.cpp
)
set_target_properties(
        f_c_string_cpp_test
        PROPERTIES
        LINKER_LANGUAGE CXX
)
add_pfunit_ctest(test_f_c_string_t_mod
        TEST_SOURCES test_f_c_string_t_mod.pf
        LINK_LIBRARIES v3 f_c_string_cpp_test
)

add_pfunit_ctest(test_f_c_string_array_t_mod
        TEST_SOURCES test_f_c_string_array_t_mod.pf
        LINK_LIBRARIES v3 f_c_string_cpp_test
)
add_pfunit_ctest(test_netcdf_file_mod
        TEST_SOURCES test_netcdf_file_mod.pf
        LINK_LIBRARIES v3 netcdf_test_utils
)
add_pfunit_ctest(test_netcdf_group_mod
        TEST_SOURCES test_netcdf_group_mod.pf
        LINK_LIBRARIES v3 netcdf_test_utils
)
add_pfunit_ctest(test_netcdf_dimension_mod
        TEST_SOURCES test_netcdf_dimension_mod.pf
        LINK_LIBRARIES v3 netcdf_test_utils
)
add_pfunit_ctest(test_netcdf_variable_mod
        TEST_SOURCES test_netcdf_variable_mod.pf
        LINK_LIBRARIES v3 netcdf_test_utils
)
