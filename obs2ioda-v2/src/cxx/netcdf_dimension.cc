#include "netcdf_dimension.h"

#include <ioda_names.h>

#include "netcdf_file.h"
#include "netcdf_error.h"

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
            auto iodaDimName = getIodaName(
                dimName,
                IODA_DIMENSION_NAMES
            );
            auto dim = group->addDim(iodaDimName, len);
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
