#include "netcdf_utils.h"
#include "netcdf_file.h"
#include <algorithm>
#include <iostream>

namespace Obs2Ioda {

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
