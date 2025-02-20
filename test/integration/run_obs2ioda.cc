#include <gtest/gtest.h>
#include <netcdf>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace fs = std::filesystem;

#ifndef TEST_DIR
#define TEST_DIR
#endif

#ifndef OBS2IODA_EXECUTABLE
#define OBS2IODA_EXECUTABLE
#endif

const std::string REFERENCE_DIR =
        std::string(TEST_DIR) + "/data/reference";
const std::string OUTPUT_DIR = std::string(TEST_DIR) + "/data/output";

const std::string INPUT_DIR = std::string(TEST_DIR) + "/data/input";

const std::vector<std::string> FILES = {
    "gdas.1bamua.t00z.20180415.bufr", "gdas.satwnd.t00z.20180415.bufr",
    "gdas.gpsro.t00z.20180415.bufr", "prepbufr.gdas.20180415.t00z.nr.48h"
};

const std::vector<std::string> IGNORE_ATTRIBUTES = {
    "nstring", "_ioda_layout", "_ioda_layout_version"
};

const std::vector<std::string> IGNORE_VARIABLES = {
    "nstring", "_ioda_layout", "_ioda_layout_version"
};


// Parameterized Test Fixture
class NetCDFTest : public ::testing::TestWithParam<std::pair<std::string
            , std::string> > {
protected:
    // Runs before each test in this suite
    void SetUp() override {
        if (!fs::exists(OUTPUT_DIR)) {
            fs::create_directories(OUTPUT_DIR);
        }
        ASSERT_TRUE(
            fs::exists(OUTPUT_DIR )
        ) <<
 "Output directory does not exist. Setup might have failed.";
    }

    // Helper function to run the obs2ioda command
    static void runObs2IodaCommand(
        const std::string &file, const std::string &outputFile
    ) {
        if (!fs::exists(OUTPUT_DIR)) {
            fs::create_directories(OUTPUT_DIR);
        }

        // Process each file
        std::string command =
                std::string(OBS2IODA_EXECUTABLE) + " -i " + INPUT_DIR + " -o " +
                OUTPUT_DIR + " " + file;
        std::cout << "Processing file: " << file << std::endl;
        int result = std::system(command.c_str());
        if (result != 0) {
            throw std::runtime_error("Error processing file: " + file);
        }

        std::cout << "All files processed successfully." << std::endl;
    }

    // Runs once for the entire test suite
    static void SetUpTestSuite() {
        std::cout << "Setting up test suite..." << std::endl;
        std::cout << "Test suite setup complete." << std::endl;
    }
};

std::vector<std::pair<std::string, std::string> > getInputFiles() {
    std::vector<std::pair<std::string, std::string> > filePairs;
    filePairs.reserve(FILES.size());
for (const auto &file: FILES) {
        filePairs.emplace_back(file, OUTPUT_DIR + "/" + file);
    }
    return filePairs;
}

// Parameterized Test Cases
TEST_P(NetCDFTest, RunObs2IodaTest) {
    const auto &[inputFile, outputFile] = GetParam();
    runObs2IodaCommand(inputFile, outputFile);
}


// Instantiate Parameterized Tests
INSTANTIATE_TEST_SUITE_P(
    NetCDFFileTests, NetCDFTest, ::testing::ValuesIn(getInputFiles())
);
