#include <gtest/gtest.h>
#include <vector>
#include "netcdf_group.h"
#include "netcdf_file.h"
#include "netcdf_variable.h"
#include "netcdf_attribute.h"
#include "netcdf_dimension.h"
#include "netcdf_utils.h"


class NetcdfFunctionalTests : public ::testing::Test {
protected:
    const char *testCreatePath = "testfile.nc";
    const char *testGroupPath = "testgroupfile.nc";
    const char *testDimPath = "testdimfile.nc";
    const char *testVarPath = "testvarfile.nc";
    const char *testDataPath = "testdatafile.nc";

    void
    SetUp() override {
        // Prepare any required resources before each test
    }

    void
    TearDown() override {
        // Clean up resources after each test
    }
};

TEST_F(NetcdfFunctionalTests,
       TestNetcdfCreate) {
    int netcdfID = -1;

    int result = Obs2Ioda::netcdfCreate(
            this->testCreatePath,
            &netcdfID
    );
    EXPECT_EQ(result,
              0);
    EXPECT_GT(netcdfID,
              0); // Ensure netcdfID is positive


    // Cleanup
    Obs2Ioda::netcdfClose(netcdfID);
    std::remove(
            this->testCreatePath
    );
}

TEST_F(NetcdfFunctionalTests,
       TestAddGroup) {
    int netcdfID = -1;
    Obs2Ioda::netcdfCreate(
            this->testGroupPath,
            &netcdfID
    );

    const char *groupName = "TestGroup";
    int result = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            groupName
    );
    EXPECT_EQ(result,
              0);

    // Verify group creation
    auto file = Obs2Ioda::NETCDF_FILE_MAP[netcdfID];
    EXPECT_NO_THROW(file->getGroup(groupName));

    // Cleanup
    Obs2Ioda::netcdfClose(netcdfID);
    std::remove(this->testGroupPath);
}

TEST_F(NetcdfFunctionalTests,
       TestAddDim) {
    int netcdfID = -1;
    Obs2Ioda::netcdfCreate(
            this->testDimPath,
            &netcdfID
    );

    const char *dimName = "TestDim";
    int dimLength = 10;
    int result = Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            dimName,
            dimLength
    );
    EXPECT_EQ(result,
              0);

    // Verify dimension creation
    auto file = Obs2Ioda::NETCDF_FILE_MAP[netcdfID];
    EXPECT_NO_THROW(file->getDim(dimName));

    // Cleanup
    Obs2Ioda::netcdfClose(netcdfID);
    std::remove(this->testDimPath);
}

TEST_F(NetcdfFunctionalTests,
       TestAddVar) {
    int netcdfID = -1;
    Obs2Ioda::netcdfCreate(
            this->testVarPath,
            &netcdfID
    );

    const char *dimName = "TestDim";
    int dimLength = 10;
    Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            dimName,
            dimLength
    );

    const char *varName = "TestVar";
    const char *dimNames[] = {dimName};
    int result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            nullptr,
            varName,
            NC_INT,
            1,
            dimNames
    );
    EXPECT_EQ(result,
              0);

    // Verify variable creation
    auto file = Obs2Ioda::NETCDF_FILE_MAP[netcdfID];
    EXPECT_NO_THROW(file->getVar(varName));

    // Cleanup
    Obs2Ioda::netcdfClose(netcdfID);
    std::remove(this->testVarPath);
}

TEST_F(NetcdfFunctionalTests,
       TestPutVarAndGetVar) {
    int netcdfID = -1;
    Obs2Ioda::netcdfCreate(
            this->testDataPath,
            &netcdfID
    );

    const char *dimName = "TestDim";
    int dimLength = 10;
    Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            dimName,
            dimLength
    );

    const char *varName = "TestVar";
    const char *dimNames[] = {dimName};
    Obs2Ioda::netcdfAddVar(
            netcdfID,
            nullptr,
            varName,
            NC_INT,
            1,
            dimNames
    );

    int data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int result = Obs2Ioda::netcdfPutVar(
            netcdfID,
            nullptr,
            varName,
            data
    );
    EXPECT_EQ(result,
              0);

    // Retrieve data
    auto retrievedData = new int[10];
    result = Obs2Ioda::netcdfGetVar(
            netcdfID,
            nullptr,
            varName,
            &retrievedData
    );
    EXPECT_EQ(result,
              0);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(retrievedData[i],
                  data[i]);
    }

    // Cleanup
    Obs2Ioda::netcdfClose(netcdfID);
    std::remove(this->testDataPath);
    delete[] retrievedData;
}


int main(
        int argc,
        char **argv
) {
    ::testing::InitGoogleTest(
            &argc,
            argv
    );
    return RUN_ALL_TESTS();
}