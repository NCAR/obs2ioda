#include <gtest/gtest.h>
#include "ioda_obs_schema.h"
#include "FilePathConfig.h"
#include <memory>

class IodaObsSchemaFixture : public ::testing::Test {
protected:
    std::unique_ptr<IodaObsSchema> iodaObsSchema;

    void SetUp() override {
        iodaObsSchema = std::make_unique<IodaObsSchema>(
            YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
        iodaObsSchema->addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
        iodaObsSchema->addVariableRegexPattern(
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
        iodaObsSchema->addGroupRegexPattern(R"(@([a-zA-Z0-9_]+))");
    }
};

TEST_F(IodaObsSchemaFixture, VariableGroupNameResolution_StationId) {
    std::string groupName = "MetaData";
    std::string v1VariableName = "station_id@MetaData";
    std::string v2VariableName = "station_id";
    std::string v3VariableName = "stationIdentification";

    EXPECT_EQ(
        iodaObsSchema->getVariable(v1VariableName)->getValidName(),
        v3VariableName);
    EXPECT_EQ(
        iodaObsSchema->getVariable(v2VariableName)->getValidName(),
        v3VariableName);
    EXPECT_EQ(
        iodaObsSchema->getVariable(v3VariableName)->getValidName(),
        v3VariableName);
    EXPECT_EQ(iodaObsSchema->getGroup(v1VariableName)->getValidName(),
              groupName);
    EXPECT_EQ(iodaObsSchema->getGroup(groupName)->getValidName(),
              groupName);
}

TEST_F(IodaObsSchemaFixture,
       VariableGroupNameResolution_BrightnessTemp) {
    std::string groupName = "ObsValue";
    std::string v1VariableName = "brightness_temperature_15@ObsValue";
    std::string v2VariableName = "brightness_temperature";
    std::string v3VariableName = "brightnessTemperature";

    EXPECT_EQ(
        iodaObsSchema->getVariable(v1VariableName)->getValidName(),
        v3VariableName);
    EXPECT_EQ(
        iodaObsSchema->getVariable(v2VariableName)->getValidName(),
        v3VariableName);
    EXPECT_EQ(
        iodaObsSchema->getVariable(v3VariableName)->getValidName(),
        v3VariableName);
    EXPECT_EQ(iodaObsSchema->getGroup(v1VariableName)->getValidName(),
              groupName);
    EXPECT_EQ(iodaObsSchema->getGroup(groupName)->getValidName(),
              groupName);
}

TEST_F(IodaObsSchemaFixture, CombinedVariableAndGroupResolution) {
    struct TestCase {
        std::string groupName;
        std::string v1VariableName;
        std::string v2VariableName;
        std::string v3VariableName;
    };

    std::vector<TestCase> testCases = {
        {
            "MetaData", "station_id@MetaData", "station_id",
            "stationIdentification"
        },
        {
            "ObsValue", "brightness_temperature_15@ObsValue",
            "brightness_temperature", "brightnessTemperature"
        }
    };

    for (const auto &tc: testCases) {
        EXPECT_EQ(
            iodaObsSchema->getVariable(tc.v1VariableName)->getValidName(
            ), tc.v3VariableName);
        EXPECT_EQ(
            iodaObsSchema->getVariable(tc.v2VariableName)->getValidName(
            ), tc.v3VariableName);
        EXPECT_EQ(
            iodaObsSchema->getVariable(tc.v3VariableName)->getValidName(
            ), tc.v3VariableName);
        EXPECT_EQ(
            iodaObsSchema->getGroup(tc.v1VariableName)->getValidName(),
            tc.groupName);
        EXPECT_EQ(iodaObsSchema->getGroup(tc.groupName)->getValidName(),
                  tc.groupName);
    }
}

int main(int argc,
         char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
