#include "netcdf_c.h"
#include <mutex>

namespace Obs2Ioda {

    int netcdfAddGroup(
            int netcdfID,
            const char *parentGroupName,
            const char *groupName
    ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            file->addGroup(groupName);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }

}
