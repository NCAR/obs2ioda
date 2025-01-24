#include "netcdf_group.h"
#include "netcdf_utils.h"
#include "netcdf_file.h"
#include "netcdf_error.h"

namespace Obs2Ioda {

    int netcdfAddGroup(
            int netcdfID,
            const char *parentGroupName,
            const char *groupName
    ) {
        try {
            auto file = FileMap::getInstance().getFile(netcdfID);
            const auto parrentGroup = getParentGroup(netcdfID, parentGroupName);
            const auto group = parrentGroup->addGroup(groupName);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    __LINE__,
                    __FILE__
            );
        }
    }

}
