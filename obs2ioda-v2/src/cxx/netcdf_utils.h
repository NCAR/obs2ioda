#ifndef OBS2IODA_NETCDF_UTILS_H
#define OBS2IODA_NETCDF_UTILS_H


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

    std::string removeWhiteSpace(
            const std::string &name
    );

}


#endif //OBS2IODA_NETCDF_UTILS_H
