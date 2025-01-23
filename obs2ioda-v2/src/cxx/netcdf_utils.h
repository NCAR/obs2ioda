#ifndef NETCDF_UTILS_H
#define NETCDF_UTILS_H

#include <netcdf>
#include <unordered_map>
#include <memory>

namespace Obs2Ioda {

    std::shared_ptr<netCDF::NcGroup> getRootGroup(
            int netcdfID,
            const char *groupName
    );

    std::string removeWhiteSpace(
            const std::string &name
    );

}

#endif //NETCDF_UTILS_H
