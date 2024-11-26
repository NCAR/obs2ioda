#include <gtest/gtest.h>
#include <vector>

#include "netcdf_c.h"


class TestNetcdfSatwindFixture : public ::testing::Test {
protected:
    const char *satwindPath = "satwind.nc";
    std::shared_ptr<netCDF::NcFile> satwindFile;

    void
    SetUp() override {
        // Prepare any required resources before each test
    }

    void
    TearDown() override {
        // Clean up resources after each test
    }
};

TEST_F(TestNetcdfSatwindFixture,
       TestSatwind) {
    int netcdfID = -1;
    int result{};
    result = Obs2Ioda::netcdfCreate(
            this->satwindPath,
            &netcdfID
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            "nvars",
            2
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            "nlocs",
            278424
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddDim(
            netcdfID,
            nullptr,
            "nstring",
            50
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfPutAtt(
            netcdfID,
            nullptr,
            nullptr,
            "nvars",
            NC_INT,
            2
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfPutAtt(
            netcdfID,
            nullptr,
            nullptr,
            "nlocs",
            NC_INT,
            278424
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfPutAtt(
            netcdfID,
            nullptr,
            nullptr,
            "nstring",
            NC_INT,
            50
    );
    EXPECT_EQ(result,
              0);
    "2018-04-14T22:00:00Z";
    result = Obs2Ioda::netcdfPutAtt(
            netcdfID,
            nullptr,
            nullptr,
            "min_datetime",
            "2018-04-14T22:00:00Z"
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfPutAtt(
            netcdfID,
            nullptr,
            nullptr,
            "max_datetime",
            "2018-04-15T01:10:00Z"
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            "MetaData"
    );
    EXPECT_EQ(result,
              0);

    result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            "MetaData",
            "air_pressure",
            NC_FLOAT,
            1,
            std::vector<const char *>(
                    {"nlocs"}
            ).data()
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfSetFill(
            netcdfID,
            "MetaData",
            "air_pressure",
            1,
            -999.f
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            "MetaData",
            "height",
            NC_FLOAT,
            1,
            std::vector<const char *>(
                    {"nlocs"}
            ).data()
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfSetFill(
            netcdfID,
            "MetaData",
            "height",
            1,
            -999.f
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            "MetaData",
            "latitude",
            NC_FLOAT,
            1,
            std::vector<const char *>(
                    {"nlocs"}
            ).data()
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfSetFill(
            netcdfID,
            "MetaData",
            "latitude",
            1,
            -999.f
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            "MetaData",
            "longitude",
            NC_FLOAT,
            1,
            std::vector<const char *>(
                    {"nlocs"}
            ).data()
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfSetFill(
            netcdfID,
            "MetaData",
            "longitude",
            1,
            -999.f
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            "MetaData",
            "dateTime",
            NC_INT64,
            1,
            std::vector<const char *>(
                    {"nlocs"}
            ).data()
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfPutAtt(
            netcdfID,
            "MetaData",
            "dateTime",
            "units",
            "seconds since 1970-01-01T00:00:00Z"
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            "MetaData",
            "station_id",
            NC_STRING,
            1,
            std::vector<const char *>(
                    {"nlocs"}
            ).data()
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfSetFill(
            netcdfID,
            "MetaData",
            "station_id",
            1,
            ""
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            nullptr,
            "variable_names",
            NC_STRING,
            1,
            std::vector<const char *>(
                    {"nvars"}
            ).data()
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfSetFill(
            netcdfID,
            nullptr,
            "variable_names",
            1,
            ""
    );
    EXPECT_EQ(result,
              0);

    /*
     * group: ObsValue {
  variables:
        float eastward_wind(nlocs) ;
                eastward_wind:_FillValue = -999.f ;
                eastward_wind:units = "m/s" ;
        float northward_wind(nlocs) ;
                northward_wind:_FillValue = -999.f ;
                northward_wind:units = "m/s" ;
  } // group ObsValue

     */
    result = Obs2Ioda::netcdfAddGroup(
            netcdfID,
            nullptr,
            "ObsValue"
    );
    result = Obs2Ioda::netcdfAddVar(
            netcdfID,
            "ObsValue",
            "northward_wind",
            NC_FLOAT,
            1,
            std::vector<const char *>(
                    {"nlocs"}
            ).data()
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfSetFill(
            netcdfID,
            "ObsValue",
            "northward_wind",
            1,
            -999.f
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfPutAtt(
            netcdfID,
            "ObsValue",
            "windNorthward",
            "units",
            "m/s"
    );
    EXPECT_EQ(result,
              0);
    result = Obs2Ioda::netcdfPutVar(
            netcdfID,
            "ObsValue",
            "northward_wind",
            std::vector<float>(
                    10,
                    1.0f
            ).data()
    );
    EXPECT_EQ(result,
              0);


    result = Obs2Ioda::netcdfClose(
            netcdfID
    );
    EXPECT_EQ(result,
              0);
}


int main(
        int argc,
        char **argv
) {
    ::testing::InitGoogleTest(
            &argc,
            argv
    );
    return RUN_ALL_TESTS();
}