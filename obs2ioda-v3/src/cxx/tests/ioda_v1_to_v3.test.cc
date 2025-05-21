#include <gtest/gtest.h>
#include <netcdf_dimension.h>
#include <filesystem>
#include "netcdf_file.h"
#include "netcdf_group.h"
#include "netcdf_variable.h"

/**
 * @brief Unit test fixture for validating v1-to-v3 NetCDF IODA variable translation.
 *
 * This fixture sets up a synthetic NetCDF file with legacy v1-style variable names
 * and validates that they are correctly mapped and accessible via their v3 representations.
 */
class IodaV1ToV3Test : public ::testing::Test {
protected:
    std::string testFilePath = "test_group.nc";
    /**< Path to the temporary NetCDF test file. */

    std::vector<const char *> v1ChannelVariableNames = {
        "brightness_temperature_1@ObsValue",
        "brightness_temperature_2@ObsValue",
        "brightness_temperature_3@ObsValue"
    }; /**< Legacy v1-style channel variable names. */

    std::string v1StandardVariableName = "solar_zenith_angle@MetaData";
    /**< Legacy v1-style standard variable name. */
    std::string v3StandardVariableName = "solarZenithAngle";
    /**< v3-style standard variable name. */
    std::string v3ChannelVariableName = "brightnessTemperature";
    /**< v3-style channel variable name. */
    std::string v1LocationDimName = "nlocs";
    /**< Dimension name for number of locations. */
    std::string v2ChannelDimName = "nchans";
    /**< Dimension name for number of channels. */
    std::string ObsValueGroupName = "ObsValue";
    /**< Name of the group for observed values. */
    std::string MetaDataGroupName = "MetaData";
    /**< Name of the group for metadata. */
    int locationDimSize = 10; /**< Size of the location dimension. */
    int channelDimSize = 3; /**< Size of the channel dimension. */

    int netcdfID{}; /**< NetCDF file ID. */
    int locationDimID{};
    /**< ID of the location dimension in the file. */
    int channelDimID{}; /**< ID of the channel dimension in the file. */

    std::vector<double> standardVariableData = std::vector<double>(
        locationDimSize, 0);
    /**< Data to be written/read from the standard variable. */

    std::vector<std::vector<double> > channelVariableData = {
        std::vector<double>(10, 0),
        std::vector<double>(10, 0),
        std::vector<double>(10, 0)
    }; /**< Data to be written/read from the channel variable. */

    std::shared_ptr<netCDF::NcFile> file;
    /**< Shared pointer to the opened NetCDF file. */

    /**
     * @brief Sets up a test NetCDF file with v1-style variable names and writes sample data.
     */
    void SetUp() override {
        ASSERT_EQ(
            Obs2Ioda::netcdfCreate(testFilePath.c_str(), &netcdfID,
                netCDF::NcFile::replace), 0);
        file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        ASSERT_EQ(
            Obs2Ioda::netcdfAddDim(netcdfID, nullptr,
                v1LocationDimName.c_str(),
                locationDimSize, &
                locationDimID), 0);
        ASSERT_EQ(
            Obs2Ioda::netcdfAddDim(netcdfID, nullptr,
                v2ChannelDimName.c_str(), channelDimSize, &
                channelDimID), 0);

        std::vector dimNames = {v1LocationDimName.c_str()};
        ASSERT_EQ(
            Obs2Ioda::netcdfAddVar(netcdfID, nullptr,
                v1StandardVariableName.c_str(),
                NC_DOUBLE,
                dimNames.size(), dimNames.data()), 0);
        for (const auto varName: v1ChannelVariableNames) {
            ASSERT_EQ(
                Obs2Ioda::netcdfAddVar(netcdfID, nullptr, varName,
                    NC_DOUBLE,
                    dimNames.size(), dimNames.data()), 0);
        }
        for (size_t i = 0; i < locationDimSize; ++i) {
            channelVariableData.at(0).at(i) = i * 1.0;
            channelVariableData.at(1).at(i) = i * 2.0;
            channelVariableData.at(2).at(i) = i * 3.0;
            standardVariableData.at(i) = i * 4.0;
        }
        ASSERT_EQ(
            Obs2Ioda::netcdfPutVarDouble(netcdfID, nullptr,
                v1ChannelVariableNames.at(0), channelVariableData.at(0).
                data()
            ), 0);
        ASSERT_EQ(
            Obs2Ioda::netcdfPutVarDouble(netcdfID, nullptr,
                v1ChannelVariableNames.at(1), channelVariableData.at(1).
                data()
            ), 0);
        ASSERT_EQ(
            Obs2Ioda::netcdfPutVarDouble(netcdfID, nullptr,
                v1ChannelVariableNames.at(2), channelVariableData.at(2).
                data()
            ), 0);
        ASSERT_EQ(
            Obs2Ioda::netcdfPutVarDouble(netcdfID, nullptr,
                v1StandardVariableName.c_str(), standardVariableData.
                data()
            ), 0);
    }

