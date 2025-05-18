#include <gtest/gtest.h>
#include <memory>
#include <ncException.h>

#include "ioda_obs_schema_map.h"
#include "ioda_obs_schema/ioda_obs_schema.h"
#include "FilePathConfig.h"


TEST(IodaObsSchemaMap, IodaObsSchemaMapAddFile) {
    int iodaObsSchemaID = 1;
    const auto iodaObsSchema = std::make_shared<IodaObsSchema>(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML)
        );
    Obs2Ioda::IodaObsSchemaMap::getInstance().addIodaObsSchema(
        iodaObsSchemaID,
        iodaObsSchema
    );
    EXPECT_NO_THROW(Obs2Ioda::IodaObsSchemaMap::getInstance().getIodaObsSchema(iodaObsSchemaID));
    EXPECT_NO_THROW(Obs2Ioda::IodaObsSchemaMap::getInstance().removeIodaObsSchema(iodaObsSchemaID));
    EXPECT_THROW(Obs2Ioda::IodaObsSchemaMap::getInstance().getIodaObsSchema(iodaObsSchemaID), netCDF::exceptions::NcBadId);
}

TEST(IodaObsSchemaMap, IodaObsSchemaMapThrow) {
    int iodaObsSchemaID = 1;
    EXPECT_THROW(Obs2Ioda::IodaObsSchemaMap::getInstance().getIodaObsSchema(iodaObsSchemaID), netCDF::exceptions::NcBadId);
    EXPECT_THROW(Obs2Ioda::IodaObsSchemaMap::getInstance().removeIodaObsSchema(iodaObsSchemaID), netCDF::exceptions::NcBadId);
}

int main(int argc,
         char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
