#ifndef OBS2IODA_IODA_NAMES_H
#define OBS2IODA_IODA_NAMES_H

#include <unordered_map>
#include <string>

namespace Obs2Ioda {
    extern std::unordered_map<
            std::string,
            std::string
    > IODA_VARIABLE_NAMES;

}

#endif //OBS2IODA_IODA_NAMES_H
