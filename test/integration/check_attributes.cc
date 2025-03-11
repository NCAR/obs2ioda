#include "utils.h"
#include <gtest/gtest.h>

// Check attribute names
void checkAllAttributeNames(const std::string &referencePath, const std::string &testPath) {
    netCDF::NcFile referenceFile = openNetCDFFile(referencePath);
    netCDF::NcFile testFile = openNetCDFFile(testPath);

    std::vector<std::string> referenceAttributes, testAttributes;
    for (const auto &group : referenceFile.getGroups()) {
        auto attrs = getAttributeNames(group.second);
        referenceAttributes.insert(referenceAttributes.end(), attrs.begin(), attrs.end());
    }
    for (const auto &group : testFile.getGroups()) {
        auto attrs = getAttributeNames(group.second);
        testAttributes.insert(testAttributes.end(), attrs.begin(), attrs.end());
    }

    expectEqualVector(referenceAttributes, testAttributes, "Attribute names in variables across all groups");
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

// Instantiate Parameterized Tests
INSTANTIATE_TEST_SUITE_P(NetCDFFileTests, NetCDFTest,
                         ::testing::ValuesIn(generateFilePairs()));