    /**
     * @brief Closes the NetCDF file after the test and performs cleanup.
     */
    void TearDown() override {
        ASSERT_EQ(Obs2Ioda::netcdfClose(netcdfID), 0);
        std::error_code ec;
        std::filesystem::remove(testFilePath, ec);
        ASSERT_FALSE(ec) << "Failed to delete file: " << ec.message();
    }
};

/**
 * @test Validates that the ObsValue group exists in the output NetCDF file.
 */
TEST_F(IodaV1ToV3Test, GroupObsValueExists) {
    const auto group = file->getGroup(ObsValueGroupName);
    ASSERT_FALSE(group.isNull());
}

/**
 * @test Ensures that the brightnessTemperature variable was created correctly in the v3 format.
 */
TEST_F(IodaV1ToV3Test, BrightnessTemperatureVariableExists) {
    const auto variable = file->getGroup(ObsValueGroupName).getVar(
        v3ChannelVariableName);
    ASSERT_FALSE(variable.isNull());
}

/**
 * @test Checks that the second dimension of the brightnessTemperature variable is named "Channel".
 */
TEST_F(IodaV1ToV3Test, BrightnessTemperatureSecondDimIsChannel) {
    const auto variable = file->getGroup(ObsValueGroupName).getVar(
        v3ChannelVariableName);
    ASSERT_EQ(variable.getDim(1).getName(), "Channel");
}

/**
 * @test Validates that channel data is written correctly to the brightnessTemperature variable.
 */
TEST_F(IodaV1ToV3Test, WriteDataToChannelVariable) {
    const auto variable = file->getGroup(ObsValueGroupName).getVar(
        v3ChannelVariableName);
    std::vector<double> data(locationDimSize * channelDimSize);
    variable.getVar(data.data());
    int index = 0;
    for (size_t i = 0; i < data.size(); i += channelDimSize) {
        ASSERT_EQ(data.at(i), channelVariableData.at(0).at(index));
        ASSERT_EQ(data.at(i + 1), channelVariableData.at(1).at(index));
        ASSERT_EQ(data.at(i + 2), channelVariableData.at(2).at(index));
        index++;
    }
}

/**
 * @test Validates that standard (non-channel) variable data is written and retrieved correctly.
 */
TEST_F(IodaV1ToV3Test, WriteDataToStandardVariable) {
    const auto variable = file->getGroup(MetaDataGroupName).getVar(
        v3StandardVariableName);
    std::vector<double> data(locationDimSize);
    variable.getVar(data.data());
    for (size_t i = 0; i < locationDimSize; i++) {
        ASSERT_EQ(data.at(i), standardVariableData.at(i));
    }
}

/**
 * @brief Main entry point for running all GoogleTest unit tests in this suite.
 */
int main(int argc,
         char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
