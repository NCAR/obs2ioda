#include <FilePathConfig.h>
#include <gtest/gtest.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"
#include <memory>
#include "ioda_variable/ioda_variable.h"


class IodaVariableFixture : public ::testing::Test {
protected:
    std::string testFilePath =
            "/Users/astokely/projects/obs2ioda/build/test/validation/data/v1/amsua_n15_obs_2018041500.nc4";
    std::shared_ptr<netCDF::NcFile> netcdfFile;
    std::string station_idVariableName = "station_id@MetaData";
    std::string stationIdentificationVariableName = "stationIdentification";
    std::string brightness_temperature_1VariableName = "brightness_temperature_1@ObsValue";
    std::string brightnessTemperatureVariableName = "brightnessTemperature";


    void SetUp() override {
        netcdfFile = std::make_shared<netCDF::NcFile>(
            testFilePath, netCDF::NcFile::read);
    }

    void TearDown() override {
        netcdfFile->close();
    }
};

    TEST_F(IodaVariableFixture, V1StandardIodaVariable) {
        IodaVariable iodaVariable(station_idVariableName);
        EXPECT_EQ(iodaVariable.m_name, stationIdentificationVariableName);
    }

    TEST_F(IodaVariableFixture, V1ChannelIodaVariable) {
        IodaVariable iodaVariable(brightness_temperature_1VariableName);
        EXPECT_EQ(iodaVariable.m_name, brightnessTemperatureVariableName);
    }

    int main(int argc,
             char **argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
