#include "netcdf_attribute.h"
#include "netcdf_file.h"
#include "netcdf_error.h"

namespace Obs2Ioda {

    int netcdfPutAttIntArray(
        int netcdfID, const char *attName, const int *attValue,
        const int attLen, const char *varName, const char *groupName
    ) {
        return netcdfPutAtt(
            netcdfID, attName, attValue, varName, groupName,
            netCDF::NcType(netCDF::ncInt), attLen
        );
    }

    int netcdfPutAttRealArray(
        int netcdfID, const char *attName, const float *attValue,
        const int attLen, const char *varName, const char *groupName
    ) {
        return netcdfPutAtt(
            netcdfID, attName, attValue, varName, groupName,
            netCDF::NcType(netCDF::ncFloat), attLen
        );
    }

    int netcdfPutAttInt(
        int netcdfID, const char *attName, const int *attValue,
        const char *varName, const char *groupName
    ) {
        return netcdfPutAtt(
            netcdfID, attName, attValue, varName, groupName,
            netCDF::NcType(netCDF::ncInt), 1
        );
    }

    int netcdfPutAttString(
        const int netcdfID, const char *attName, const char *attValue,
        const char *varName, const char *groupName
    ) {
        return netcdfPutAtt(
            netcdfID, attName, attValue, varName, groupName,
            netCDF::NcType(netCDF::ncString), strlen(attValue)
        );
    }
}
