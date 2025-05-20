#include "FilePathConfig.h"
#include <netcdf>
#include "ioda_obs_schema.h"
#include <memory>
#include "ioda_variable.h"
#include <gtest/gtest.h>


class IodaVariableFixture : public ::testing::Test {
protected:
    std::string testFilePath =
            "/Users/astokely/projects/obs2ioda/build/test/validation/data/v1/amsua_n15_obs_2018041500.nc4";
    std::shared_ptr<netCDF::NcFile> netcdfFile;
    std::string station_id_VariableName = "station_id@MetaData";
    std::string stationIdentification_VariableName = "stationIdentification";
    std::string brightness_temperature_1_VariableName = "brightness_temperature_1@ObsValue";
    std::string brightnessTemperature_VariableName = "brightnessTemperature";
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
        IodaVariable iodaVariable(station_id_VariableName);
        EXPECT_EQ(iodaVariable.getName(), stationIdentification_VariableName);
    }

    TEST_F(IodaVariableFixture, V1ChannelIodaVariable) {
        IodaVariable iodaVariable(brightness_temperature_1_VariableName);
        EXPECT_EQ(iodaVariable.getName(), brightnessTemperature_VariableName);
    }


    TEST_F(IodaVariableFixture, ChannelVariable) {
        IodaVariable iodaChannelVariable(brightness_temperature_1_VariableName);
        IodaVariable iodaStandardVariable(station_id_VariableName);
        EXPECT_TRUE(iodaChannelVariable.isChannelVariable());
        EXPECT_FALSE(iodaStandardVariable.isChannelVariable());
        EXPECT_EQ(iodaChannelVariable.getNumChannels(), 0);
    }
    TEST_F(IodaVariableFixture, IsV1Variable) {
        IodaVariable iodaV1Variable(brightness_temperature_1_VariableName);
        IodaVariable iodaV3Variable(stationIdentification_VariableName);
        EXPECT_TRUE(iodaV1Variable.isV1Variable());
        EXPECT_FALSE(iodaV3Variable.isV1Variable());
    }

    TEST_F(IodaVariableFixture, GetChannelIndex) {
        IodaVariable iodaChannelVariable(brightness_temperature_1_VariableName);
        IodaVariable iodaStandardVariable(station_id_VariableName);
        EXPECT_EQ(iodaChannelVariable.getChannelIndex(
            brightness_temperature_1_VariableName), 0);
    }

    TEST_F(IodaVariableFixture, GetChannelIndexThrow) {
        IodaVariable iodaStandardVariable(station_id_VariableName);
        EXPECT_THROW(iodaStandardVariable.getChannelIndex(
            station_id_VariableName), std::runtime_error);
    }





    int main(int argc,
             char **argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
