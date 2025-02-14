#include <gtest/gtest.h>
#include <netcdf>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <iostream>
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


void checkAllAttributeNames(const std::string &referencePath,
                                    const std::string &testPath) {
    netCDF::NcFile referenceFile(referencePath, netCDF::NcFile::read);
    netCDF::NcFile testFile(testPath, netCDF::NcFile::read);

    auto gatherAttributesFromVariables = [](
        const netCDF::NcGroup &group,
        const std::vector<std::string> &ignoreList) {
        std::map<std::string, std::string> attributeNames;
        for (const auto &var: group.getVars()) {
            for (const auto &attr: var.second.getAtts()) {
                if (std::find(ignoreList.begin(), ignoreList.end(),
                              attr.second.getName()) == ignoreList.
                    end() && std::find(ignoreList.begin(), ignoreList.end(),
                                       var.second.getName()) == ignoreList.
                    end()) {
                    std::stringstream key;
                    key << group.getName() << "_" << var.second.getName();
                    attributeNames[key.str()] = attr.second.getName();
                }
            }
        }
        return attributeNames;
    };

    // Gather attributes from all variables in all groups in the reference file
    std::map<std::string, std::string> referenceAttributes;
    auto globalReferenceAttributes = gatherAttributesFromVariables(
        referenceFile, IGNORE_ATTRIBUTES);
    referenceAttributes.insert(globalReferenceAttributes.begin(),
                               globalReferenceAttributes.end());
    for (const auto &group: referenceFile.getGroups()) {
        auto groupAttributes = gatherAttributesFromVariables(
            group.second, IGNORE_ATTRIBUTES);
        referenceAttributes.insert(groupAttributes.begin(),
                                   groupAttributes.end());
    }

    // Gather attributes from all variables in all groups in the test file
    std::map<std::string, std::string> testAttributes;
    auto globalTestAttributes = gatherAttributesFromVariables(
        testFile, IGNORE_ATTRIBUTES);
    testAttributes.insert(globalTestAttributes.begin(),
                          globalTestAttributes.end());
    for (const auto &group: testFile.getGroups()) {
        auto groupAttributes = gatherAttributesFromVariables(
            group.second, IGNORE_ATTRIBUTES);
        testAttributes.insert(groupAttributes.begin(),
                              groupAttributes.end());
    }

    auto numReferenceAttributes = referenceAttributes.size();
    auto numTestAttributes = testAttributes.size();
    EXPECT_EQ(numReferenceAttributes, numTestAttributes) <<
            "Number of attributes mismatch " << referencePath << " vs " <<
                testPath << std::endl;
    auto referenceIt = referenceAttributes.begin();
    auto testIt = testAttributes.begin();
    for (size_t i = 0; i < numReferenceAttributes; ++i) {
        EXPECT_EQ(referenceIt->first, testIt->first) <<
                "Mismatch in attribute name at index " << i
        << " in " << referencePath << " vs " << testPath << std::endl;
        ++referenceIt;
        ++testIt;
    }
}


// Parameterized Test Fixture
class NetCDFTest : public ::testing::TestWithParam<std::pair<std::string
            , std::string> > {
};

// Parameterized Test Case
TEST_P(NetCDFTest, AllVariableAttributeNamesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkAllAttributeNames(referencePath, testPath);
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
