#include "netcdf_dimension.h"
#include "netcdf_file.h"
#include "netcdf_error.h"
#include "netcdf_names.h"

namespace Obs2Ioda {
    int netcdfAddDim(
        const int netcdfID,
        const char *groupName,
        const char *dimName,
        const int len
    ) {
        try {
            const auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            auto strippedName = trim(dimName);
            auto dim = group->addDim(strippedName, len);
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
