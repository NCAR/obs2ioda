#include <gtest/gtest.h>
#include <netcdf_dimension.h>
#include <__filesystem/operations.h>

#include "netcdf_file.h"
#include "netcdf_group.h"
#include "netcdf_variable.h"


#include <netcdf_dimension.h>
#include <gtest/gtest.h>
#include <__filesystem/operations.h>

#include "netcdf_file.h"
#include "netcdf_group.h"
#include "netcdf_variable.h"

class NetcdfV1GroupTest : public ::testing::Test {
protected:
    std::string testFilePath = "test_group.nc";
    std::vector<const char *> v1ChannelVariableNames = {
        "brightness_temperature_1@ObsValue",
        "brightness_temperature_2@ObsValue",
        "brightness_temperature_3@ObsValue"
    };
    std::string v1StandardVariableName = "solar_zenith_angle@MetaData";
    std::string v3StandardVariableName = "solarZenithAngle";
    std::string v3ChannelVariableName = "brightnessTemperature";
    std::string v1LocationDimName = "nlocs";
    std::string v2ChannelDimName = "nchans";
    std::string ObsValueGroupName = "ObsValue";
    std::string MetaDataGroupName = "MetaData";
    int locationDimSize = 10;
    int channelDimSize = 3;

    int netcdfID{};
    int locationDimID{};
    int channelDimID{};
    //Allocate channelVariableData upon construction

    std::vector<double> standardVariableData = std::vector<double>(
        locationDimSize, 0);
    std::vector<std::vector<double> > channelVariableData =
    {
        std::vector<double>(10, 0),
        std::vector<double>(10, 0),
        std::vector<double>(10, 0)
    };
    std::shared_ptr<netCDF::NcFile> file;

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

    void TearDown() override {
        ASSERT_EQ(Obs2Ioda::netcdfClose(netcdfID), 0);
        // std::error_code ec;
        // std::filesystem::remove(testFilePath, ec);
        // ASSERT_FALSE(ec) << "Failed to delete file: " << ec.message();
    }
};

TEST_F(NetcdfV1GroupTest, GroupObsValueExists) {
    const auto group = file->getGroup(ObsValueGroupName);
    ASSERT_FALSE(group.isNull());
}

TEST_F(NetcdfV1GroupTest, BrightnessTemperatureVariableExists) {
    const auto variable = file->getGroup(ObsValueGroupName).getVar(
        v3ChannelVariableName);
    ASSERT_FALSE(variable.isNull());
}

TEST_F(NetcdfV1GroupTest, BrightnessTemperatureSecondDimIsChannel) {
    const auto variable = file->getGroup(ObsValueGroupName).getVar(
        v3ChannelVariableName);
    ASSERT_EQ(variable.getDim(1).getName(), "Channel");
}

TEST_F(NetcdfV1GroupTest, WriteDataToChannelVariable) {
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

TEST_F(NetcdfV1GroupTest, WriteDataToStandardVariable) {
    const auto variable = file->getGroup(MetaDataGroupName).getVar(
        v3StandardVariableName);
    std::vector<double> data(locationDimSize);
    variable.getVar(data.data());
    for (size_t i = 0; i < locationDimSize; i++) {
        ASSERT_EQ(data.at(i), standardVariableData.at(i));
    }
}

int main(int argc,
         char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
