#include "netcdf_attribute.h"
#include "netcdf_file.h"
#include "netcdf_error.h"
#include <variant>

namespace Obs2Ioda {
    template<typename T>
    int netcdfPutAtt(
        const int netcdfID,
        const char *groupName,
        const char *varName,
        const char *attName,
        const netCDF::NcType &netcdfDataType,
        T values
    ) {
        try {
            auto file = FileMap::getInstance().getFile(
                netcdfID
            );
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName
                                       )
                                   );
            if (varName) {
                auto var = group->getVar(
                    varName
                );
                var.putAtt(
                    attName,
                    netcdfDataType,
                    values
                );
            } else {
                group->putAtt(
                    attName,
                    netcdfDataType,
                    values
                );
            }
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    int netcdfPutAttInt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const char *attName,
        const int *attValue
    ) {
        return netcdfPutAtt<int>(
            netcdfID,
            groupName,
            varName,
            attName,
            netCDF::NcType(
                netCDF::ncInt
            ),
            *attValue
        );
    }

    int netcdfPutAttString(
        const int netcdfID,
        const char *groupName,
        const char *varName,
        const char *attName,
        const char *attValue
    ) {
        try {
            auto file = FileMap::getInstance().getFile(
                netcdfID
            );
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName
                                       )
                                   );

            if (!varName) {
                group->putAtt(
                    attName,
                    attValue
                );
            } else {
                group->getVar(
                    varName
                ).putAtt(
                    attName,
                    attValue
                );
            }
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }
}
