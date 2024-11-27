#ifndef OBS2IODA_NETCDF_DIMENSION_H
#define OBS2IODA_NETCDF_DIMENSION_H


#include <netcdf>
#include <map>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "ioda_names.h"

namespace Obs2Ioda {

    extern "C" {

    int netcdfAddDim(
            int netcdfID,
            const char *groupName,
            const char *dimName,
            int len
    );

    }
}


#endif //OBS2IODA_NETCDF_DIMENSION_H
