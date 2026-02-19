#include <netcdf>
#include <cstdio>
#include <string>
#include <vector>

#include "netcdf_variable.h"
#include "netcdf_file.h"

#include "eckit/testing/Test.h"

using namespace eckit::testing;
using namespace Obs2Ioda;

namespace {
    static void assertVarNotCompressed(
        const std::string &path, const std::string &varName
    ) {
        int ncid, varid;

        ASSERT(nc_open(path.c_str(), NC_NOWRITE, &ncid) == NC_NOERR);
        ASSERT(nc_inq_varid(ncid, varName.c_str(), &varid) == NC_NOERR);

        int shuffle = 0, deflate = 0, level = 0;
        ASSERT(
            nc_inq_var_deflate(ncid, varid, &shuffle, &deflate, &level)
            == NC_NOERR
        );

        EXPECT(deflate == 0);

        int storage = 0;
        size_t chunks[NC_MAX_VAR_DIMS] = {0};
        ASSERT(
            nc_inq_var_chunking(ncid, varid, &storage, chunks) ==
            NC_NOERR
        );

        EXPECT(storage == NC_CONTIGUOUS);

        nc_close(ncid);
    }


    static void assertVarCompressed(
        const std::string &path, const std::string &varName,
        int expectedLevel
    ) {
        int ncid, varid;

        ASSERT(nc_open(path.c_str(), NC_NOWRITE, &ncid) == NC_NOERR);
        ASSERT(nc_inq_varid(ncid, varName.c_str(), &varid) == NC_NOERR);

        int shuffle = 0, deflate = 0, level = 0;
        ASSERT(
            nc_inq_var_deflate(ncid, varid, &shuffle, &deflate, &level)
            == NC_NOERR
        );

        EXPECT(deflate == 1);
        EXPECT(level == expectedLevel);

        int storage = 0;
        size_t chunks[NC_MAX_VAR_DIMS] = {0};
        ASSERT(
            nc_inq_var_chunking(ncid, varid, &storage, chunks) ==
            NC_NOERR
        );
        EXPECT(storage == NC_CHUNKED);

        nc_close(ncid);
    }

    static void assertVarFilterState(const std::string& path,
                                 const std::string& varName,
                                 int expectShuffle,
                                 int expectDeflate,
                                 int expectLevel)
    {
        int ncid, varid;
        ASSERT(nc_open(path.c_str(), NC_NOWRITE, &ncid) == NC_NOERR);
        ASSERT(nc_inq_varid(ncid, varName.c_str(), &varid) == NC_NOERR);

        int shuffle = 0, deflate = 0, level = 0;
        ASSERT(nc_inq_var_deflate(ncid, varid, &shuffle, &deflate, &level) == NC_NOERR);

        EXPECT(shuffle == expectShuffle);
        EXPECT(deflate == expectDeflate);

        if(expectDeflate)
            EXPECT(level == expectLevel);
        else
            EXPECT(level == 0);

        nc_close(ncid);
    }



    struct NetcdfVariableFixture {
        std::string filePath;
        int netcdfID;
        std::shared_ptr<netCDF::NcFile> file;
        ZlibSettings zlibSettings;

        NetcdfVariableFixture() : filePath("test_variable.nc"),
                                  netcdfID(-1) {
            std::remove(filePath.c_str());
            file = std::make_shared<netCDF::NcFile>(
                filePath, netCDF::NcFile::replace
            );
            netcdfID = file->getId();
            FileMap::getInstance().addFile(netcdfID, file);
        }

        ~NetcdfVariableFixture() {
            try {
                FileMap::getInstance().removeFile(netcdfID);
            } catch (...) {
            }
            std::remove(filePath.c_str());
        }
    };
} // namespace

//--------------------------------------------------------------------
// Add int variable and put values
//--------------------------------------------------------------------

CASE("NetcdfVariable - AddVarAndPutIntValues") {
    NetcdfVariableFixture f;

    f.file->addDim("loc", 4);
    const char *dims[] = {"loc"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "var_int", NC_INT, 1, dims, &f.zlibSettings) == 0
    );

    int values[] = {1, 2, 3, 4};
    EXPECT(netcdfPutVarInt(f.netcdfID, "", "var_int", values) == 0);

    int result[4];
    f.file->getVar("var_int").getVar(result);
    EXPECT(result[0] == 1);
    EXPECT(result[3] == 4);
}

