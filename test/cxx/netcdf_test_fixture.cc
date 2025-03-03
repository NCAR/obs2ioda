#include "netcdf_test_fixture.h"
#include <filesystem>

void NetCDFTestFixture::SetUp() {
    test_file_path = "test_file.nc";
    test_group_path = "test_group.nc";
    test_dim_path = "test_dim.nc";
    test_var_path = "test_var.nc";
    test_att_path = "test_att.nc";
    test_dim_path = "test_dim.nc";

    test_group_name = "group";
    test_nested_group_name = "nested_group";

    test_global_dim_name = "global_dim";
    test_dim_name = "dim";
    test_dim_len = 5;
    test_global_dim_len = 5;

    test_int_var_name = "int_var";
    test_int64_var_name = "int64_var";
    test_real_var_name = "real_var";
    test_string_var_name = "string_var";

    test_int_var_data = {1, 2, 3, 4, 5};
    test_int64_var_data = {11111, 22222, 33333, 44444, 55555};
    test_real_var_data = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    test_string_var_data = {"one", "two", "three", "four", "five"};

    test_int_att_name = "int_var";
    test_int_1d_att_name = "int_1d_att";
    test_string_att_name = "string_att";
    test_int_att_data = 2;
    test_int_1d_att_data = {1, 2, 3, 4, 5};
    test_string_att_data = "string";
}

void NetCDFTestFixture::TearDown() {
    // Remove the test file
    // std::filesystem::remove(this->test_file_path);
    // std::filesystem::remove(this->test_group_path);
    // std::filesystem::remove(this->test_dim_path);
    // std::filesystem::remove(this->test_var_path);
    // std::filesystem::remove(this->test_att_path);
}
