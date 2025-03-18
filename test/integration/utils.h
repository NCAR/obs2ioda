#ifndef NETCDF_UTILS_H
#define NETCDF_UTILS_H

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstring>

#include <netcdf>
#include <map>

#ifndef TEST_DIR
#define TEST_DIR
#endif

// Ignore lists
const std::vector<std::string> IGNORE_ATTRIBUTES = {
    "nstring", "_ioda_layout", "_ioda_layout_version"
};

const std::vector<std::string> IGNORE_VARIABLES = {
    "nstring", "_ioda_layout", "_ioda_layout_version"
};


namespace fs = std::filesystem;

const std::string REFERENCE_DIR = std::string(TEST_DIR) + "/data/reference/v3";
const std::string OUTPUT_DIR = std::string(TEST_DIR) + "/data/output";


// Opens a NetCDF file
inline netCDF::NcFile openNetCDFFile(const std::string &path) {
    return netCDF::NcFile(path, netCDF::NcFile::read);
}

// Extracts variable names from a NetCDF group
inline std::vector<std::string> getVariableNames(const netCDF::NcGroup &group) {
    std::vector<std::string> names;
    for (const auto &var : group.getVars()) {
        if (std::find(IGNORE_VARIABLES.begin(), IGNORE_VARIABLES.end(), var.second.getName()) == IGNORE_VARIABLES.end()) {
            names.push_back(var.second.getName());
        }
    }
    return names;
}

// Extracts attribute names from a NetCDF group
inline std::vector<std::string> getAttributeNames(const netCDF::NcGroup &group) {
    std::vector<std::string> names;
    for (const auto &attr : group.getAtts()) {
        if (std::find(IGNORE_ATTRIBUTES.begin(), IGNORE_ATTRIBUTES.end(), attr.second.getName()) == IGNORE_ATTRIBUTES.end()) {
            names.push_back(attr.second.getName());
        }
    }
    return names;
}

// Extracts variable types
inline std::map<std::string, std::string> getVariableTypes(const netCDF::NcGroup &group) {
    std::map<std::string, std::string> types;
    for (const auto &var : group.getVars()) {
        if (std::find(IGNORE_VARIABLES.begin(), IGNORE_VARIABLES.end(), var.second.getName()) == IGNORE_VARIABLES.end()) {
            types[var.second.getName()] = var.second.getType().getName();
        }
    }
    return types;
}

// Vector comparison helper
template<typename T>
inline void expectEqualVector(const std::vector<T> &reference, const std::vector<T> &test, const std::string &message) {
    ASSERT_EQ(reference.size(), test.size()) << "Size mismatch in " << message;
    for (size_t i = 0; i < reference.size(); ++i) {
        EXPECT_EQ(reference[i], test[i]) << "Mismatch in " << message << " at index " << i;
    }
}

// Map key extraction helper
template<typename K, typename V>
inline std::vector<K> getKeys(const std::map<K, V> &map) {
    std::vector<K> keys;
    for (const auto &[key, _] : map) {
        keys.push_back(key);
    }
    return keys;
}

// Generates reference/test file pairs
inline std::vector<std::pair<std::string, std::string>> generateFilePairs() {
    std::vector<std::pair<std::string, std::string>> filePairs;

    auto referencePath = fs::path(REFERENCE_DIR);
    auto outputPath = fs::path(OUTPUT_DIR);
    if (!fs::exists(referencePath) || !fs::exists(outputPath)) {
        throw std::runtime_error(
            "Reference or output directory does not exist");
    }

    for (const auto &entry : fs::directory_iterator(REFERENCE_DIR)) {
        if (entry.is_regular_file() && entry.path().extension() == ".h5") {
            auto testFile = (outputPath / entry.path().filename()).string();
            auto referenceFile = referencePath / fs::path(testFile).filename();

            if (fs::exists(testFile)) {
                filePairs.emplace_back(referenceFile, testFile);
            } else {
                std::cerr << "Warning: No matching output file for " << referencePath << std::endl;
            }
        }
    }

    return filePairs;
}
#endif // NETCDF_UTILS_H
