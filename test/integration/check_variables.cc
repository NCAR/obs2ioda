#include <gtest/gtest.h>
#include <netcdf>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstring>

#ifndef TEST_DIR
#define TEST_DIR
#endif

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(NetCDFTest);

namespace fs = std::filesystem;

const std::string REFERENCE_DIR = std::string(TEST_DIR) + "/data/reference/original";
const std::string OUTPUT_DIR = std::string(TEST_DIR) + "/data/output";

const std::vector<std::string> IGNORE_VARIABLES = {
    "nstring", "_ioda_layout", "_ioda_layout_version", "sensorScanPosition"
};
const std::vector<std::string> IGNORE_ATTRIBUTES = {
    "nstring", "_ioda_layout", "_ioda_layout_version", "sensorScanPosition"
};

template<typename T>
void expectEqualVector(const std::vector<T> &reference,
                       const std::vector<T> &test,
                       const std::string &message) {
    ASSERT_EQ(reference.size(), test.size()) << "Size mismatch in " <<
 message;

    for (size_t i = 0; i < reference.size(); ++i) {
        EXPECT_EQ(reference[i], test[i]) << "Mismatch in " << message <<
 " at index " << i;
    }
}

void checkAllGroupVariableFillValues(const std::string &referencePath,
                                     const std::string &testPath) {
    netCDF::NcFile referenceFile(referencePath, netCDF::NcFile::read);
    netCDF::NcFile testFile(testPath, netCDF::NcFile::read);

    auto gatherVariableFillValues = [](const netCDF::NcGroup &group,
                                       const std::vector<std::string> &
                                       ignoreList) {
        std::map<std::string, std::pair<std::string, char *> >
                fillModeValues;
        for (const auto &var: group.getVars()) {
            if (std::find(ignoreList.begin(), ignoreList.end(),
                          var.second.getName()) == ignoreList.end()) {
                bool fillMode;
                const std::string intString = "int";
                const std::string floatString = "float";
                const std::string stringString = "string";
                const std::string varTypeNameStr = var.second.getType().
                        getName();
                std::stringstream keySS;
                keySS << var.second.getName() << "_" << group.getName();
                auto key = keySS.str();
                if (varTypeNameStr == intString) {
                    int fillValue;
                    var.second.getFillModeParameters(
                        fillMode, fillValue);
                    fillModeValues[key] =
                            std::make_pair(
                                varTypeNameStr, new char[sizeof(int)]);
                    std::memcpy(
                        fillModeValues[key].second,
                        &fillValue, sizeof(int));
                }
                if (varTypeNameStr == floatString) {
                    float fillValue;
                    var.second.getFillModeParameters(
                        fillMode, fillValue);
                    fillModeValues[key] =
                            std::make_pair(
                                varTypeNameStr,
                                new char[sizeof(float)]);
                    std::memcpy(
                        fillModeValues[key].second,
                        &fillValue, sizeof(float));
                }
            }
        }
        return fillModeValues;
    };

    // Gather variables from all groups in the reference file
    std::map<std::string, std::pair<std::string, char *> >
            referenceFillValues;
    for (const auto &group: referenceFile.getGroups()) {
        auto groupFillValues = gatherVariableFillValues(
            group.second, IGNORE_VARIABLES);
        referenceFillValues.insert(groupFillValues.begin(),
                                   groupFillValues.end());
    }

    std::map<std::string, std::pair<std::string, char *> >
            testFillValues;
    for (const auto &group: testFile.getGroups()) {
        auto groupFillValues = gatherVariableFillValues(
            group.second, IGNORE_VARIABLES);
        testFillValues.insert(groupFillValues.begin(),
                              groupFillValues.end());
    }
    auto numRefKeys = referenceFillValues.size();
    auto numTestKeys = testFillValues.size();
    ASSERT_EQ(numRefKeys, numTestKeys) << "Mismatch in number of "
            "variables with fill values";
    auto refIt = referenceFillValues.begin();
    auto testIt = testFillValues.begin();
    for (size_t i = 0; i < numRefKeys; ++i) {
        if (refIt->second.first == "float") {
            float refFillValue;
            std::memcpy(&refFillValue, refIt->second.second,
                        sizeof(float));
            float testFillValue;
            std::memcpy(&testFillValue, testIt->second.second,
                        sizeof(float));
            EXPECT_FLOAT_EQ(refFillValue,
                            testFillValue) << "Mismatch in fill "
                    "value for variable " << refIt->first;
            delete[] refIt->second.second;
            delete[] testIt->second.second;
        }
        if (refIt->second.first == "int") {
            int refFillValue;
            std::memcpy(&refFillValue, refIt->second.second,
                        sizeof(int));
            int testFillValue;
            std::memcpy(&testFillValue, testIt->second.second,
                        sizeof(int));
            EXPECT_EQ(refFillValue,
                      testFillValue) << "Mismatch in fill value "
                    "for variable " << refIt->first;
            delete[] refIt->second.second;
            delete[] testIt->second.second;
        }
        ++refIt;
        ++testIt;
    }
}

