#ifndef NETCDF_ATTRIBUTE_H
#define NETCDF_ATTRIBUTE_H

namespace Obs2Ioda {
    extern "C" {
    /**
     * @brief Writes an integer attribute to a variable, group, or as a global attribute in a NetCDF file.
     *
     * If `varName` is provided, the attribute is assigned to the specified variable.
     * If `varName` is `NULL`, the attribute is assigned to the group.
     * If both `groupName` and `varName` are `NULL`, the attribute is written as a global attribute.
     *
     * @param netcdfID The identifier of the NetCDF file where the attribute will be written.
     * @param groupName The name of the group containing the variable. If `NULL`, the root group is assumed.
     * @param varName The name of the variable to which the attribute will be attached. If `NULL`, the attribute is assigned to the group.
     * @param attName The name of the attribute to be written.
     * @param attValue A pointer to the integer value to be assigned to the attribute.
     * @return int A status code indicating the outcome of the operation:
     *         - 0: Success.
     *         - Non-zero: Failure, with an error message logged.
     */
    int netcdfPutAttInt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const char *attName,
        const int *attValue
    );

    int netcdfPutAttString(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const char *attName,
        const char *attValue
    );
    }
}

#endif //NETCDF_ATTRIBUTE_H
