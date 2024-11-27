#include "netcdf_group.h"
#include "netcdf_utils.h"
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
            auto group = getRootGroup(netcdfID, parentGroupName);
            group->addGroup(groupName);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }

}