//--------------------------------------------------------------------
// Put double values
//--------------------------------------------------------------------

CASE("NetcdfVariable - PutDoubleValues") {
    NetcdfVariableFixture f;

    f.file->addDim("dim1", 2);
    const char *dims[] = {"dim1"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "var_double", NC_DOUBLE, 1, dims, &f.zlibSettings)
        == 0
    );

    double vals[] = {3.14, 2.71};
    EXPECT(netcdfPutVarDouble(f.netcdfID, "", "var_double", vals) == 0);

    double out[2];
    f.file->getVar("var_double").getVar(out);
    EXPECT(out[1] == 2.71);
}

//--------------------------------------------------------------------
// Put float values
//--------------------------------------------------------------------

CASE("NetcdfVariable - PutFloatValues") {
    NetcdfVariableFixture f;

    f.file->addDim("d", 3);
    const char *dims[] = {"d"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "var_float", NC_FLOAT, 1, dims, &f.zlibSettings) ==
        0
    );

    float vals[] = {1.1f, 2.2f, 3.3f};
    EXPECT(netcdfPutVarReal(f.netcdfID, "", "var_float", vals) == 0);

    float out[3];
    f.file->getVar("var_float").getVar(out);
    EXPECT(out[0] == 1.1f);
    EXPECT(out[1] == 2.2f);
    EXPECT(out[2] == 3.3f);
}

//--------------------------------------------------------------------
// Put char array values
//--------------------------------------------------------------------

CASE("NetcdfVariable - PutCharArrayValues") {
    NetcdfVariableFixture f;

    f.file->addDim("nstr", 3);
    f.file->addDim("len", 7);
    const char *dims[] = {"nstr", "len"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "char_arr", NC_CHAR, 2, dims, &f.zlibSettings) == 0
    );

    const char *values[] = {"apple", "banana", "pear"};
    EXPECT(netcdfPutVarChar(f.netcdfID, "", "char_arr", values) == 0);

    char buffer[3][7] = {};
    f.file->getVar("char_arr").getVar(&buffer[0][0]);
    EXPECT(std::string(buffer[0]) == "apple");
    EXPECT(std::string(buffer[1]) == "banana");
    EXPECT(std::string(buffer[2]) == "pear");
}

//--------------------------------------------------------------------
// Put int64 values
//--------------------------------------------------------------------

CASE("NetcdfVariable - PutInt64Values") {
    NetcdfVariableFixture f;

    f.file->addDim("d", 2);
    const char *dims[] = {"d"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "var_i64", NC_INT64, 1, dims, &f.zlibSettings) == 0
    );

    long long vals[] = {123456789LL, -987654321LL};
    EXPECT(netcdfPutVarInt64(f.netcdfID, "", "var_i64", vals) == 0);
}

//--------------------------------------------------------------------
// Set fill value int
//--------------------------------------------------------------------

CASE("NetcdfVariable - SetFillValueInt") {
    NetcdfVariableFixture f;

    f.file->addDim("n", 1);
    const char *dims[] = {"n"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "var_with_fill", NC_INT, 1, dims, &f.zlibSettings)
        == 0
    );
    EXPECT(
        netcdfSetFillInt(f.netcdfID, "", "var_with_fill", true, -999) ==
        0
    );
}

//--------------------------------------------------------------------
// Set fill value string
//--------------------------------------------------------------------

CASE("NetcdfVariable - SetFillValueString") {
    NetcdfVariableFixture f;

    f.file->addDim("n", 1);
    const char *dims[] = {"n"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "str_fill", NC_STRING, 1, dims, &f.zlibSettings) ==
        0
    );
    EXPECT(
        netcdfSetFillString(f.netcdfID, "", "str_fill", true, "") == 0
    );
}

//--------------------------------------------------------------------
// Flatten char array pads and null terminates
//--------------------------------------------------------------------

