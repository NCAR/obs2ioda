#include "netcdf_c.h"
#include <mutex>
#include <memory>

namespace Obs2Ioda {

    int netcdfCreate(
            const char *path,
            int *netcdfID
    ) {
        try {
            auto file = std::make_shared<netCDF::NcFile>(
                    path,
                    netCDF::NcFile::replace
            );
            *netcdfID = file->getId();
            std::lock_guard<std::mutex> lock(map_mutex);
            NETCDF_FILE_MAP[*netcdfID] = file;
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }

    int netcdfClose(int netcdfID) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            file->close();
            NETCDF_FILE_MAP.erase(netcdfID);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }
}
