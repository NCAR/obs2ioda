//
// Created by astokely on 11/29/24.
//

#ifndef OBS2IODA_NETCDF_TEST_FIXTURE_H
#define OBS2IODA_NETCDF_TEST_FIXTURE_H

#include <gtest/gtest.h>
#include <vector>
#include <string>

class NetCDFTestFixture : public ::testing::Test {
protected:
    // Paths and names
    std::string test_file_path;
    std::string test_group_path;
    std::string test_dim_path;
    std::string test_var_path;
    std::string test_att_path;

    std::string test_group_name;
    std::string test_nested_group_name;

    std::string test_dim_name;
    std::string test_global_dim_name;

    int test_dim_len;
    int test_global_dim_len;

    std::string test_int_var_name;
    std::string test_int64_var_name;
    std::string test_real_var_name;
    std::string test_string_var_name;

    std::vector<int> test_int_var_data;
    std::vector<long long> test_int64_var_data;
    std::vector<float> test_real_var_data;
    std::vector<const char*> test_string_var_data;

    std::string test_int_att_name;
    std::string test_string_att_name;
    std::string test_string_att_data;
    int test_int_att_data;

    void SetUp() override;
    void TearDown() override;
};

#endif //OBS2IODA_NETCDF_TEST_FIXTURE_H
