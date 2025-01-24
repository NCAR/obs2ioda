#include "netcdf_group.h"
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
            // Set the parent group to the root group if the parent group name is null.
            // Otherwise, set the parent group to the group with the specified name.
            const auto parentGroup = parentGroupName == nullptr
                                         ? file
                                         : std::make_shared<
                                             netCDF::NcGroup>(
                                             file->getGroup(
                                                 parentGroupName));
            const auto group = parentGroup->addGroup(groupName);
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
