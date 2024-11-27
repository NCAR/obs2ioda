#ifndef OBS2IODA_NETCDF_VARIABLE_H_
#define OBS2IODA_NETCDF_VARIABLE_H_
#include <netcdf>
#include <map>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "ioda_names.h"

namespace Obs2Ioda {

    template<typename T>
    int netcdfPutVar(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const T *data
    );

    template<typename T>
    int netcdfGetVar(
            int netcdfID,
            const char *groupName,
            const char *varName,
            T **data
    );

    template<typename T>
    int netcdfSetFill(
            int netcdfID,
            const char *groupName,
            const char *varName,
            int fillMode,
            T fillValue
    );

    extern "C" {

    int netcdfAddVar(
            int netcdfID,
            const char *groupName,
            const char *varName,
            nc_type netcdfDataType,
            int numDims,
            const char **dimNames
    );
    int netcdfPutVarInt(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const int *data
    );

    int netcdfPutVarInt64(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const long long *data
    );

    int netcdfPutVarReal(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const float *data
    );

    int netcdfPutVarString(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char **data
    );

    int netcdfGetVarString1D(
            int netcdfID,
            const char *groupName,
            const char *varName,
            char ***data
    );

    }

}
#endif //OBS2IODA_NETCDF_VARIABLE_H_

