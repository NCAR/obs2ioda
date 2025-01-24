#include "netcdf_utils.h"
#include "netcdf_file.h"
#include <algorithm>
#include <iostream>

namespace Obs2Ioda {

    std::shared_ptr<netCDF::NcGroup> getParentGroup(
            int netcdfID,
            const char *groupName
    ) {
        auto file = FileMap::getInstance().getFile(netcdfID);
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
}
