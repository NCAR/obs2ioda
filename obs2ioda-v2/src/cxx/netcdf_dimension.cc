#include "netcdf_dimension.h"
#include "netcdf_file.h"
#include "netcdf_error.h"

namespace Obs2Ioda {
    int netcdfAddDim(
        const int netcdfID,
        const char *groupName,
        const char *dimName,
        const int len,
        int *dimID
    ) {
        try {
            const auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            auto iodaDimName = iodaSchema.getDimension(dimName)->name;
            auto dim = group->addDim(iodaDimName, len);
            *dimID = dim.getId();
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
