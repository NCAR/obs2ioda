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
        EXPECT_EQ(iodaFile.m_name, "/");
        for (const auto &var : netcdfFile->getVars()) {
            std::string groupName = iodaFile.m_schema.getGroup(var.first)->getValidName();
            if (iodaFile.m_groups.count(groupName) == 0) {
                iodaFile.m_groups[groupName] = std::make_unique<IodaGroup>(groupName);
            }
        }
        EXPECT_EQ(iodaFile.m_groups.size(), referenceGroupNames.size());
        for (const auto &groupName : referenceGroupNames) {
            EXPECT_EQ(iodaFile.m_groups[groupName]->m_name, groupName);
        }
    }

    int main(int argc,
             char **argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
