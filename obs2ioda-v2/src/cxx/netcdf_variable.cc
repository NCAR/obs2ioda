#include "netcdf_c.h"
#include "ioda_names.h"
#include <algorithm>
#include <mutex>

namespace Obs2Ioda {

    template int netcdfGetVar<int>(
            int,
            const char *,
            const char *,
            int **
    );

    template int netcdfGetVar<long long>(
            int,
            const char *,
            const char *,
            long long **
    );

    template int netcdfGetVar<float>(
            int,
            const char *,
            const char *,
            float **
    );

    template int netcdfSetFill<int>(
            int,
            const char *,
            const char *,
            int,
            int
    );
    template int netcdfSetFill<float>(
            int,
            const char *,
            const char *,
            int,
            float
    );
    template int netcdfSetFill<long long>(
            int,
            const char *,
            const char *,
            int,
            long long
    );
    template int netcdfSetFill<const char *>(
            int,
            const char *,
            const char *,
            int,
            const char *
    );
    template int netcdfPutVar<int>(
            int,
            const char *,
            const char *,
            const int *
    );
    template int netcdfPutVar<float>(
            int,
            const char *,
            const char *,
            const float *
    );

    int netcdfAddVar(
            int netcdfID,
            const char *groupName,
            const char *varName,
            nc_type netcdfDataType,
            int numDims,
            const char **dimNames
    ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto group = getRootGroup(
                    netcdfID,
                    groupName
            );
            auto file = NETCDF_FILE_MAP[netcdfID];
            std::string ioda3Name = getIodaName(
                    varName,
                    IODA_VARIABLE_NAMES
            );
            std::vector<netCDF::NcDim> dims;
            dims.reserve(numDims);
            for (int i = 0; i < numDims; i++) {
                dims.push_back(file->getDim(dimNames[i]));
            }
            auto var = group->addVar(
                    ioda3Name,
                    netCDF::NcType(netcdfDataType),
                    dims
            );
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }

    template<typename T>
    int netcdfPutVar(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const T *data
    ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            std::string ioda3Name = getIodaName(
                    varName,
                    IODA_VARIABLE_NAMES
            );
            std::shared_ptr<netCDF::NcGroup> group = getRootGroup(
                    netcdfID,
                    groupName
            );
            auto var = group->getVar(ioda3Name);
            var.putVar(data);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }


    template<typename T>
    int netcdfGetVar(
            int netcdfID,
            const char *groupName,
            const char *varName,
            T **data
    ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            std::string ioda3Name = getIodaName(
                    varName,
                    IODA_VARIABLE_NAMES
            );
            auto var = file->getVar(ioda3Name);
            const std::vector<size_t> start = {0};
            const std::vector<size_t> count = {var.getDim(0).getSize()};
            var.getVar(
                    start,
                    count,
                    *data
            );
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }

    template<typename T>
    int netcdfSetFill(
            int netcdfID,
            const char *groupName,
            const char *varName,
            int fillMode,
            T fillValue
            ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            std::string ioda3Name = getIodaName(
                    varName,
                    IODA_VARIABLE_NAMES
            );
            std::shared_ptr<netCDF::NcGroup> group = getRootGroup(
                    netcdfID,
                    groupName
            );
            auto var = group->getVar(ioda3Name);
            var.setFill(fillMode, fillValue);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }

    int netcdfGetVarString1D(
            int netcdfID,
            const char *groupName,
            const char *varName,
            char ***data
    ) {
        auto file = NETCDF_FILE_MAP[netcdfID];
        std::string ioda3Name = getIodaName(
                varName,
                IODA_VARIABLE_NAMES
        );
        auto dims = file->getVar(ioda3Name).getDims();
        auto var = file->getVar(ioda3Name);
        size_t numStrings = dims[0].getSize();
        size_t bufferSize = numStrings * sizeof(char *);
        char **buffer = (char **) malloc(bufferSize);
        int  retval = netcdfGetVar(
                netcdfID,
                groupName,
                varName,
                &buffer
        );
        for (auto i = 0; i < numStrings; i++) {
            std::string tmpStr = buffer[i];
            std::copy(
                    tmpStr.begin(),
                    tmpStr.end(),
                    (*data)[i]
            );
            (*data)[i][tmpStr.size()] = '\0';
        }
        var.freeString(
                5,
                (char **) buffer
        );
        free(buffer);
        return retval;
    }
}