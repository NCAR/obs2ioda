#include <netcdf_attribute.h>
#include <gtest/gtest.h>
#include "fixture/NetcdfCxxAttributeFixture.h"


TEST_P(NetcdfCxxAttributeFixture, PutGetAttribute) {
    auto parameterizedGroupName = std::get<0>(GetParam());
    auto parameterizedVariableName = std::get<1>(GetParam());
    int status = Obs2Ioda::netcdfPutAtt(
        netcdfAttributeFileID, scalarAttributeName.c_str(),
        &putScalarAttributeValue, parameterizedVariableName,
        parameterizedGroupName, netCDF::NcType(netCDF::ncInt), 1
    );
    EXPECT_EQ(status, 0);

    status = Obs2Ioda::netcdfGetAtt(
        netcdfAttributeFileID, scalarAttributeName.c_str(),
        &getScalarAttributeValue, parameterizedVariableName,
        parameterizedGroupName
    );
    EXPECT_EQ(status, 0);
    EXPECT_EQ(getScalarAttributeValue, putScalarAttributeValue);

    status = Obs2Ioda::netcdfPutAtt(
        netcdfAttributeFileID, arrayAttributeName.c_str(),
        putArrayAttributeValue.data(), parameterizedVariableName,
        parameterizedGroupName, netCDF::NcType(netCDF::ncInt),
        putArrayAttributeValue.size()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfGetAtt(
        netcdfAttributeFileID, arrayAttributeName.c_str(),
        getArrayAttributeValue.get(), parameterizedVariableName,
        parameterizedGroupName
    );
    EXPECT_EQ(status, 0);
    for (size_t i = 0; i < putArrayAttributeValue.size(); ++i) {
        EXPECT_EQ(
            getArrayAttributeValue.get()[i], putArrayAttributeValue[i]
        );
    }
    status = Obs2Ioda::netcdfPutAtt(
        netcdfAttributeFileID, stringAttributeName.c_str(),
        putStringArrayAttributeValue, parameterizedVariableName,
        parameterizedGroupName, netCDF::NcType(netCDF::ncString), 2
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfGetAtt(
        netcdfAttributeFileID, stringAttributeName.c_str(),
        getStringArrayAttributeValue, parameterizedVariableName,
        parameterizedGroupName
    );
    EXPECT_EQ(status, 0);
    for (size_t i = 0; i < numStrings; ++i) {
        EXPECT_STREQ(
            getStringArrayAttributeValue[i],
            putStringArrayAttributeValue[i]
        );
    }
    status = Obs2Ioda::netcdfPutAtt(
        netcdfAttributeFileID, stringAttributeName.c_str(),
        putStringAttributeValue.c_str(), parameterizedVariableName,
        parameterizedGroupName, netCDF::NcType(netCDF::ncString),
        putStringAttributeValue.size()
    );
    EXPECT_EQ(status, 0);
    status = Obs2Ioda::netcdfGetAtt(
        netcdfAttributeFileID, stringAttributeName.c_str(),
        getStringAttributeValue, parameterizedVariableName,
        parameterizedGroupName
    );
    EXPECT_EQ(status, 0);
    EXPECT_STREQ(
        getStringAttributeValue, putStringAttributeValue.c_str()
    );
}

INSTANTIATE_TEST_SUITE_P(
    PutGetAttribute, NetcdfCxxAttributeFixture,
    ::testing::Values( std::make_tuple(nullptr, nullptr) , std::
        make_tuple("group", "variable") , std::make_tuple("group",
            nullptr) , std::make_tuple(nullptr, "variable") )
);


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
