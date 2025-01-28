#ifndef OBS2IODA_NETCDF_ATTRIBUTE_H
#define OBS2IODA_NETCDF_ATTRIBUTE_H

#include <netcdf>
#include <map>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "ioda_names.h"

namespace Obs2Ioda {

    template<typename T>
    int netcdfPutAtt(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            netCDF::NcType netcdfDataType,
            T value
    );

    extern "C" {

    int netcdfPutAttString(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            const char *data
    );
    int netcdfPutAttInt(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            const int *data
    );
           int netcdfPutAttInt1
    }
}
#endif //OBS2IODA_NETCDF_ATTRIBUTE_H