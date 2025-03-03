#ifndef NETCDF_GET_VARIABLE_H
#define NETCDF_GET_VARIABLE_H
namespace Obs2Ioda {

    extern "C" {

    int netcdfGetVarSize(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int *varSize
    );

 /**
 * @brief Frees memory allocated for an array of strings retrieved from a NetCDF file.
 *
 * @param numStrings The number of strings in the array.
 * @param data A pointer to the array of C-style strings to be freed.
 *
 * This function is used to deallocate memory allocated for storing string data
 * retrieved from a NetCDF variable. It iterates over each string in the array,
 * frees its allocated memory.
 */
    void netcdfFreeString(
        int numStrings,
        char ***data);

    int netcdfGetVarInt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int **data
    );

    int netcdfGetVarInt64(
        int netcdfID,
        const char *groupName,
        const char *varName,
        long long **data
    );

    int netcdfGetVarReal(
        int netcdfID,
        const char *groupName,
        const char *varName,
        float **data
    );

    int netcdfGetVarString(
        int netcdfID,
        const char *groupName,
        const char *varName,
        char ***data
    );
    }
}

#endif //NETCDF_GET_VARIABLE_H
