#include <netcdf_dimension.h>
#include <gtest/gtest.h>
#include <__filesystem/operations.h>

#include "netcdf_file.h"
#include "netcdf_group.h"
#include "netcdf_variable.h"

class NetcdfV1GroupTest : public ::testing::Test {
protected:
    std::string testFilePath = "test_group.nc";
    std::vector<const char *> v1VariableNames = {
        "brightness_temperature_1@ObsValue",
        "brightness_temperature_2@ObsValue",
        "brightness_temperature_3@ObsValue"
    };

    int netcdfID{};
    int locationDimID{};
    int channelDimID{};
    std::shared_ptr<netCDF::NcFile> file;

    void SetUp() override {
        ASSERT_EQ(
            Obs2Ioda::netcdfCreate(testFilePath.c_str(), &netcdfID,
                netCDF::NcFile::replace), 0);
        file = Obs2Ioda::FileMap::getInstance().getFile(netcdfID);
        ASSERT_EQ(
            Obs2Ioda::netcdfAddDim(netcdfID, nullptr, "nlocs", 10, &
                locationDimID), 0);
        ASSERT_EQ(
            Obs2Ioda::netcdfAddDim(netcdfID, nullptr, "nchans", 3, &
                channelDimID), 0);

        std::vector dimNames = {"nlocs"};
        for (const auto varName: v1VariableNames) {
            ASSERT_EQ(
                Obs2Ioda::netcdfAddVar(netcdfID, nullptr, varName,
                    NC_DOUBLE,
                    dimNames.size(), dimNames.data()), 0);
        }
    }

    void TearDown() override {
        ASSERT_EQ(Obs2Ioda::netcdfClose(netcdfID), 0);
        // std::error_code ec;
        // std::filesystem::remove(testFilePath, ec);
        // ASSERT_FALSE(ec) << "Failed to delete file: " << ec.message();
    }
};

TEST_F(NetcdfV1GroupTest, GroupObsValueExists) {
    const auto group = file->getGroup("ObsValue");
    ASSERT_FALSE(group.isNull());
}

TEST_F(NetcdfV1GroupTest, BrightnessTemperatureVariableExists) {
    const auto variable = file->getGroup("ObsValue").getVar(
        "brightnessTemperature");
    ASSERT_FALSE(variable.isNull());
}

TEST_F(NetcdfV1GroupTest, BrightnessTemperatureSecondDimIsChannel) {
    const auto variable = file->getGroup("ObsValue").getVar(
        "brightnessTemperature");
    ASSERT_EQ(variable.getDim(1).getName(), "Channel");
}

TEST_F(NetcdfV1GroupTest, WriteData) {
    const auto variable = file->getGroup("ObsValue").getVar(
        "brightnessTemperature");
    auto var = file->getGroup("ObsValue").getVar(
        "brightnessTemperature");
    std::vector<size_t> start = {0, 0};
    std::vector<size_t> count = {10, 1};
    std::vector<double> column(10, 0.0);
    for (size_t i = 0; i < column.size(); ++i) {
        column[i] = static_cast<double>(i);
    }
    std::vector<double> reverseColumn(10, 0.0);
    for (size_t i = 0; i < column.size(); ++i) {
        reverseColumn[i] = static_cast<double>(column.size() - i);
    }
    Obs2Ioda::netcdfPutVarDouble(netcdfID, "ObsValue",
        v1VariableNames.at(0), column.data());
    Obs2Ioda::netcdfPutVarDouble(netcdfID, "ObsValue",
        v1VariableNames.at(1), reverseColumn.data());
    Obs2Ioda::netcdfPutVarDouble(netcdfID, "ObsValue",
        v1VariableNames.at(2), column.data());
//TODO: Formally check the data.

}
