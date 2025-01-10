#include <sstream>
#include "netcdf_error.h"

namespace Obs2Ioda {

    int netcdfErrorMessage(
            const netCDF::exceptions::NcException &e,
            int lineNumber,
            const std::string& fileName
    ) {
        std::stringstream message;
        message << "NetCDF Error" << std::endl;
        message << "Error code: " << e.errorCode() << std::endl;
        if (not fileName.empty()) {
            message << "Obs2Ioda File: " << fileName << std::endl;
            if (lineNumber > 0) {
                message << "Obs2Ioda Line: " << lineNumber << std::endl;
            }
        }
        message << "Message: " << e.what() << std::endl;
        std::cerr << message.str();
        return e.errorCode();
    }
}
