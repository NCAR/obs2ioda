#ifndef OBS2IODA_NETCDF_UTILS_H
#define OBS2IODA_NETCDF_UTILS_H


#include <netcdf>
#include <map>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace Obs2Ioda {


    extern std::unordered_map<
            int,
            std::shared_ptr<netCDF::NcFile>> NETCDF_FILE_MAP;
    extern std::mutex map_mutex;

    int netcdfErrorMessage(
            netCDF::exceptions::NcException &e,
            int errorCode
    );

}


#endif //OBS2IODA_NETCDF_UTILS_H
