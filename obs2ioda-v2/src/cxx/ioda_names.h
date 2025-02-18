#ifndef OBS2IODA_IODA_NAMES_H
#define OBS2IODA_IODA_NAMES_H

#include <unordered_map>
#include <string>

namespace Obs2Ioda {
    extern std::unordered_map<std::string, std::string>
    IODA_VARIABLE_NAMES;

    extern std::unordered_map<std::string, std::string>
    IODA_DIMENSION_NAMES;

    std::string getIodaName(
        const std::string &name,
        const std::unordered_map<std::string, std::string> &iodaNameMap
    );
}

#endif //OBS2IODA_IODA_NAMES_H
