#include "netcdf_c.h"
#include <mutex>

namespace Obs2Ioda {

    int netcdfAddDim(
            int netcdfID,
            const char *groupName,
            const char *dimName,
            int len
    ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            file->addDim(
                    dimName,
                    len
            );
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }
}
