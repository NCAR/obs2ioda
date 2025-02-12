#include "netcdf_attribute.h"
#include "netcdf_file.h"
#include "netcdf_error.h"

namespace Obs2Ioda {
    template<typename T>
    int netcdfPutAtt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const char *attName,
        const netCDF::NcType &netcdfDataType,
        size_t len,
        T values
    ) {
        try {
            auto file = FileMap::getInstance().getFile(
                netcdfID
            );
            std::shared_ptr<netCDF::NcGroup> group = (groupName && *
                groupName)
                ? std::make_shared<netCDF::NcGroup>(
                    file->getGroup(
                        groupName
                    )
                )
                : file;

            // This is required because the putAtt method signature is different for strings.
            auto putStringAtt = [](
                auto &target,
                const char *stringAttName,
                T attValue
            ) {
                target.putAtt(
                    stringAttName,
                    std::string(
                        reinterpret_cast<const char *>(attValue)
                    )
                );
            };

            if (varName) {
                auto var = group->getVar(
                    varName
                );
                if (netcdfDataType == netCDF::ncString) {
                    putStringAtt(
                        var,
                        attName,
                        values
                    );
                } else {
                    var.putAtt(
                        attName,
                        netcdfDataType,
                        len,
                        values
                    );
                }
            } else {
                if (netcdfDataType == netCDF::ncString) {
                    putStringAtt(
                        *group,
                        attName,
                        values
                    );
                } else {
                    group->putAtt(
                        attName,
                        netcdfDataType,
                        len,
                        values
                    );
                }
            }

            return 0;
        } catch (const netCDF::exceptions::NcException &e) {
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
        return netcdfPutAtt(
            netcdfID,
            groupName,
            varName,
            attName,
            netCDF::NcType(
                netCDF::ncInt
            ),
            1,
            attValue
        );
    }

    int netcdfPutAttString(
        const int netcdfID,
        const char *groupName,
        const char *varName,
        const char *attName,
        const char *attValue
    ) {
        return netcdfPutAtt(
            netcdfID,
            groupName,
            varName,
            attName,
            netCDF::NcType(
                netCDF::ncString
            ),
            strlen(
                attValue
            ),
            attValue
        );
    }
}
