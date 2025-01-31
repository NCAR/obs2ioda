#ifndef NETCDF_VARIABLE_H
#define NETCDF_VARIABLE_H
#include <netcdf>

namespace Obs2Ioda {
    /**
* @brief Writes data to a NetCDF variable.
*
* This function writes data of any supported type (`T`) to a variable in a NetCDF file,
* within the specified group or the root group.
*
* @tparam T The data type of the variable (e.g., `int`, `float`, `double`, etc.).
* @param netcdfID The identifier of the NetCDF file.
* @param groupName A null-terminated string specifying the group containing the variable.
*                  If `nullptr`, the variable is assumed to be in the root group.
* @param varName A null-terminated string specifying the name of the variable.
* @param data A pointer to the data to be written to the variable.
* @return 0 on success, non-zero on failure.
*/
    template<typename T>
    int netcdfPutVar(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const T *data
    );

    /**
 * @brief Reads data from a NetCDF variable.
 *
 * This function reads data of any supported type (`T`) from a variable in a NetCDF file,
 * within the specified group or the root group.
 *
 * @tparam T The data type of the variable (e.g., `int`, `float`, `double`, etc.).
 * @param netcdfID The identifier of the NetCDF file.
 * @param groupName A null-terminated string specifying the group containing the variable.
 *                  If `nullptr`, the variable is assumed to be in the root group.
 * @param varName A null-terminated string specifying the name of the variable.
 * @param data A pointer to a pointer that will receive the data read from the variable.
 *             Memory for the data will be allocated by the function.
 * @return 0 on success, non-zero on failure.
 */

    template<typename T>
    int netcdfGetVar(
        int netcdfID,
        const char *groupName,
        const char *varName,
        T **data
    );

    /**
     * @brief Sets the fill mode and fill value for a NetCDF variable.
     *
     * This function sets the fill mode and optional fill value for a variable in a NetCDF file.
     *
     * @tparam T The data type of the fill value (e.g., `int`, `float`, etc.).
     * @param netcdfID The identifier of the NetCDF file.
     * @param groupName A null-terminated string specifying the group containing the variable.
     *                  If `nullptr`, the variable is assumed to be in the root group.
     * @param varName A null-terminated string specifying the name of the variable.
     * @param fillMode The fill mode to be set.
     * @param fillValue The fill value to be set for the variable.
     * @return 0 on success, non-zero on failure.
     */
    template<typename T>
    int netcdfSetFill(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        T fillValue
    );

    extern "C" {
    /**
* @brief Adds a variable to a NetCDF file.
*
* @param netcdfID The identifier of the NetCDF file.
* @param groupName A null-terminated string specifying the group to which the variable will be added.
*                  If `nullptr`, the variable is added to the root group.
* @param varName A null-terminated string specifying the name of the variable.
* @param netcdfDataType The NetCDF data type of the variable (e.g., `NC_INT`, `NC_FLOAT`).
* @param numDims The number of dimensions for the variable.
* @param dimNames An array of null-terminated strings specifying the names of the dimensions.
* @return 0 on success, non-zero on failure.
*/
    int netcdfAddVar(
        int netcdfID,
        const char *groupName,
        const char *varName,
        nc_type netcdfDataType,
        int numDims,
        const char **dimNames
    );

    int netcdfPutVarInt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const int *data
    );

    int netcdfPutVarInt64(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const long long *data
    );

    int netcdfPutVarReal(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const float *data
    );

    int netcdfPutVarString(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const char **data
    );

    int netcdfGetVarString1D(
        int netcdfID,
        const char *groupName,
        const char *varName,
        char ***data
    );

    int netcdfSetFillInt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        int fillValue
    );

    int netcdfSetFillReal(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        float fillValue
    );

    int netcdfSetFillInt64(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        long long fillValue
    );

    int netcdfSetFillString(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        const char *fillValue
    );
    }
}

#endif //NETCDF_VARIABLE_H
