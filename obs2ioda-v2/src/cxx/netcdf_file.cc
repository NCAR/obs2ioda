#include "netcdf_file.h"
#include "netcdf_error.h"
#include <memory>

namespace Obs2Ioda {

    FileMap &FileMap::getInstance() {
        static FileMap instance;
        return instance;
    }

    int FileMap::addFile(const int netcdfID, const std::shared_ptr<netCDF::NcFile> &file) {
        auto netcdfFileIterator = this->fileMap.find(netcdfID);
        if (netcdfFileIterator != this->fileMap.end()) {
            throw netCDF::exceptions::NcCantCreate(
                "NetCDF ID already exists in the NetCDF file map",
                __FILE__,
                __LINE__
            );
        }
        this->fileMap[netcdfID] = file;
        return 0;
    }


    int FileMap::removeFile(const int netcdfID) {
        auto netcdfFileIterator = this->fileMap.find(netcdfID);
        if (netcdfFileIterator == this->fileMap.end()) {
            throw netCDF::exceptions::NcBadId(
                "NetCDF ID not found in the NetCDF file map",
                __FILE__,
                __LINE__
            );
        }
        this->fileMap.erase(netcdfFileIterator);
        return 0;
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
        int *netcdfID
    ) {
        try {
            const auto file = std::make_shared<netCDF::NcFile>(
                path,
                netCDF::NcFile::replace
            );
            *netcdfID = file->getId();
            FileMap::getInstance().addFile(
                *netcdfID,
                file
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
            return FileMap::getInstance().removeFile(netcdfID);
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }
}