CASE("NetcdfVariable - FlattenCharArrayPadsAndNullTerminates") {
    const char *values[] = {"apple", "banana", "pear"};
    size_t numStrings = 3;
    size_t stringLen = 7;

    std::vector<char> result = flattenCharArray(
        values, numStrings, stringLen
    );

    EXPECT(result.size() == numStrings * stringLen);
    EXPECT(std::string(&result[0]) == "apple");
    EXPECT(std::string(&result[7]) == "banana");
    EXPECT(std::string(&result[14]) == "pear");
    EXPECT(result[5] == '\0');
    EXPECT(result[13] == '\0');
    EXPECT(result[21] == '\0');
}

//--------------------------------------------------------------------
// Put string values
//--------------------------------------------------------------------

CASE("NetcdfVariable - PutStringValues") {
    NetcdfVariableFixture f;

    f.file->addDim("nstr", 2);
    const char *dims[] = {"nstr"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "var_str", NC_STRING, 1, dims, &f.zlibSettings) == 0
    );

    const char *inputValues[] = {"hello", "world"};
    EXPECT(
        netcdfPutVarString(f.netcdfID, "", "var_str", inputValues) == 0
    );

    char *outputValues[2] = {nullptr, nullptr};
    f.file->getVar("var_str").getVar(outputValues);

    EXPECT(std::string(outputValues[0]) == "hello");
    EXPECT(std::string(outputValues[1]) == "world");

    for (auto &outputValue: outputValues) {
        if (outputValue) free(outputValue);
    }
}

//--------------------------------------------------------------------
// Put empty and special string values
//--------------------------------------------------------------------

CASE("NetcdfVariable - PutEmptyAndSpecialStringValues") {
    NetcdfVariableFixture f;

    f.file->addDim("nstr", 3);
    const char *dims[] = {"nstr"};
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "special_str", NC_STRING, 1, dims, &f.zlibSettings)
        == 0
    );

    const char *inputValues[] = {"", "foo\nbar", "©2025!"};
    EXPECT(
        netcdfPutVarString(f.netcdfID, "", "special_str", inputValues)
        == 0
    );

    char *outputValues[3] = {nullptr, nullptr, nullptr};
    f.file->getVar("special_str").getVar(outputValues);

    EXPECT(std::string(outputValues[0]) == "");
    EXPECT(std::string(outputValues[1]) == "foo\nbar");
    EXPECT(std::string(outputValues[2]) == "©2025!");

    for (auto &outputValue: outputValues) {
        if (outputValue) free(outputValue);
    }
}

//--------------------------------------------------------------------
// AddVar with null group name returns error
//--------------------------------------------------------------------

CASE("NetcdfVariable - AddVarWithNullGroupNameReturnsError") {
    NetcdfVariableFixture f;

    const char *dims[] = {"dim"};
    int ret = netcdfAddVar(
        f.netcdfID, nullptr, "var_null_group", NC_INT, 1, dims, &f.zlibSettings
    );
    EXPECT(ret == -116);
}

//--------------------------------------------------------------------
// Compression enabled produces deflated variable
//--------------------------------------------------------------------

CASE("NetcdfVariable - CompressionEnabledProducesDeflateFilter") {
    NetcdfVariableFixture f;
    f.zlibSettings.enabled = true;

    // create a reasonably sized variable (important: netcdf may skip chunking on scalars)
    f.file->addDim("x", 20);
    f.file->addDim("y", 10);
    const char *dims[] = {"x", "y"};

    // Assume your interface enables compression when option is on
    // (adjust if your API uses a flag/env/config)
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "compressed_var", NC_FLOAT, 2, dims, &f.zlibSettings
        ) == 0
    );

    std::vector<float> data(200, 42.0f);
    EXPECT(
        netcdfPutVarReal(f.netcdfID, "", "compressed_var", data.data())
        == 0
    );

    // ---- critical ----
    // Force file close so HDF5 metadata is written
    f.file.reset();

    // Verify actual on-disk filter
    assertVarCompressed("test_variable.nc", "compressed_var", 4);
    // adjust expected level
}

//--------------------------------------------------------------------
// Scalar variables must NOT be compressed
//--------------------------------------------------------------------

CASE("NetcdfVariable - ScalarVariablesAreNotCompressed") {
    NetcdfVariableFixture f;
    f.zlibSettings.enabled = true;

    // No dimensions -> scalar variable
    EXPECT(
        netcdfAddVar(f.netcdfID, "", "scalar_var", NC_FLOAT, 0, nullptr, &f.zlibSettings)
        == 0
    );

    float value = 3.14159f;
    EXPECT(
        netcdfPutVarReal(f.netcdfID, "", "scalar_var", &value)
        == 0
    );

    // flush to disk
    f.file.reset();

    // verify storage layout
    assertVarNotCompressed("test_variable.nc", "scalar_var");
}

