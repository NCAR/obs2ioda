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
    // NetCDF type constants
    static constexpr int NC_BYTE = 1;    // 1-byte signed integer
    static constexpr int NC_CHAR = 2;    // 1-byte character
    static constexpr int NC_SHORT = 3;   // 2-byte signed integer
    static constexpr int NC_INT = 4;     // 4-byte signed integer
    static constexpr int NC_LONG = 4;    // Alias for NC_INT
    static constexpr int NC_FLOAT = 5;   // 4-byte IEEE floating-point
    static constexpr int NC_DOUBLE = 6;  // 8-byte IEEE floating-point
    static constexpr int NC_UBYTE = 7;   // 1-byte unsigned integer
    static constexpr int NC_USHORT = 8;  // 2-byte unsigned integer
    static constexpr int NC_UINT = 9;    // 4-byte unsigned integer
    static constexpr int NC_INT64 = 10;  // 8-byte signed integer
    static constexpr int NC_UINT64 = 11; // 8-byte unsigned integer
    static constexpr int NC_STRING = 12; // Variable-length string

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
    int test_int_att_data = 0;

    void SetUp() override;
    void TearDown() override;
};

#endif //OBS2IODA_NETCDF_TEST_FIXTURE_H
