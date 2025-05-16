#include "netcdf_file.h"
#include "netcdf_error.h"
#include <memory>
#include <ioda_obs_schema_map/ioda_obs_schema_map.h>


namespace Obs2Ioda {

    FileMap &FileMap::getInstance() {
        static FileMap instance;
        return instance;
    }

    void FileMap::addFile(
        const int netcdfID,
        const std::shared_ptr<netCDF::NcFile> &file
    ) {
        auto netcdfFileIterator = this->fileMap.find(netcdfID);
        if (netcdfFileIterator != this->fileMap.end()) {
            throw netCDF::exceptions::NcCantCreate(
                "NetCDF ID already exists in the NetCDF file map",
                __FILE__,
                __LINE__
            );
        }
        this->fileMap[netcdfID] = file;
    }


    void FileMap::removeFile(
        const int netcdfID
    ) {
        auto netcdfFileIterator = this->fileMap.find(netcdfID);
        if (netcdfFileIterator == this->fileMap.end()) {
            throw netCDF::exceptions::NcBadId(
                "NetCDF ID not found in the NetCDF file map",
                __FILE__,
                __LINE__
            );
        }
        this->fileMap.erase(netcdfFileIterator);
    }

    std::shared_ptr<netCDF::NcFile> FileMap::getFile(const int netcdfID) {
        const auto netcdfFileIterator = this->fileMap.find(netcdfID);
        if (netcdfFileIterator == this->fileMap.end()) {
            throw netCDF::exceptions::NcBadId(
                "NetCDF ID not found in the NetCDF file map",
                __FILE__,
                __LINE__
            );
        }
        return netcdfFileIterator->second;
    }

    int netcdfCreate(
        const char *path,
        int *netcdfID,
        int fileMode
    ) {
        try {
            const auto file = std::make_shared<netCDF::NcFile>(
                path,
                static_cast<netCDF::NcFile::FileMode>(fileMode)
            );
            *netcdfID = file->getId();
            FileMap::getInstance().addFile(
                *netcdfID,
                file
            );
            const auto iodaObsSchema = std::make_shared<IodaObsSchema>(YAML::LoadFile(IODA_SCHEMA_YAML));
            IodaObsSchemaMap::getInstance().addIodaObsSchema(
                *netcdfID,
                iodaObsSchema
            );
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    int netcdfClose(const int netcdfID) {
        try {
            FileMap::getInstance().getFile(netcdfID)->close();
            FileMap::getInstance().removeFile(netcdfID);
            IodaObsSchemaMap::getInstance().removeIodaObsSchema(netcdfID);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }
}
