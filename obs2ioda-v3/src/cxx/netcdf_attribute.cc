#include "netcdf_attribute.h"

#include <ioda_group.h>
#include <ioda_variable.h>

#include "netcdf_file.h"
#include "netcdf_error.h"

namespace Obs2Ioda {
    template<typename T>
int netcdfPutAtt(
    int netcdfID, const char *attName, T values,
    const char *varName, const char *groupName,
    const netCDF::NcType &netcdfDataType, size_t len
) {
    try {
        auto file = FileMap::getInstance().getFile(netcdfID);
        std::string iodaGroupName;

        if (varName) {
            IodaVariable iodaVariable(varName);
            if (iodaVariable.isV1Variable()) {
                IodaGroup iodaGroup(varName);
                const auto& v1DerivedGroupName = iodaGroup.getName();
                auto v1Group = file->getGroup(v1DerivedGroupName);
                if (v1Group.isNull()) {
                    v1Group = file->addGroup(v1DerivedGroupName);
                }
                iodaGroupName = v1Group.getName();
            }
        }

        if (iodaGroupName.empty() && groupName != nullptr) {
            iodaGroupName = iodaSchema.getGroup(groupName)->getValidName();
        }

        std::shared_ptr<netCDF::NcGroup> group = iodaGroupName.empty()
            ? file
            : std::make_shared<netCDF::NcGroup>(file->getGroup(iodaGroupName));

        if (varName) {
            IodaVariable iodaVariable(varName);
            auto iodaVarName = iodaVariable.getName();
            auto var = group->getVar(iodaVarName);

            if (netcdfDataType == netCDF::ncString) {
                var.putAtt(
                    attName, std::string(
                        reinterpret_cast<const char *>(values)
                    )
                );
            } else {
                var.putAtt(attName, netcdfDataType, len, values);
            }
        } else {
            if (netcdfDataType == netCDF::ncString) {
                group->putAtt(
                    attName, std::string(
                        reinterpret_cast<const char *>(values)
                    )
                );
            } else {
                group->putAtt(attName, netcdfDataType, len, values);
            }
        }

        return 0;
    } catch (const netCDF::exceptions::NcException &e) {
        return netcdfErrorMessage(e, __LINE__, __FILE__);
    }
}


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
