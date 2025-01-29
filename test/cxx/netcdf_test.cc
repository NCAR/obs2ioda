#include <netcdf_attribute.h>
#include <netcdf_utils.h>

#include "netcdf_test_fixture.h"
#include "netcdf_file.h"
#include "netcdf_group.h"
#include "netcdf_dimension.h"
#include "netcdf_variable.h"
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

TEST_F(NetCDFTestFixture, NetCDFAddAttTest) {
    int netcdfID{};
    int status = Obs2Ioda::netcdfCreate(
        this->test_att_path.c_str(),
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
    std::vector<int> intAttData = {1, 2};
    status = Obs2Ioda::netcdfPutAtt1D<int>(
        netcdfID, this->test_group_name.c_str(),
        this->test_int_var_name.c_str(),
        this->test_int_att_name.c_str(),
        NC_INT,
        intAttData.data(),
        2
        );
    EXPECT_EQ(status, 0);

    status = Obs2Ioda::netcdfClose(netcdfID);
    EXPECT_EQ(status, 0);
}

TEST(NetcdfObsTest, NetCDFObsGlobalDimVar) {
    int netcdfID{};
    std::string v2LocationDimName = "nlocs";
    std::string v2LocationVarName = "nlocs";
    std::string v3LocationDimName = "Location";
    std::string v3LocationVarName = "Location";
    Obs2Ioda::netcdfCreate("test_obs_global_dim_var.nc", &netcdfID);
    Obs2Ioda::netcdfAddDim(netcdfID, nullptr, v2LocationDimName.c_str(),
                           5);
    Obs2Ioda::netcdfAddVar(netcdfID, nullptr, v2LocationVarName.c_str(),
                           NC_INT, 1, std::vector<const char *>({
                               v2LocationDimName.c_str()
                           }).data());
    auto locationDim = Obs2Ioda::NETCDF_FILE_MAP[netcdfID]->getDim(
        v3LocationDimName);
    auto locationVar = Obs2Ioda::NETCDF_FILE_MAP[netcdfID]->getVar(
        v3LocationVarName);
    auto locationDimName = locationDim.getName();
    auto locationVarName = locationVar.getName();
    EXPECT_EQ(locationDimName, v3LocationDimName);
    EXPECT_EQ(locationVarName, v3LocationVarName);
    EXPECT_EQ(locationVar.getDims()[0].getName(), "Location");
    Obs2Ioda::netcdfClose(netcdfID);
}

int main(
    int argc,
    char **argv
) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
