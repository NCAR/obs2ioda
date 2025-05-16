#include "netcdf_file.h"

#include <FilePathConfig.h>

#include "netcdf_error.h"
#include "ioda_obs_schema_map.h"


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

    std::shared_ptr<netCDF::NcFile>
    FileMap::getFile(const int netcdfID) {
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
            auto iodaSchema = std::make_shared<IodaObsSchema>(
                YAML::LoadFile(IODA_SCHEMA_YAML)
            );
            iodaSchema->addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
            iodaSchema->addVariableRegexPattern(
                R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
            iodaSchema->addGroupRegexPattern(R"(@([a-zA-Z0-9_]+))");
            IodaObsSchemaMap::getInstance().addIodaObsSchema(
                *netcdfID,
                iodaSchema
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
