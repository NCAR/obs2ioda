#ifndef OBS2IODA_NETCDF_FILE_MAP_H
#define OBS2IODA_NETCDF_FILE_MAP_H

#include <netcdf>
#include <unordered_map>
#include <memory>
#include "ioda_obs_schema/ioda_obs_schema.h"

namespace Obs2Ioda {
    extern IodaObsSchema iodaSchema;
    /**
     * @class FileMap
     * @brief Singleton class for managing a mapping of NetCDF file IDs to file objects.
     */
    class FileMap {
    public:
        /**
         * @brief Retrieves the singleton instance of the NetcdfFileMap.
         *
         * Ensures there is only one instance of the NetcdfFileMap throughout the application.
         *
         * @return A reference to the singleton instance of NetcdfFileMap.
         */
        static FileMap &getInstance();

        /**
         * @brief Deleted copy constructor to enforce singleton behavior.
         */
        FileMap(
            const FileMap &
        ) = delete;

        /**
         * @brief Deleted assignment operator to enforce singleton behavior.
         */
        FileMap &operator=(
            const FileMap &
        ) = delete;

        /**
         * @brief Adds a NetCDF file to the map.
         *
         * Associates a unique NetCDF file ID with a `std::shared_ptr` to a `netCDF::NcFile` object.
         * Throws an exception if the ID already exists in the map.
         *
         * @param netcdfID The unique NetCDF file ID.
         * @param file A shared pointer to the NetCDF file to be added.
         * @throws netCDF::exceptions::NcCantCreate if the `netcdfID` already exists in the map.
         */
        void addFile(
            int netcdfID,
            const std::shared_ptr<netCDF::NcFile> &file
        );

        /**
         * @brief Removes a NetCDF file from the map.
         *
         * Removes the association of the given NetCDF file ID from the map.
         * Throws an exception if the ID does not exist in the map.
         *
         * @param netcdfID The unique NetCDF file ID to be removed.
         * @throws netCDF::exceptions::NcBadId if the `netcdfID` does not exist in the map.
         */
        void removeFile(
            int netcdfID
        );

        /**
         * @brief Retrieves a NetCDF file from the map.
         *
         * Retrieves the `std::shared_ptr` to the `netCDF::NcFile` object associated with the given
         * NetCDF file ID.
         *
         * @param netcdfID The unique NetCDF file ID to retrieve.
         * @return A shared pointer to the NetCDF file.
         * @throws netCDF::exceptions::NcBadId if the `netcdfID` does not exist in the map.
         */
        std::shared_ptr<netCDF::NcFile> getFile(
            int netcdfID
        );

    private:
        /**
         * @brief Private constructor to prevent direct instantiation.
         */
        FileMap() = default;

        /// Map associating NetCDF file IDs with their corresponding shared pointers to NetCDF files.
        std::unordered_map<int, std::shared_ptr<netCDF::NcFile> >
        fileMap;
    };
} // namespace Obs2Ioda

#endif // OBS2IODA_NETCDF_FILE_MAP_H
