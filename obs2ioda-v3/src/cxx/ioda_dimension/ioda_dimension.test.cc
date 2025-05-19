#include <FilePathConfig.h>
#include <gtest/gtest.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"
#include <memory>
#include "ioda_dimension/ioda_dimension.h"


class IodaDimensionFixture : public ::testing::Test {
protected:
    std::string testFilePath =
            "/Users/astokely/projects/obs2ioda/build/test/validation/data/v1/amsua_n15_obs_2018041500.nc4";
    std::shared_ptr<netCDF::NcFile> netcdfFile;
    std::string referenceDimensionName = "nlocs";
    int referenceDimensionSize = 80820;
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

    TEST_F(IodaDimensionFixture, IodaDimension) {
        IodaDimension iodaDimension(referenceDimensionName, referenceDimensionSize);
        EXPECT_EQ(iodaDimension.getName(), schema.getDimension(referenceDimensionName)->getValidName());
    }

    TEST_F(IodaDimensionFixture, UpdateSize) {
        IodaDimension iodaDimension(referenceDimensionName, 0);
        iodaDimension.setSize(1);
        EXPECT_EQ(iodaDimension.getSize(), 1);
        iodaDimension.setSize(iodaDimension.getSize() + 1);
        EXPECT_EQ(iodaDimension.getSize(), 2);
    }

    int main(int argc,
             char **argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
