#ifndef NETCDFCXXFIXTURE_H
#define NETCDFCXXFIXTURE_H

#include <fstream>
#include <netcdf_group.h>
#include <gtest/gtest.h>
#include "netcdf_file.h"
#include "netcdf_attribute.h"
#include "netcdf_dimension.h"
#include "netcdf_variable.h"
#include <cstring>


class NetcdfCxxAttributeFixture : public ::testing::TestWithParam<std::tuple<const char *, const char *> > {
protected:
    const int numStrings = 2;
    const int stringSize = 4;
    std::string netcdfAttributeFileName = "test_attribute.nc";
    int netcdfAttributeFileID{};
    int netcdfAttributeFileMode = netCDF::NcFile::replace;
    int putScalarAttributeValue = 42;
    int getScalarAttributeValue{};
    std::vector<int> putArrayAttributeValue = {1, 2, 3, 4, 5};
    std::shared_ptr<int> getArrayAttributeValue;
    std::string putStringAttributeValue = "abc";
    const char *putStringArrayAttributeValue[2] = {
        "abc", "def"
    };
    char ** getStringArrayAttributeValue = nullptr;
    char *getStringAttributeValue = nullptr;
    std::string scalarAttributeName = "scalar_attribute";
    std::string arrayAttributeName = "array_attribute";
    std::string stringAttributeName = "string_attribute";
    std::string stringArrayAttributeName = "string_array_attribute";
    std::string variableName = "variable";
    std::string groupName = "group";
    std::string dimensionName = "dimension";

    std::vector<const char *> dimensionNames = {dimensionName.c_str()};

    int dimID{};

    void SetUp() override;

    void TearDown() override;
};


#endif //NETCDFCXXFIXTURE_H
