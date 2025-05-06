#ifndef NETCDF_ATTRIBUTE_H
#define NETCDF_ATTRIBUTE_H
#include <netcdf_error.h>
#include <netcdf_file.h>

namespace Obs2Ioda {
    template<typename T> int netcdfGetAtt(
        const int netcdfID, const char *attName, T values,
        const char *varName, const char *groupName
    ) {
        try {
            auto file = FileMap::getInstance().getFile(netcdfID);
            std::shared_ptr<netCDF::NcGroup> group =
                    (groupName && *groupName)
                        ? std::make_shared<netCDF::NcGroup>(
                            file->getGroup(groupName)
                        ) : file;

            auto getAttValues = [&](auto &ncEntity) {
                auto attLength = ncEntity.getAtt(attName).getAttLength();
                if constexpr (std::is_same_v<T, char **>) {
                    auto att = ncEntity.getAtt(attName);
                    auto tmpBuffer = new char *[attLength];
                    att.getValues(tmpBuffer);
                    auto stringLength = strlen(tmpBuffer[0]) + 1;
                    for (size_t i = 0; i < attLength; ++i) {
                        std::copy(
                            tmpBuffer[i], tmpBuffer[i] + stringLength,
                            values[i]
                        );
                    }
                    for (size_t i = 0; i < attLength; ++i) {
                        free(tmpBuffer[i]);
                    }
                    delete[] tmpBuffer;
                } else {
                    if constexpr (std::is_same_v<T, char *>) {
                        values[attLength] = '\0';
                    }
                    ncEntity.getAtt(attName).getValues(values);
                }
            };

            if (varName) {
                const auto iodaVarName = iodaSchema.getVariable(varName)
                        ->getValidName();
                auto var = group->getVar(iodaVarName);
                getAttValues(var);
            } else {
                getAttValues(*group);
            }

            return 0;
        } catch (const netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }


    template<typename T> int netcdfPutAtt(
        int netcdfID, const char *attName, T values,
        const char *varName, const char *groupName,
        const netCDF::NcType &netcdfDataType, size_t len
    ) {
        try {
            auto file = FileMap::getInstance().getFile(netcdfID);
            std::shared_ptr<netCDF::NcGroup> group =
                    (groupName && *groupName)
                        ? std::make_shared<netCDF::NcGroup>(
                            file->getGroup(groupName)
                        ) : file;

            auto putVarAtt = [&](auto &var) {
                if constexpr (std::is_same_v<T, const char *>) {
                    var.putAtt(attName, std::string(values));
                } else if constexpr (std::is_same_v<T, const char **>) {
                    var.putAtt(attName, len, values);
                } else {
                    var.putAtt(attName, netcdfDataType, len, values);
                }
            };

            auto putGroupAtt = [&](auto &grp) {
                if constexpr (std::is_same_v<T, const char *>) {
                    grp.putAtt(attName, std::string(values));
                } else if constexpr (std::is_same_v<T, const char **>) {
                    grp.putAtt(attName, len, values);
                } else {
                    grp.putAtt(attName, netcdfDataType, len, values);
                }
            };

            if (varName) {
                auto iodaVarName = iodaSchema.getVariable(varName)->
                        getValidName();
                auto var = group->getVar(iodaVarName);
                putVarAtt(var);
            } else {
                putGroupAtt(*group);
            }

            return 0;
        } catch (const netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(e, __LINE__, __FILE__);
        }
    }


    extern "C" {
    /**
     * @brief Writes an attribute to a variable, group, or as a global attribute in a NetCDF file.
     *
     * If `varName` is provided, the attribute is assigned to the specified variable.
     * If `varName` is `NULL`, the attribute is assigned to the group.
     * If both `groupName` and `varName` are `NULL`, the attribute is written as a global attribute.
     *
     * @param netcdfID The identifier of the NetCDF file where the attribute will be written.
     * @param attName The name of the attribute to be written.
     * @param attValue A pointer to the integer value to be assigned to the attribute.
     * @param varName The name of the variable to which the attribute will be attached. If `NULL`, the attribute is assigned to the group.
     * @param groupName The name of the group containing the variable. If `NULL`, the root group is assumed.
     * @return int A status code indicating the outcome of the operation:
     *         - 0: Success.
     *         - Non-zero: Failure, with an error message logged.
     */
    int netcdfPutAttInt(
        int netcdfID, const char *attName, const int *attValue,
        const char *varName, const char *groupName
    );

    /**
     * @brief Writes an integer array attribute to a variable, group, or as a global attribute in a NetCDF file.
     *
     * This function writes an attribute that contains an array of integer values.
     * If `varName` is provided, the attribute is assigned to the specified variable.
     * If `varName` is `NULL`, the attribute is assigned to the group.
     * If both `groupName` and `varName` are `NULL`, the attribute is written as a global attribute.
     *
     * @param netcdfID The identifier of the NetCDF file where the attribute will be written.
     * @param attName The name of the attribute to be written.
     * @param attValue A pointer to the array of integer values to be assigned to the attribute.
     * @param attLen The length of the integer array.
     * @param varName The name of the variable to which the attribute will be attached. If `NULL`, the attribute is assigned to the group.
     * @param groupName The name of the group containing the variable. If `NULL`, the root group is assumed.
     * @return int A status code indicating the outcome of the operation:
     *         - 0: Success.
     *         - Non-zero: Failure, with an error message logged.
     */
    int netcdfPutAttIntArray(
        int netcdfID, const char *attName, const int *attValue,
        int attLen, const char *varName, const char *groupName
    );

    int netcdfPutAttRealArray(
        int netcdfID, const char *attName, const float *attValue,
        int attLen, const char *varName, const char *groupName
    );

    int netcdfPutAttString(
        int netcdfID, const char *attName, const char *attValue,
        const char *varName, const char *groupName
    );
    }
}

#endif //NETCDF_ATTRIBUTE_H
