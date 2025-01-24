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
