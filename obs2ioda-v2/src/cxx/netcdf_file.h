#ifndef OBS2IODA_NETCDF_FILE_H
#define OBS2IODA_NETCDF_FILE_H

#include <netcdf>
#include <map>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace Obs2Ioda {

    extern "C" {


// Wrapper for nc_create
    int netcdfCreate(
            const char *path, int *netcdfID
    );

    int netcdfClose(int netcdfID);

    }

}
#endif //OBS2IODA_NETCDF_FILE_H

