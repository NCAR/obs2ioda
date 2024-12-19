#include "netcdf_error.h"

namespace Obs2Ioda {

    int netcdfErrorMessage(
            netCDF::exceptions::NcException &e
    ) {
        std::cerr
                << "NetCDF Error: "
                << e.what()
                << std::endl;
        return e.errorCode();
    }
}
