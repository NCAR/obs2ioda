#include "netcdf_group.h"
#include "netcdf_utils.h"
#include "netcdf_file.h"
#include "netcdf_error.h"

namespace Obs2Ioda {

    std::shared_ptr<netCDF::NcGroup> getGroup(
            int netcdfID,
            const char *groupName
    ) {
        auto file = FileMap::getInstance().getFile(netcdfID);
        if (groupName != nullptr) {
            return std::make_shared<netCDF::NcGroup>(file->getGroup(groupName));
        }
        return file;
    }

    int netcdfAddGroup(
            int netcdfID,
            const char *parentGroupName,
            const char *groupName
    ) {
        try {
            auto file = FileMap::getInstance().getFile(netcdfID);
            const auto rootGroup = getGroup(netcdfID, parentGroupName);
            const auto group = rootGroup->addGroup(groupName);
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
