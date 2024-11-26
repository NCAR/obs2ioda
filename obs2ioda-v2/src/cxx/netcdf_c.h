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
    int netcdfPutAtt(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            netCDF::NcType netcdfDataType,
            T value
    );

    int netcdfPutAtt(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            const std::string& value
    );

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


// Wrapper for nc_create
    int netcdfCreate(
            const char *path,
            int *netcdfID
    );

    int netcdfAddGroup(
            int netcdfID,
            const char *parentGroupName,
            const char *groupName
    );

    int netcdfAddDim(
            int netcdfID,
            const char *groupName,
            const char *dimName,
            int len
    );

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

    int netcdfClose(int netcdfID);


    }

    std::string getIodaName(
            const char *name,
            const std::unordered_map<
                    std::string,
                    std::string
            > &iodaNameMap
    );

    int netcdfErrorMessage(
            netCDF::exceptions::NcException &e,
            int errorCode
    );

    std::shared_ptr<netCDF::NcGroup> getRootGroup(
            int netcdfID,
            const char *groupName
    );

}

#endif //OBS2IODA_NETCDF_C_H
