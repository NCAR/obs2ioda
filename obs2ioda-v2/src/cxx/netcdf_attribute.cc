#include "netcdf_attribute.h"
#include "netcdf_utils.h"
#include "ioda_names.h"
#include <mutex>
#include <memory>

namespace Obs2Ioda {

    template int netcdfPutAtt<int>(
            int,
            const char *,
            const char *,
            const char *,
            netCDF::NcType,
            int
    );

    template int netcdfPutAtt<float>(
            int,
            const char *,
            const char *,
            const char *,
            netCDF::NcType,
            float
    );

   template int netcdfPutAtt1D<int>(
            int,
            const char *,
            const char *,
            const char *,
            netCDF::NcType,
            int *,
            size_t
    );


    template<typename T>
    int netcdfPutAtt(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            netCDF::NcType netcdfDataType,
            T value
    ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            netCDF::NcVar var;
            std::shared_ptr<netCDF::NcGroup> group = getRootGroup(
                    netcdfID,
                    groupName
            );
            if (varName != nullptr) {
                auto ioda3VarName = getIodaName(
                        varName,
                        IODA_VARIABLE_NAMES
                );
                var = group->getVar(ioda3VarName);
                var.putAtt(
                        attName,
                        netcdfDataType,
                        value
                );
            }
            else {
                group->putAtt(
                        attName,
                        netcdfDataType,
                        value
                );
            }
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }

    template<typename T>
    int netcdfPutAtt1D(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            netCDF::NcType netcdfDataType,
            T *value,
            const size_t len
    ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            netCDF::NcVar var;
            std::shared_ptr<netCDF::NcGroup> group = getRootGroup(
                    netcdfID,
                    groupName
            );
            if (varName != nullptr) {
                auto ioda3VarName = getIodaName(
                        varName,
                        IODA_VARIABLE_NAMES
                );
                var = group->getVar(ioda3VarName);
                var.putAtt(
                        attName,
                        netcdfDataType,
                        len,
                        value
                );
            }
            else {
                group->putAtt(
                        attName,
                        netcdfDataType,
                        len,
                        value
                );
            }
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }
    int netcdfPutAttString(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            const char *data
    ) {
        try {
            std::lock_guard<std::mutex> lock(map_mutex);
            auto file = NETCDF_FILE_MAP[netcdfID];
            netCDF::NcVar var;
            std::shared_ptr<netCDF::NcGroup> group = getRootGroup(
                    netcdfID,
                    groupName
            );
            if (varName != nullptr) {
                auto ioda3Name = getIodaName(
                        varName,
                        IODA_VARIABLE_NAMES
                );
                var = group->getVar(ioda3Name);
                var.putAtt(
                        attName,
                        data
                );
            }
            else {
                group->putAtt(
                        attName,
                        data
                );
            }
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                    e,
                    1
            );
        }
    }
    int netcdfPutAttInt(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            const int *data
    ) {
        return netcdfPutAtt<int>(
                netcdfID,
                groupName,
                varName,
                attName,
                netCDF::ncInt,
                *data
        );
    }
    int netcdfPutAttInt1D(
            int netcdfID,
            const char *groupName,
            const char *varName,
            const char *attName,
            const int **data,
            const size_t len
    ) {
        return netcdfPutAtt1D<const int*>(
                netcdfID,
                groupName,
                varName,
                attName,
                netCDF::ncInt,
                data,
                len
        );
    }
}


