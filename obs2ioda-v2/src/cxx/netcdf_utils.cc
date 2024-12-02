#include "netcdf_utils.h"
#include "ioda_names.h"
#include <algorithm>
#include <iostream>

namespace Obs2Ioda {

    std::unordered_map<
            int,
            std::shared_ptr<netCDF::NcFile>> NETCDF_FILE_MAP;
    std::mutex map_mutex;

    std::shared_ptr<netCDF::NcGroup> getRootGroup(
            int netcdfID,
            const char *groupName
    ) {
        auto file = NETCDF_FILE_MAP[netcdfID];
        if (groupName != nullptr) {
            return std::make_shared<netCDF::NcGroup>(file->getGroup(groupName));
        }
        return file;
    }

    std::string removeWhiteSpace(
            const std::string &name
    ) {
        std::string strippedName = name;

        strippedName.erase(
                std::remove_if(
                        strippedName.begin(),
                        strippedName.end(),
                        [](unsigned char x) { return std::isspace(x); }
                ),
                strippedName.end()
        );
        return strippedName;
    }


    std::string getIodaName(
            const char *name,
            const std::unordered_map<
                    std::string,
                    std::string
            > &iodaNameMap
    ) {
        // Remove white spaces from the name
        std::string iodaName = removeWhiteSpace(name);

        // Check if the name exists in the map
        if (iodaNameMap.find(iodaName) != iodaNameMap.end()) {
            return iodaNameMap.at(iodaName);
        }
        return iodaName;
    }

    int netcdfErrorMessage(
            netCDF::exceptions::NcException &e,
            int errorCode
    ) {
        std::cerr
                << "NetCDF Error: "
                << e.what()
                << std::endl;
        return errorCode;
    }
}
