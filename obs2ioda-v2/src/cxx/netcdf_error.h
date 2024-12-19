#ifndef OBS2IODA_NETCDF_ERROR_H
#define OBS2IODA_NETCDF_ERROR_H


#include <netcdf>

namespace Obs2Ioda {

    int netcdfErrorMessage(
            netCDF::exceptions::NcException &e
    );

}


#endif //OBS2IODA_NETCDF_ERROR_H
