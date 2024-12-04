#include "netcdf_utils.h"
#include <algorithm>
#include <iostream>

namespace Obs2Ioda {

    std::unordered_map<
            int,
            std::shared_ptr<netCDF::NcFile>> NETCDF_FILE_MAP;
    std::mutex map_mutex;

    int netcdfErrorMessage(
            netCDF::exceptions::NcException &e,
            int errorCode
    ) {
        std::cerr
                << "NetCDF Error: "
                << e.what()
                << std::endl;
        return errorCode;
    }
}
