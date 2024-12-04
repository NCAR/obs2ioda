#include "netcdf_test_fixture.h"
#include "netcdf_file.h"
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}