void checkAllGroupVariableNames(const std::string &referencePath,
                                const std::string &testPath) {
    netCDF::NcFile referenceFile(referencePath, netCDF::NcFile::read);
    netCDF::NcFile testFile(testPath, netCDF::NcFile::read);

    auto gatherVariables = [](const netCDF::NcGroup &group,
                              const std::vector<std::string> &
                              ignoreList) {
        std::vector<std::string> variableNames;
        for (const auto &var: group.getVars()) {
            if (std::find(ignoreList.begin(), ignoreList.end(),
                          var.second.getName()) == ignoreList.end()) {
                variableNames.push_back(var.second.getName());
            }
        }
        return variableNames;
    };

    // Gather variables from all groups in the reference file
    std::vector<std::string> referenceVariables;
    for (const auto &group: referenceFile.getGroups()) {
        auto groupVariables = gatherVariables(
            group.second, IGNORE_VARIABLES);
        referenceVariables.insert(referenceVariables.end(),
                                  groupVariables.begin(),
                                  groupVariables.end());
    }

    // Gather variables from all groups in the test file
    std::vector<std::string> testVariables;
    for (const auto &group: testFile.getGroups()) {
        auto groupVariables = gatherVariables(
            group.second, IGNORE_VARIABLES);
        testVariables.insert(testVariables.end(),
                             groupVariables.begin(),
                             groupVariables.end());
    }

    // Check that the gathered variable names match
    std::sort(referenceVariables.begin(), referenceVariables.end());
    std::sort(testVariables.begin(), testVariables.end());
    expectEqualVector(referenceVariables, testVariables,
                      "Variable names across all groups");
}

void checkAllVariableAttributeNames(const std::string &referencePath,
                                    const std::string &testPath) {
    netCDF::NcFile referenceFile(referencePath, netCDF::NcFile::read);
    netCDF::NcFile testFile(testPath, netCDF::NcFile::read);

    auto gatherAttributesFromVariables = [](
        const netCDF::NcGroup &group,
        const std::vector<std::string> &ignoreList) {
        std::vector<std::string> attributeNames;
        for (const auto &var: group.getVars()) {
            for (const auto &attr: var.second.getAtts()) {
                if (std::find(ignoreList.begin(), ignoreList.end(),
                              attr.second.getName()) == ignoreList.
                    end()) {
                    attributeNames.push_back(attr.second.getName());
                }
            }
        }
        return attributeNames;
    };

    // Gather attributes from all variables in all groups in the reference file
    std::vector<std::string> referenceAttributes;
    for (const auto &group: referenceFile.getGroups()) {
        auto groupAttributes = gatherAttributesFromVariables(
            group.second, IGNORE_ATTRIBUTES);
        referenceAttributes.insert(referenceAttributes.end(),
                                   groupAttributes.begin(),
                                   groupAttributes.end());
    }

    // Gather attributes from all variables in all groups in the test file
    std::vector<std::string> testAttributes;
    for (const auto &group: testFile.getGroups()) {
        auto groupAttributes = gatherAttributesFromVariables(
            group.second, IGNORE_ATTRIBUTES);
        testAttributes.insert(testAttributes.end(),
                              groupAttributes.begin(),
                              groupAttributes.end());
    }

    // Check that the gathered attribute names match
    std::sort(referenceAttributes.begin(), referenceAttributes.end());
    std::sort(testAttributes.begin(), testAttributes.end());
    expectEqualVector(referenceAttributes, testAttributes,
                      "Attribute names in variables across all groups");
}


// Parameterized Test Fixture
class NetCDFTest : public ::testing::TestWithParam<std::pair<std::string
            , std::string> > {
};

// Parameterized Test Case
TEST_P(NetCDFTest, AllGroupVariableNamesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkAllGroupVariableNames(referencePath, testPath);
}

// Parameterized Test Case
TEST_P(NetCDFTest, AllVariableAttributeNamesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkAllVariableAttributeNames(referencePath, testPath);
}

// Parameterized Test Case
TEST_P(NetCDFTest, AllGroupVariableFillValuesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkAllGroupVariableFillValues(referencePath, testPath);
}

// Helper Function to Generate Test Parameters
std::vector<std::pair<std::string, std::string> > generateFilePairs() {
    std::vector<std::pair<std::string, std::string> > filePairs;

    if (!fs::exists(REFERENCE_DIR) || !fs::exists(OUTPUT_DIR)) {
        throw std::runtime_error(
            "Reference or output directory does not exist");
    }

    for (const auto &entry: fs::directory_iterator(REFERENCE_DIR)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            ".h5") {
            if (entry.path().filename().string().find(
                    "gnssro_obs_2018041500.h5") != std::string::npos) {
                continue;
            }
            std::string referencePath = entry.path();
            std::string testPath =
                    OUTPUT_DIR + "/" + entry.path().filename().string();

            if (fs::exists(testPath)) {
                filePairs.emplace_back(referencePath, testPath);
            } else {
                std::cerr << "Warning: No matching output file for " <<
                        referencePath << std::endl;
            }
        }
    }

    return filePairs;
}

// Instantiate Parameterized Tests
INSTANTIATE_TEST_SUITE_P(NetCDFFileTests, NetCDFTest,
                         ::testing::ValuesIn(generateFilePairs()));
