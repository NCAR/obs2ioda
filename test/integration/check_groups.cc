#include <gtest/gtest.h>
#include <netcdf>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "utils.h"

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(NetCDFTest);

namespace fs = std::filesystem;

const std::vector<std::string> FILES = {
    "gdas.1bamua.t00z.20180415.bufr", "gdas.satwnd.t00z.20180415.bufr",
    "gdas.gpsro.t00z.20180415.bufr",
    "prepbufr.gdas.20180415.t00z.nr.48h"
};

// Check group names
void checkGroupNames(const std::string &referencePath, const std::string &testPath) {
    netCDF::NcFile referenceFile = openNetCDFFile(referencePath);
    netCDF::NcFile testFile = openNetCDFFile(testPath);

    std::vector<std::string> referenceGroups, testGroups;
    for (const auto &group : referenceFile.getGroups()) {
        referenceGroups.push_back(group.second.getName());
    }
    for (const auto &group : testFile.getGroups()) {
        testGroups.push_back(group.second.getName());
    }

    expectEqualVector(referenceGroups, testGroups, "Group names");
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

// Instantiate Parameterized Tests
INSTANTIATE_TEST_SUITE_P(
    NetCDFFileTests, NetCDFTest,
    ::testing::ValuesIn(generateFilePairs())
);
