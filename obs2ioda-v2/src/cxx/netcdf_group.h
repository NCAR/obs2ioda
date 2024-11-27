#ifndef OBS2IODA_NETCDF_GROUP_H_
#define OBS2IODA_NETCDF_GROUP_H_
#include <netcdf>
#include <map>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "ioda_names.h"

namespace Obs2Ioda {

    extern "C" {
    int netcdfAddGroup(
            int netcdfID,
            const char *parentGroupName,
            const char *groupName
    );

    }

}
#endif //OBS2IODA_NETCDF_GROUP_H_

