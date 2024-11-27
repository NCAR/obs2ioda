#ifndef OBS2IODA_NETCDF_C_H
#define OBS2IODA_NETCDF_C_H

#include <netcdf>
#include <map>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "ioda_names.h"

namespace Obs2Ioda {

    extern std::unordered_map<
            int,
            std::shared_ptr<netCDF::NcFile>> NETCDF_FILE_MAP;
    extern std::mutex map_mutex;


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

    int netcdfGetVarString1D(
            int netcdfID,
            const char *groupName,
            const char *varName,
            char ***data
    );

    }

}

#endif //OBS2IODA_NETCDF_C_H
