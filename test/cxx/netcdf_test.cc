#include "netcdf_test_fixture.h"
#include "netcdf_file.h"
#include "netcdf_group.h"
#include "netcdf_dimension.h"
#include "netcdf_variable.h"
#include "netcdf_attribute.h"
#include <gtest/gtest.h>

// Example test case using the fixture
TEST_F(NetCDFTestFixture, NetCDFCreateTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_file_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

TEST_F(NetCDFTestFixture, NetCDFAddGroupTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_group_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            this->test_group_name.c_str()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_nested_group_name.c_str()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

TEST_F(NetCDFTestFixture, NetCDFAddDimTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_dim_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            this->test_group_name.c_str()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            this->test_global_dim_name.c_str(),
            this->test_global_dim_len
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddDim(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_dim_name.c_str(),
            this->test_dim_len
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

TEST_F(NetCDFTestFixture, NetCDFAddVarTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
            this->test_var_path.c_str(),
            &netcdfID
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            this->test_group_name.c_str()
    );

    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            this->test_global_dim_name.c_str(),
            this->test_global_dim_len
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddDim(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_dim_name.c_str(),
            this->test_dim_len
    );

    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_int_var_name.c_str(),
            NC_INT,
            1,
            std::vector<const char *>(
                    {this->test_global_dim_name.c_str()}
            ).data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_int64_var_name.c_str(),
            NC_INT64,
            1,
            std::vector<const char *>(
                    {this->test_global_dim_name.c_str()}
            ).data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_real_var_name.c_str(),
            NC_FLOAT,
            1,
            std::vector<const char *>(
                    {this->test_global_dim_name.c_str()}
            ).data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfAddVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_string_var_name.c_str(),
            NC_STRING,
            1,
            std::vector<const char *>(
                    {this->test_global_dim_name.c_str()}
            ).data()
    );
    EXPECT_EQ(status, 0);

    status = Obs2Ioda::netcdfPutVarInt(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_int_var_name.c_str(),
            this->test_int_var_data.data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfPutVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_int64_var_name.c_str(),
            this->test_int64_var_data.data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfPutVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_real_var_name.c_str(),
            this->test_real_var_data.data()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfPutVar(
            netcdfID,
            this->test_group_name.c_str(),
            this->test_string_var_name.c_str(),
            this->test_string_var_data.data()
    );
    EXPECT_EQ(status, 0);

    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}