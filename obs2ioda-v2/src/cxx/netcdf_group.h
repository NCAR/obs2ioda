#ifndef NETCDF_GROUP_H
#define NETCDF_GROUP_H
#include <memory>
#include <netcdf>

namespace Obs2Ioda {

    extern "C" {
/**
 * @brief Adds a new group to a NetCDF file, under a specified parent group.
 *
 * This function provides an interface for adding a new group to a NetCDF file
 * using its unique identifier (`netcdfID`)
 *
 * @param netcdfID
 *     The unique identifier for the NetCDF file, used to retrieve the associated file
 *     object from the internal file map.
 * @param parentGroupName
 *     The name of the parent group under which the new group will be added. If this is
 *     the root group, it should typically be an empty string or a top-level group name.
 * @param groupName
 *     The name of the new group to be created within the specified parent group.
 *
 * @return
 *     - 0 on success.
 *     - A non-zero error code if an exception is encountered (e.g., invalid group name,
 *       parent group not found, or other NetCDF-related errors).
 *
 * @throws netCDF::exceptions::NcException
 *     If any NetCDF-related operation fails, this exception is caught and processed
 *     to generate an appropriate error message using `netcdfErrorMessage`.
 *
 * @note
 * - This function requires the `FileMap` and `getRootGroup` utilities to manage NetCDF
 *   file and group objects.
 * - Ensure that `netcdfID` is valid and corresponds to an open file managed by the
 *   `FileMap` instance.
 *
 * Example Usage:
 * ```
 * int netcdfID = 1; // ID of an open NetCDF file
 * const char* parentGroupName = "parent_group";
 * const char* groupName = "new_group";
 * int result = Obs2Ioda::netcdfAddGroup(netcdfID, parentGroupName, groupName);
 * if (result != 0) {
 *     // Handle error
 * }
 * ```
 */

        int netcdfAddGroup(
                int netcdfID,
                const char *parentGroupName,
                const char *groupName
        );

    }

}

#endif //NETCDF_GROUP_H
