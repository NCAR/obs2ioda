#include <gtest/gtest.h>
#include <netcdf>
#include "ioda_obs_schema.h"
#include <memory>
#include "ioda_group.h"
#include <ranges>



class IodaGroupFixture : public ::testing::Test {
protected:
    std::vector<std::string> v1VariableNames = {
        "brightness_temperature_1@ObsValue",
        "brightness_temperature_1@ObsError",
        "brightness_temperature_1@PreQC",
    };
    std::vector<std::string> v2VariableNames = {
        "brightness_temperature_1",
        "brightness_temperature_1",
        "brightness_temperature_1",
    };
    std::vector<std::string> v3VariableNames = {
        "brightnessTemperature",
        "brightnessTemperature",
        "brightnessTemperature",
    };
    std::vector<std::string> groupNames = {
        "ObsValue",
        "ObsError",
        "PreQC"
    };
    int numV1Variables = v1VariableNames.size();
    int numV2Variables = v2VariableNames.size();
    int numV3Variables = v3VariableNames.size();
    int numGroups = groupNames.size();

    void SetUp() override {

    }

    void TearDown() override {
    }
};

TEST_F(IodaGroupFixture, GroupNameFromV1VariableName) {
    for (int i : std::views::iota(0, numV1Variables)) {
        IodaGroup iodaGroup(v1VariableNames[i]);
        EXPECT_EQ(iodaGroup.getName(), groupNames[i]);
    }
}

TEST_F(IodaGroupFixture, SchemaRegex) {
    IodaGroup iodaGroup("/");
    auto schema = iodaGroup.getSchema();
    for (int i : std::views::iota(0, numV1Variables)) {
        ASSERT_EQ(schema->getVariable(v1VariableNames[i])->getValidName(), v3VariableNames[i]);
        ASSERT_EQ(schema->getVariable(v3VariableNames[i])->getValidName(), v3VariableNames[i]);
    }
}


int main(int argc,
         char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
