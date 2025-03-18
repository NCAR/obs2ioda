#include <gtest/gtest.h>
#include "utils.h"
#include <netcdf>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstring>

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(NetCDFTest);


// Check variable names
void checkAllGroupVariableNames(
    const std::string &referencePath, const std::string &testPath
) {
    netCDF::NcFile referenceFile = openNetCDFFile(referencePath);
    netCDF::NcFile testFile = openNetCDFFile(testPath);

    std::vector<std::string> referenceVariables, testVariables;
    for (const auto &group: referenceFile.getGroups()) {
        auto vars = getVariableNames(group.second);
        referenceVariables.insert(
            referenceVariables.end(), vars.begin(), vars.end()
        );
    }
    for (const auto &group: testFile.getGroups()) {
        auto vars = getVariableNames(group.second);
        testVariables.insert(
            testVariables.end(), vars.begin(), vars.end()
        );
    }

    expectEqualVector(
        referenceVariables, testVariables,
        "Variable names across all groups"
    );
}

// Check variable types
void checkAllGroupVariableTypes(
    const std::string &referencePath, const std::string &testPath
) {
    netCDF::NcFile referenceFile = openNetCDFFile(referencePath);
    netCDF::NcFile testFile = openNetCDFFile(testPath);

    std::map<std::string, std::string> referenceTypes, testTypes;
    for (const auto &group: referenceFile.getGroups()) {
        auto vars = getVariableTypes(group.second);
        referenceTypes.insert(vars.begin(), vars.end());
    }
    for (const auto &group: testFile.getGroups()) {
        auto vars = getVariableTypes(group.second);
        testTypes.insert(vars.begin(), vars.end());
    }

    expectEqualVector(
        getKeys(referenceTypes), getKeys(testTypes),
        "Variable names across all groups"
    );

    for (auto &[varName, refType]: referenceTypes) {
        if (refType == "char") {
            refType = "string";
        }
        if (varName == "scan_position") {
            refType = "int";
        }
        EXPECT_EQ(refType, testTypes[varName]) << "Variable type for "
 << varName;
    }
}

// Function to check the number of dimensions for each variable
void checkAllGroupVariableDimensions(
    const std::string &referencePath, const std::string &testPath
) {
    netCDF::NcFile referenceFile = openNetCDFFile(referencePath);
    netCDF::NcFile testFile = openNetCDFFile(testPath);

    auto gatherVariableDimensions = [](const netCDF::NcGroup &group) {
        std::map<std::string, size_t> variableDimensions;
        for (const auto &var: group.getVars()) {
            if (std::find(
                    IGNORE_VARIABLES.begin(), IGNORE_VARIABLES.end(),
                    var.second.getName()
                ) == IGNORE_VARIABLES.end()) {
                variableDimensions[var.second.getName()] = var.second.
                        getDimCount();
            }
        }
        return variableDimensions;
    };

    std::map<std::string, size_t> referenceVariableDims,
            testVariableDims;

    // Gather variable dimensions for all groups
    for (const auto &group: referenceFile.getGroups()) {
        auto groupDims = gatherVariableDimensions(group.second);
        referenceVariableDims.insert(
            groupDims.begin(), groupDims.end()
        );
    }
    for (const auto &group: testFile.getGroups()) {
        auto groupDims = gatherVariableDimensions(group.second);
        testVariableDims.insert(groupDims.begin(), groupDims.end());
    }

    // Check that variable names match
    expectEqualVector(
        getKeys(referenceVariableDims), getKeys(testVariableDims),
        "Variable names across all groups"
    );

    // Check that the number of dimensions match
    for (const auto &[varName, refDims]: referenceVariableDims) {
        if (varName == "station_id" || varName == "variable_names") {
            ASSERT_EQ(
                testVariableDims[varName], 1
            ) << "Dimension count mismatch for variable: " << varName;
        } else {
            ASSERT_EQ(
                testVariableDims[varName], refDims
            ) << "Dimension count mismatch for variable: " << varName;
        }
    }
}

class NetCDFTest : public ::testing::TestWithParam<std::pair<std::string
            , std::string> > {
};

// Parameterized Test Case
TEST_P(NetCDFTest, AllGroupVariableNamesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkAllGroupVariableNames(referencePath, testPath);
}

TEST_P(NetCDFTest, AllGroupVariableTypesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkAllGroupVariableTypes(referencePath, testPath);
}

// Parameterized Test Case
TEST_P(NetCDFTest, AllGroupVariableDimensionsTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkAllGroupVariableDimensions(referencePath, testPath);
}


// Instantiate Parameterized Tests
INSTANTIATE_TEST_SUITE_P(
    NetCDFFileTests, NetCDFTest,
    ::testing::ValuesIn(generateFilePairs())
);
