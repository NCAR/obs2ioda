#include <gtest/gtest.h>
#include <netcdf>
#include <fstream>
#include <cstdio>

#include "netcdf_file.h"
#include "netcdf_group.h"
#include "netcdf_utils.h"

namespace {

    using namespace Obs2Ioda;

    class SetNetcdfGroupTest : public ::testing::Test {
    protected:
        std::string filePath;
        std::string groupName;
        int netcdfID;
        std::shared_ptr<netCDF::NcFile> file;
        std::shared_ptr<netCDF::NcGroup> refGroup;

        void SetUp() override {
            filePath = "test_set_netcdf_group.nc";
            groupName = "group";
            std::remove(filePath.c_str());

            // Create NetCDF file
            file = std::make_shared<netCDF::NcFile>(filePath,
                                                    netCDF::NcFile::replace);
            netcdfID = file->getId();

            FileMap::getInstance().addFile(netcdfID, file);
            refGroup = std::make_shared<netCDF::NcGroup>(file->addGroup(groupName));
        }

        void TearDown() override {
            try {
                FileMap::getInstance().removeFile(netcdfID);
            } catch (...) {}
            std::remove(filePath.c_str());
        }
    };

    TEST_F(SetNetcdfGroupTest, SetGroupForGlobalComponent) {
        auto group = Obs2Ioda::setNetcdfGroup(file, "");
        EXPECT_EQ(file, group);
    }

    TEST_F(SetNetcdfGroupTest, SetGroupForGroupLevelComponent) {
        auto group = Obs2Ioda::setNetcdfGroup(file, groupName.c_str());
        EXPECT_EQ(refGroup->getName(), group->getName());
    }

    TEST_F(SetNetcdfGroupTest, SetGroupThrowsForNullptrGroupName) {
        EXPECT_THROW(Obs2Ioda::setNetcdfGroup(file, nullptr), netCDF::exceptions::NcBadGroupId);
    }

    TEST_F(SetNetcdfGroupTest, SetGroupThrowsForNonExistentGroupName) {
        EXPECT_THROW(Obs2Ioda::setNetcdfGroup(file, "foo"), netCDF::exceptions::NcBadGroupId);
    }

}  // namespace
