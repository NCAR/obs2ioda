#include <gtest/gtest.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"
#include <memory>
#include "ioda_group/ioda_group.h"


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
    for (const auto &dim: netcdfFile->getDims()) {
        std::string dimName = schema.getDimension(dim.first)
                ->getValidName();
        int dimSize = dim.second.getSize();
        std::shared_ptr<IodaDimension> dimension = std::make_shared<
            IodaDimension>(dimName, dimSize);
        iodaFile.addDimension(dimension);
    }
    EXPECT_EQ(iodaFile.numDimensions(), referenceDimensionNames.size());
    for (const auto &dimName: referenceDimensionNames) {
        EXPECT_EQ(iodaFile.getDimension(dimName)->getName(),
                  schema.getDimension(dimName)->getValidName(
                  ));
    }
    for (const auto &var: netcdfFile->getVars()) {
        std::string groupName = var.first;
        std::shared_ptr<IodaGroup> group = std::make_shared<IodaGroup>(
            groupName);
        iodaFile.addGroup(group);
    }
    EXPECT_EQ(iodaFile.numGroups(), referenceGroupNames.size());
    for (const auto &groupName: referenceGroupNames) {
        EXPECT_EQ(iodaFile.getGroup(groupName)->getName(),
                  schema.getGroup(groupName)->getValidName(
                  ));
    }
}

TEST_F(IodaGroupFixture, AddVariable) {
    IodaGroup iodaFile("/");
    auto ObsValue = std::make_shared<IodaGroup>("ObsValue");
    auto nlocs = std::make_shared<IodaDimension>("nlocs", 80820);
    auto nchans = std::make_shared<IodaDimension>("nchans", 0);
    auto brightness_temperature_1 = std::make_shared<IodaVariable>(
        "brightness_temperature_1@ObsValue");
    iodaFile.addGroup(ObsValue);
    ObsValue->addDimension(nlocs);
    ObsValue->addDimension(nchans);
    brightness_temperature_1->addDimension(nlocs);
    brightness_temperature_1->addDimension(nchans);
    ObsValue->addVariable(brightness_temperature_1);
    EXPECT_EQ(ObsValue->getDimension("nchans")->getSize(), 1);
    EXPECT_EQ(brightness_temperature_1->getDimension("nchans")->getSize(), 1);



}

int main(int argc,
         char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
