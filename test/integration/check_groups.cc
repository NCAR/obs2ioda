#include <gtest/gtest.h>
#include <netcdf>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <iostream>

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(NetCDFTest);

namespace fs = std::filesystem;

const std::string REFERENCE_DIR = "data/reference/original";
const std::string OUTPUT_DIR = "data/output";

const std::string INPUT_DIR = "data/input";

const std::vector<std::string> FILES = {
    "gdas.1bamua.t00z.20180415.bufr", "gdas.satwnd.t00z.20180415.bufr",
    "gdas.gpsro.t00z.20180415.bufr",
    "prepbufr.gdas.20180415.t00z.nr.48h"
};

const std::vector<std::string> IGNORE_ATTRIBUTES = {
    "nstring", "_ioda_layout", "_ioda_layout_version"
};

const std::vector<std::string> IGNORE_VARIABLES = {
    "nstring", "_ioda_layout", "_ioda_layout_version"
};

template<typename T> void expectEqualVector(
    const std::vector<T> &reference, const std::vector<T> &test,
    const std::string &message
) {
    ASSERT_EQ(reference.size(), test.size()) << "Size mismatch in " <<
 message;

    for (size_t i = 0; i < reference.size(); ++i) {
        EXPECT_EQ(reference[i], test[i]) << "Mismatch in " << message <<
 " at index " << i;
    }
}

void checkGroupNames(
    const std::string &referencePath, const std::string &testPath
) {
    netCDF::NcFile referenceFile(referencePath, netCDF::NcFile::read);
    netCDF::NcFile testFile(testPath, netCDF::NcFile::read);

    std::vector<std::string> referenceGroupNames, testGroupNames;

    for (const auto &group: referenceFile.getGroups()) {
        referenceGroupNames.push_back(group.second.getName());
    }
    for (const auto &group: testFile.getGroups()) {
        testGroupNames.push_back(group.second.getName());
    }

    expectEqualVector(
        referenceGroupNames, testGroupNames, "Group names"
    );
}

void checkGroupVariableNames(
    const std::string &referencePath, const std::string &testPath
) {
    netCDF::NcFile referenceFile(referencePath, netCDF::NcFile::read);
    netCDF::NcFile testFile(testPath, netCDF::NcFile::read);

    auto processGroupVariables = [](
        const netCDF::NcGroup &group,
        const std::vector<std::string> &ignoreList
    ) {
        std::vector<std::string> groupVariableNames;
        for (const auto &var: group.getVars()) {
            if (std::find(
                    ignoreList.begin(), ignoreList.end(),
                    var.second.getName()
                ) == ignoreList.end()) {
                groupVariableNames.push_back(var.second.getName());
            }
        }
        return groupVariableNames;
    };

    std::vector<std::vector<std::string> > referenceGroupVariables,
            testGroupVariables;

    for (const auto &group: referenceFile.getGroups()) {
        referenceGroupVariables.push_back(
            processGroupVariables(group.second, IGNORE_VARIABLES)
        );
    }
    for (const auto &group: testFile.getGroups()) {
        testGroupVariables.push_back(
            processGroupVariables(group.second, IGNORE_VARIABLES)
        );
    }

    for (size_t i = 0; i < referenceGroupVariables.size(); ++i) {
        expectEqualVector(
            referenceGroupVariables[i], testGroupVariables[i],
            "Variable names in group " + std::to_string(i)
        );
    }
}

void checkGroupAttributeNames(
    const std::string &referencePath, const std::string &testPath
) {
    netCDF::NcFile referenceFile(referencePath, netCDF::NcFile::read);
    netCDF::NcFile testFile(testPath, netCDF::NcFile::read);

    auto processGroupAttributes = [](
        const netCDF::NcGroup &group,
        const std::vector<std::string> &ignoreList
    ) {
        std::vector<std::string> groupAttributeNames;
        for (const auto &attr: group.getAtts()) {
            if (std::find(
                    ignoreList.begin(), ignoreList.end(),
                    attr.second.getName()
                ) == ignoreList.end()) {
                groupAttributeNames.push_back(attr.second.getName());
            }
        }
        return groupAttributeNames;
    };

    std::vector<std::vector<std::string> > referenceGroupAttributes,
            testGroupAttributes;

    for (const auto &group: referenceFile.getGroups()) {
        referenceGroupAttributes.push_back(
            processGroupAttributes(group.second, IGNORE_ATTRIBUTES)
        );
    }
    for (const auto &group: testFile.getGroups()) {
        testGroupAttributes.push_back(
            processGroupAttributes(group.second, IGNORE_ATTRIBUTES)
        );
    }

    referenceGroupAttributes.push_back(
        processGroupAttributes(referenceFile, IGNORE_ATTRIBUTES)
    );
    testGroupAttributes.push_back(
        processGroupAttributes(testFile, IGNORE_ATTRIBUTES)
    );

    for (size_t i = 0; i < referenceGroupAttributes.size(); ++i) {
        expectEqualVector(
            referenceGroupAttributes[i], testGroupAttributes[i],
            "Attribute names in group " + std::to_string(i)
        );
    }
}

// Parameterized Test Fixture
class NetCDFTest : public ::testing::TestWithParam<std::pair<std::string
            , std::string> > {
protected:
    // Runs before each test in this suite
    void SetUp() override {
        ASSERT_TRUE(
            fs::exists(OUTPUT_DIR)
        ) <<
 "Output directory does not exist. Setup might have failed.";
    }


    // Runs once for the entire test suite
    static void SetUpTestSuite() {
        std::cout << "Setting up test suite..." << std::endl;
        std::cout << "Test suite setup complete." << std::endl;
    }
};

// Parameterized Test Cases
TEST_P(NetCDFTest, GroupNamesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkGroupNames(referencePath, testPath);
}

TEST_P(NetCDFTest, GroupVariableNamesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkGroupVariableNames(referencePath, testPath);
}

TEST_P(NetCDFTest, GroupAttributeNamesTest) {
    const auto &[referencePath, testPath] = GetParam();
    checkGroupAttributeNames(referencePath, testPath);
}

// Helper Function to Generate Test Parameters
std::vector<std::pair<std::string, std::string> > generateFilePairs() {
    std::vector<std::pair<std::string, std::string> > filePairs;

    if (!fs::exists(REFERENCE_DIR) || !fs::exists(OUTPUT_DIR)) {
        throw std::runtime_error(
            "Reference or output directory does not exist"
        );
    }

    for (const auto &entry: fs::directory_iterator(REFERENCE_DIR)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            ".h5") {
            if (entry.path().filename().string().find(
                    "gnssro_obs_2018041500.h5"
                ) != std::string::npos) {
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
INSTANTIATE_TEST_SUITE_P(
    NetCDFFileTests, NetCDFTest,
    ::testing::ValuesIn(generateFilePairs())
);