//--------------------------------------------------------------------
// Deflate works with shuffle disabled
//--------------------------------------------------------------------

CASE("NetcdfVariable - DeflateWithoutShuffle") {
    NetcdfVariableFixture f;

    f.zlibSettings.enabled = 1;
    f.zlibSettings.shuffle = 0;
    f.zlibSettings.deflate = 1;
    f.zlibSettings.deflateLevel = 4;

    f.file->addDim("x", 10);
    const char *dims[] = {"x"};

    EXPECT(
        netcdfAddVar(f.netcdfID, "", "deflate_only", NC_FLOAT, 1, dims, &f.zlibSettings) == 0
    );

    float data[10] = {};
    EXPECT(netcdfPutVarReal(f.netcdfID, "", "deflate_only", data) == 0);

    f.file.reset();

    assertVarFilterState("test_variable.nc", "deflate_only", 0, 1, 4);
}

//--------------------------------------------------------------------
// Shuffle works without deflate
//--------------------------------------------------------------------

CASE("NetcdfVariable - ShuffleWithoutDeflate") {
    NetcdfVariableFixture f;

    f.zlibSettings.enabled = 1;
    f.zlibSettings.shuffle = 1;
    f.zlibSettings.deflate = 0;
    f.zlibSettings.deflateLevel = 4; // ignored

    f.file->addDim("x", 10);
    const char *dims[] = {"x"};

    EXPECT(
        netcdfAddVar(f.netcdfID, "", "shuffle_only", NC_FLOAT, 1, dims, &f.zlibSettings) == 0
    );

    float data[10] = {};
    EXPECT(netcdfPutVarReal(f.netcdfID, "", "shuffle_only", data) == 0);

    f.file.reset();

    assertVarFilterState("test_variable.nc", "shuffle_only", 1, 0, 0);
}


//--------------------------------------------------------------------
// Compression preserves data values
//--------------------------------------------------------------------

CASE("NetcdfVariable - CompressionRoundTripIntegrity") {
    NetcdfVariableFixture f;
    f.zlibSettings.enabled = true;

    f.file->addDim("n", 50);
    const char *dims[] = {"n"};

    EXPECT(
        netcdfAddVar(f.netcdfID, "", "rt_var", NC_FLOAT, 1, dims, &f.zlibSettings) == 0
    );

    std::vector<float> input(50);
    for (size_t i = 0; i < input.size(); ++i)
        input[i] = static_cast<float>(i) * 0.25f;

    EXPECT(
        netcdfPutVarReal(f.netcdfID, "", "rt_var", input.data()) == 0
    );

    f.file.reset();

    int ncid, varid;
    nc_open("test_variable.nc", NC_NOWRITE, &ncid);
    nc_inq_varid(ncid, "rt_var", &varid);

    std::vector<float> output(50);
    nc_get_var_float(ncid, varid, output.data());

    nc_close(ncid);

    for (size_t i = 0; i < input.size(); ++i)
        EXPECT(std::abs(input[i] - output[i]) < 1e-6);
}

//--------------------------------------------------------------------
// String variables are never compressed
//--------------------------------------------------------------------

CASE("NetcdfVariable - StringVariablesAreNotCompressed") {
    NetcdfVariableFixture f;
    f.zlibSettings.enabled = true;

    f.file->addDim("nstr", 4);
    const char *dims[] = {"nstr"};

    EXPECT(
        netcdfAddVar(f.netcdfID, "", "str_no_compress", NC_STRING, 1,
            dims, &f.zlibSettings) == 0
    );

    const char *values[] = {"a", "bb", "ccc", "dddd"};
    EXPECT(
        netcdfPutVarString(f.netcdfID, "", "str_no_compress", values) ==
        0
    );

    // flush file
    f.file.reset();

    // verify storage layout
    assertVarNotCompressed("test_variable.nc", "str_no_compress");
}


//--------------------------------------------------------------------
// Entry point
//--------------------------------------------------------------------

int main(int argc, char *argv[]) {
    return run_tests(argc, argv);
}
