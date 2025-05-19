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
    std::string nlocsDimensionName = "nlocs";
    int nlocsDimensionSize = 80820;
    IodaObsSchema schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));


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
        EXPECT_EQ(iodaVariable.getName(), stationIdentificationVariableName);
    }

    TEST_F(IodaVariableFixture, V1ChannelIodaVariable) {
        IodaVariable iodaVariable(brightness_temperature_1VariableName);
        EXPECT_EQ(iodaVariable.getName(), brightnessTemperatureVariableName);
    }

    TEST_F(IodaVariableFixture, AddDimension) {
        IodaVariable iodaVariable(brightness_temperature_1VariableName);
        iodaVariable.addDimension(
            std::make_shared<IodaDimension>(nlocsDimensionName, nlocsDimensionSize));
        ASSERT_EQ(iodaVariable.numDimensions(), 1);
        EXPECT_EQ(iodaVariable.getDimension(nlocsDimensionName)->getName(),
                  schema.getDimension(nlocsDimensionName)->getValidName());
        EXPECT_THROW(iodaVariable.addDimension(
            std::make_shared<IodaDimension>("nstring", 50)),
                     std::runtime_error);

    }

    TEST_F(IodaVariableFixture, ChannelVariable) {
        IodaVariable iodaChannelVariable(brightness_temperature_1VariableName);
        IodaVariable iodaStandardVariable(station_idVariableName);
        EXPECT_TRUE(iodaChannelVariable.isChannelVariable());
        EXPECT_FALSE(iodaStandardVariable.isChannelVariable());

    }


    int main(int argc,
             char **argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
