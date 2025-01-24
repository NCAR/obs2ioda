#ifndef NETCDF_UTILS_H
#define NETCDF_UTILS_H

#include <netcdf>
#include <memory>

/**
 * @namespace Obs2Ioda
 * @brief Utilities for handling NetCDF-related operations in the Obs2Ioda library.
 */
namespace Obs2Ioda {

    /**
     * @brief Retrieves the parent NetCDF group associated with a given NetCDF file ID.
     *
     * @param netcdfID The unique identifier for the NetCDF file.
     * @param groupName The name of the group within the NetCDF file.
     *                  If nullptr, the root group of the file will be returned.
     * @return A shared pointer to the requested NetCDF group.
     * @exception netCDF::exceptions::NcException Thrown if the groupName is invalid
     *            or the NetCDF operation fails.
     */
    std::shared_ptr<netCDF::NcGroup> getParentGroup(
            int netcdfID,
            const char *groupName
    );

    /**
     * @brief Removes all whitespace characters from the provided string.
     *
     * @param name The input string from which whitespace should be removed.
     * @return A new string with all whitespace characters removed.
     */
    std::string removeWhiteSpace(
            const std::string &name
    );

}

#endif //NETCDF_UTILS_H
