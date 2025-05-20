#include <gtest/gtest.h>
#include <netcdf>
#include "ioda_obs_schema.h"
#include <memory>
#include "ioda_group.h"


class IodaGroupFixture : public ::testing::Test {
protected:
    std::string testFilePath =
            "/Users/astokely/projects/obs2ioda/build/test/validation/data/v1/amsua_n15_obs_2018041500.nc4";
    std::shared_ptr<netCDF::NcFile> netcdfFile;
    std::vector<std::string> referenceGroupNames = {
        "MetaData", "VarMetaData", "ObsValue", "ObsError", "PreQC"
    };
    std::vector<std::string> referenceDimensionNames = {
        "nvars", "nlocs", "nstring", "ndatetime"
    };
    std::vector<std::string> brightness_temperature_VariableNames = {
        "brightness_temperature_1@ObsValue",
        "brightness_temperature_2@ObsValue",
        "brightness_temperature_3@ObsValue",
    };
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

TEST_F(IodaGroupFixture, IodaGroup) {
    IodaGroup iodaFile("/");
    EXPECT_EQ(iodaFile.getName(), "/");
    for (const auto &var: netcdfFile->getVars()) {
        std::string groupName = var.first;
        std::shared_ptr<IodaGroup> group = std::make_shared<IodaGroup>(
            groupName);
        iodaFile.addGroup(group);
    }
    EXPECT_EQ(iodaFile.getNumGroups(), referenceGroupNames.size());
    for (const auto &groupName: referenceGroupNames) {
        EXPECT_EQ(iodaFile.getGroup(groupName)->getName(),
                  schema.getGroup(groupName)->getValidName(
                  ));
    }
}

TEST_F(IodaGroupFixture, ChannelVariables) {
    IodaGroup root("/");
    for (const auto &name: brightness_temperature_VariableNames) {
        IodaVariable iodaVariable(name);
        root.addVariable(std::make_shared<IodaVariable>(name));
    }
    EXPECT_EQ(root.getNumVariables(), 1);
    EXPECT_EQ(
        root.getVariable(brightness_temperature_VariableNames[0])->
        getNumChannels(), brightness_temperature_VariableNames.size());
}

int main(int argc,
         char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
